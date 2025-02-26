#include <iostream>
#include <cstdint>
#include <cassert>
#include <limits>
#include <iomanip>

#include "td/utils/base64.h"
#include "td/utils/crypto.h"
#include "vm/boc.h"

using namespace std;
// TODO: remove static

static constexpr uint32_t
	BOC_HEAD = 0xb5ee9c72u,
	REF_SIZE = 2,
	OFFSET_SIZE = 3,
	kLenNumLowBits = 3,
	kLenNumHighBits = 8,
	kLenNumLowSymbols = 1 << kLenNumLowBits,
	kLenNumHighSymbols = 1 << kLenNumHighBits,
	kNumPosBitsMax = 4,
	kNumPosStatesMax = 1 << kNumPosBitsMax,
	LenHigh = 2 * (kNumPosStatesMax << kLenNumLowBits),
	kNumLenProbs = LenHigh + kLenNumHighSymbols,
	kNumBitModelTotalBits = 11,
	kBitModelTotal = 1 << kNumBitModelTotalBits,
	kNumMoveReducingBits = 4,
	kNumAlignBits = 4,
	kAlignTableSize = 1 << kNumAlignBits,
	kNumLenToPosStates = 4,
	kDicLogSizeMax = 32,
	kDistTableSizeMax = kDicLogSizeMax * 2,
	kEndPosModelIndex = 14,
	kNumFullDistances = 1 << (kEndPosModelIndex >> 1),
	kNumStates = 12,
	kNumStates2 = 16,
	kNumPosSlotBits = 6,
	kNumLogBits = 11 + sizeof(size_t) / 8 * 3,
	kNumOpts = 1 << 11,
	kNumBitPriceShiftBits = 4,
	kInfinityPrice = 1 << 30,
	kStartPosModelIndex = 4,
	kNumMoveBits = 5,
	kTopValue = 1 << 24,
	kAlignMask = kAlignTableSize - 1,
	kState_RepAfterLit = 8,
	kState_MatchAfterLit = 7,
	kState_LitAfterRep = 5,
	kState_LitAfterMatch = 4,
	kStartOffset = 1664,
	kMatchMinLen = 2,
	kMatchSpecLenStart = kMatchMinLen + kLenNumLowSymbols * 2 + kLenNumHighSymbols,
	kMatchSpecLen_Error_Data = 1 << 9,
	kMatchSpecLen_Error_Fail = kMatchSpecLen_Error_Data - 1,
	kRange0 = 0xFFFFFFFFu,
	kBound0 = (kRange0 >> kNumBitModelTotalBits) << (kNumBitModelTotalBits - 1),
	kBadRepCode = kBound0 + (((kRange0 - kBound0) >> kNumBitModelTotalBits) << (kNumBitModelTotalBits - 1)),
	kNumLitStates = 7;
static constexpr uint16_t kProbInitValue = kBitModelTotal >> 1;
static constexpr uint8_t
	kMatchNextStates[kNumStates] = {7, 7, 7, 7, 7, 7, 7, 10, 10, 10, 10, 10},
	kShortRepNextStates[kNumStates] = {9, 9, 9, 9, 9, 9, 9, 11, 11, 11, 11, 11},
	kLiteralNextStates[kNumStates] = {0, 0, 0, 0, 1, 2, 3, 4,  5,  6,   4, 5},
	kRepNextStates[kNumStates] = {8, 8, 8, 8, 8, 8, 8, 11, 11, 11, 11, 11};
static constexpr uint32_t
	LZMA_NUM_REPS = 4,
	LZMA_MATCH_LEN_MIN = 2,
	LZMA_PB_MAX = 4,
	LZMA_NUM_PB_STATES_MAX = 1 << LZMA_PB_MAX,
	LZMA_REQUIRED_INPUT_MAX = 20,
	LZMA_LIT_SIZE = 0x300,
	RC_BUF_SIZE = 1 << 16,
	RC_INIT_SIZE = 5,
	REP_LEN_COUNT = 64;
static constexpr int
	SZ_ERROR_DATA = 1,
	SZ_ERROR_INPUT_EOF = 6,
	SZ_ERROR_FAIL = 11,
	SpecPos = -kStartOffset,
	IsRep0Long = SpecPos + kNumFullDistances,
	RepLenCoder = IsRep0Long + (kNumStates2 << kNumPosBitsMax),
	LenCoder = RepLenCoder + kNumLenProbs,
	IsMatch = LenCoder + kNumLenProbs,
	Align = IsMatch + (kNumStates2 << kNumPosBitsMax),
	IsRep = Align + kAlignTableSize,
	IsRepG0 = IsRep + kNumStates,
	IsRepG1 = IsRepG0 + kNumStates,
	IsRepG2 = IsRepG1 + kNumStates,
	PosSlot = IsRepG2 + kNumStates,
	Literal = PosSlot + (kNumLenToPosStates << kNumPosSlotBits),
	NUM_BASE_PROBS = Literal + kStartOffset,
	MAINNET_ID = -239;

static constexpr uint32_t
	dictSize = 1<<20, // 1<<12 <= dictSize
	distTableSize = 20,
	fb = 128, // 5 <= fb <= 273
	mc = 16u + (fb >> 1), // 1 <= mc <= 1<<30
	keepSizeAfter = 2*fb+1,
	hashMask = (1<<16)-1,
	cyclicBufferSize = dictSize + 1,
	seq_no_min = 39500000,
	gen_utime_min = 1727733600,
	lt_mul = 1000000;

static constexpr uint32_t tabSize = fb + 1 - LZMA_MATCH_LEN_MIN;

struct BitWriter {
	vector<uint8_t> data;
	int i = 0, j = 0;
	void write(int bits, uint64_t x) {
		if(!bits) return;
		if(i == (int)data.size()) data.push_back(0);
		const int r = 8-j;
		if(bits < 64) x &= (1ull<<bits)-1ull;
		if(bits < r) {
			data[i] &= ~((1<<r) - (1<<(r-bits)));
			data[i] |= x << (r-bits);
			j += bits;
			return;
		}
		data[i] &= ~((1<<r) - 1);
		data[i++] |= (x>>(bits-=r));
		while(bits >= 8) {
			if(i == (int)data.size()) data.push_back(0);
			data[i++] = (x>>(bits-=8)) & 0xffu;
		}
		j = bits;
		if(j) {
			if(i == (int)data.size()) data.push_back(0);
			const int r = 8-j;
			data[i] &= (1<<r)-1;
			data[i] |= (x<<r)&0xffu;
		}
	}
	void copy(int bytes, const uint8_t* buf) {
		if(!bytes) return;
		if(j) {
			if(i+bytes >= (int)data.size()) data.resize(i+bytes+1, 0);
			const int r = 8-j;
			const uint8_t m = (1<<r)-1;
			for(int k = 0; k < bytes; ++k) {
				data[i] &= ~m;
				data[i++] |= buf[k] >> j;
				data[i] &= m;
				data[i] |= (buf[k]<<r)&0xffu;
			}
		} else {
			if(i+bytes > (int)data.size()) data.resize(i+bytes, 0);
			memcpy(data.data()+i, buf, bytes);
			i += bytes;
		}
	}
	void copyb(int bits, const uint8_t* buf) {
		const int bytes = bits/8, r = bits%8;
		copy(bytes, buf);
		if(r) write(r, buf[bytes]>>(8-r));
	}
	void skip(int bytes) {
		i += bytes;
		if(j) {
			if(i >= (int)data.size()) data.resize(i+1, 0);
		} else if(i > (int)data.size()) data.resize(i, 0);
	}
	struct Pos {
		int i, j;
		int operator-(const Pos &p) { return 8*(i-p.i)+j-p.j; }
	};
	Pos pos() const { return {i, j}; }
	void setPos(const Pos &p) { i = p.i; j = p.j; }
	td::BufferSlice toSlice() {
		td::BufferSlice slice(data.size());
		copy_n(data.data(), data.size(), slice.data());
		return slice;
	}
};

struct BitReader {
	const uint8_t* data;
	int tmp_s = 0;
	BitReader(const uint8_t* data): data(data) {}
	uint64_t read(int bits) {
		const int r = 8 - tmp_s;
		if(bits < r) {
			tmp_s += bits;
			return ((*data) >> (r-bits)) & ((1<<bits)-1); 
		}
		uint64_t x = (*data++) & ((1<<r)-1);
		bits -= r;
		while(bits >= 8) {
			x = (x<<8) | (*data++);
			bits -= 8;
		}
		x <<= bits;
		tmp_s = bits;
		return x |= (*data) >> (8-bits);
	}
	void copy(int bytes, uint8_t* buf) {
		const int r = 8 - tmp_s;
		while(bytes--) {
			const uint8_t x = (*data) << tmp_s;
			++data;
			*buf++ = x | ((*data) >> r);
		}
	}
	void copyb(int bits, uint8_t* buf) {
		const int bytes = bits/8, r = bits%8;
		copy(bytes, buf);
		if(r) *(buf += bytes)++ = ((read(r)<<1)|1)<<(8-r-1);
	}
	struct Pos {
		const uint8_t* data;
		int tmp_s = 0;
		int operator-(const Pos &p) { return 8*(data-p.data)+tmp_s-p.tmp_s; }
	};
	Pos pos() const { return {data, tmp_s}; }
	void setPos(const Pos &p) { data = p.data; tmp_s = p.tmp_s; }
};

int lenbits(uint32_t x) { return 32 - td::count_leading_zeroes32(x); }

enum TLBtype {
	HASHMAP,
	HASHMAP_AUG,

	END_COULD_BE_PRUNED,

	BLOCK,
	BLOCK_INFO,
	CURRENCY_COLLECTION_4,
	VALUE_FLOW,
	MERKLE_UPDATE,
	SHARD_STATE,
	SHARD_STATE_UNSPLIT,
	BLOCK_EXTRA,
	SHARD_ACCOUNTS,
	OUT_MSG_DESCR,
	OUT_MSG,
	TRANSACTION,
	IN_MSG_DESCR,

	END_HANDLED_CELLS,

	CURRENCY_COLLECTION,
	VAR_UINT_32,
	SHARD_ACCOUNT,
	DEPTH_BALANCE_INFO,
	IMPORT_FEES,
	IN_MSG,

	END_IMPLEMENTED,

	MC_BLOCK_EXTRA,
	BLK_MASTER_INFO,
	BLK_PREV_INFO,
	OUT_MSG_QUEUE_INFO,
	SHARD_STATE_TMP,
	MC_STATE_EXTRA,
	SHARD_ACCOUNT_BLOCKS,
	ACCOUNT,
	MESSAGE,
	ANY,
	MSG_ENVELOPE,
	TRANSACTION_DESCR,
	HASH_UPDATE,
	TRANSACTION_TMP,

	UNKNOWN
};

struct TLBtype2 {
	TLBtype type;
	uint32_t var1, var2, var3;
	bool special = true;
	constexpr TLBtype2(TLBtype type=UNKNOWN, uint32_t var1=0, uint32_t var2=0, uint32_t var3=0):
		type(type), var1(var1), var2(var2), var3(var3) {}
};

void propagate(const TLBtype2 &a, TLBtype2 &b, const TLBtype2 &b2) {
	if(a.type == UNKNOWN) return;
	const bool s = b.special;
	if(b.type == UNKNOWN) (b = b2).special = s;
	if(a.type != MERKLE_UPDATE) b.special &= a.special;
}

struct Bits256 {
	uint8_t x[32];
};
ostream& operator<<(ostream&os, const Bits256 &x) {
	os << hex << setfill('0');
	for(uint8_t x : x.x) os << setw(2) << (uint32_t) x;
	return os << dec;
};

template<typename T, bool b=true>
struct Some;
template<typename T>
struct Some<T, true> { T v; constexpr Some(const T &v): v(v) {} };
template<typename T>
struct Some<T, false> { constexpr Some(const T &v) {} };

bool READ_ASSERT(BitReader &r) { return true; }
template<typename T, typename... Args>
bool READ_ASSERT(BitReader &r, T x, Args... y) {
	if constexpr (is_same_v<T, bool>) { if(x != r.read(1)) return false; }
	else if constexpr (numeric_limits<T>::is_integer) { if(x != (T) r.read(8*sizeof(T))) return false; }
	else static_assert(false);
	return READ_ASSERT(r, y...);
}
void READ(BitReader &r) {}
template<typename T, typename... Args>
void READ(BitReader &r, T& x, Args&... y) {
	if constexpr (is_same_v<T, bool>) x = r.read(1);
	else if constexpr (numeric_limits<T>::is_integer) x = r.read(8*sizeof(T));
	else if constexpr (is_same_v<T, Bits256>) r.copy(32, x.x);
	else static_assert(false);
	READ(r, y...);
}
void WRITE(BitWriter &w) {}
template<typename T, typename... Args>
void WRITE(BitWriter &w, T x, Args... y) {
	if constexpr (is_same_v<T, bool>) w.write(1, x);
	else if constexpr (numeric_limits<T>::is_integer) w.write(8*sizeof(T), x);
	else if constexpr (is_same_v<T, Bits256>) w.copy(32, x.x);
	else static_assert(false);
	WRITE(w, y...);
}

map<uint32_t, pair<Bits256, uint64_t>> trans_map;
bool do_align;
uint64_t lt0_high;
bool lt0_hight_w;

void clearStuff() {
	trans_map.clear();
	lt0_high = 0;
	lt0_hight_w = false;
	do_align = false;
}

void readLt(BitReader &r, uint64_t &x) {
	x = (r.read(24) + seq_no_min) * lt_mul + r.read(8);
}
void writeLt(BitWriter &w, uint64_t x) {
	w.write(24, x/lt_mul - seq_no_min);
	w.write(8, x%lt_mul);
}
bool checkLt(uint64_t x) {
	return (x%lt_mul) < 256 && x/lt_mul >= seq_no_min && x/lt_mul - seq_no_min < (1<<24);
}

struct Block {
	static constexpr uint32_t tag = 0x11ef55aau;
	static constexpr int global_id = MAINNET_ID;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(!S) assert(READ_ASSERT(r, tag, global_id));
		*tlbs++ = BLOCK_INFO;
		*tlbs++ = VALUE_FLOW;
		*tlbs++ = TLBtype2(MERKLE_UPDATE, SHARD_STATE);
		*tlbs++ = BLOCK_EXTRA;
	}
	template<int S>
	void write(BitWriter &w) { if(!S) WRITE(w, tag, global_id); }
};

struct ShardIdent {
	uint8_t shard_pfx_bits;
	int workchain_id;
	uint64_t shard_prefix;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(S) {
			workchain_id = (shard_pfx_bits = r.read(4)) ? 0 : -1;
			shard_prefix = r.read(4)<<60;
		} else {
			READ(r, shard_pfx_bits, workchain_id, shard_prefix);
			assert(shard_pfx_bits == 2 || shard_pfx_bits == 0);
			if(shard_pfx_bits) assert(!workchain_id);
			else assert(workchain_id == -1);
			assert(shard_prefix == (shard_prefix & (0xfull<<60)));
		}
	}
	template<int S>
	void write(BitWriter &w) {
		if(S) {
			w.write(4, shard_pfx_bits);
			w.write(4, shard_prefix>>60);
		} else WRITE(w, shard_pfx_bits, workchain_id, shard_prefix);
	}
};

struct GlobalVersion {
	static constexpr uint8_t tag = 0xc4u;
	static constexpr uint64_t capabilities = 494;
	uint32_t version;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(S) version = r.read(8);
		else {
			assert(READ_ASSERT(r, tag));
			READ(r, version);
			assert(version < 256);
			assert(READ_ASSERT(r, capabilities));
		}
	}
	template<int S>
	void write(BitWriter &w) {
		if(S) w.write(8, version);
		else WRITE(w, tag, version, capabilities);
	}
};

// TODO: compress: start_lt
struct BlockInfo {
	static constexpr uint32_t
		tag = 0x9bc7a987u,
		version = 0,
		vert_seq_no = 1;
	static constexpr bool vert_seqno_incr = false;
	bool not_master, after_merge, before_split, after_split, want_split, want_merge, key_block, flags;
	uint32_t seq_no, gen_utime, gen_validator_list_hash_short, gen_catchain_seqno, min_ref_mc_seqno, prev_key_block_seqno;
	ShardIdent shard;
	uint64_t start_lt, end_lt;
	GlobalVersion gen_software;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(!S) assert(READ_ASSERT(r, tag, version));
		READ(r, not_master, after_merge, before_split, after_split, want_split, want_merge, key_block); 
		if(S) {
			READ(r, flags);
			seq_no = r.read(24)+seq_no_min;
			shard.read<S>(r, tlbs);
			gen_utime = r.read(24)+gen_utime_min;
		} else {
			assert(READ_ASSERT(r, vert_seqno_incr));
			flags = r.read(8);
			READ(r, seq_no); assert(seq_no >= seq_no_min && seq_no-seq_no_min < (1<<24));
			assert(READ_ASSERT(r, vert_seq_no));
			shard.read<S>(r, tlbs);
			READ(r, gen_utime); assert(gen_utime >= gen_utime_min);
		}
		READ(r, start_lt, end_lt, gen_validator_list_hash_short, gen_catchain_seqno, min_ref_mc_seqno, prev_key_block_seqno);
		if(flags) gen_software.read<S>(r, tlbs);
		if(not_master) *tlbs++ = BLK_MASTER_INFO;
		*tlbs++ = TLBtype2(BLK_PREV_INFO, after_merge);
		if(vert_seqno_incr) *tlbs++ = TLBtype2(BLK_PREV_INFO, 0);
	}
	template<int S>
	void write(BitWriter &w) {
		if(!S) WRITE(w, tag, version);
		WRITE(w, not_master, after_merge, before_split, after_split, want_split, want_merge, key_block);
		if(S) {
			WRITE(w, flags);
			w.write(24, seq_no-seq_no_min);
			shard.write<S>(w);
			w.write(24, gen_utime-gen_utime_min);
		} else {
			WRITE(w, vert_seqno_incr);
			w.write(8, flags);
			WRITE(w, seq_no, vert_seq_no);
			shard.write<S>(w);
			WRITE(w, gen_utime);
		}
		WRITE(w, start_lt, end_lt, gen_validator_list_hash_short, gen_catchain_seqno, min_ref_mc_seqno, prev_key_block_seqno);
		if(flags) gen_software.write<S>(w);
	}
};

template<uint32_t n>
struct VarUInteger {
	static constexpr Some<TLBtype, n==32> TLB = VAR_UINT_32;
	uint32_t len;
	uint8_t value[n-1];
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		len = r.read(lenbits(n-1));
		r.copy(len, value);
	}
	template<int S>
	void write(BitWriter &w) {
		w.write(lenbits(n-1), len);
		w.copy(len, value);
	}
};
using Grams = VarUInteger<16>;

struct ShardAccount {
	static constexpr Some<TLBtype> TLB = SHARD_ACCOUNT;
	Bits256 last_trans_hash;
	uint64_t last_trans_lt;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		*tlbs++ = ACCOUNT;
		if(S) {
			r.copy(4, last_trans_hash.x);
			const uint32_t start = *(uint32_t*)last_trans_hash.x;
			auto it = trans_map.lower_bound(start);
			if(it == trans_map.end() || it->first != start) {
				r.copy(28, last_trans_hash.x+4);
				READ(r, last_trans_lt);
				trans_map.emplace_hint(it, start, make_pair(last_trans_hash, last_trans_lt));
			} else tie(last_trans_hash, last_trans_lt) = it->second;
		} else {
			READ(r, last_trans_hash, last_trans_lt);
		}
	}
	template<int S>
	void write(BitWriter &w) {
		if(S) {
			const uint32_t start = *(uint32_t*)last_trans_hash.x;
			auto it = trans_map.lower_bound(start);
			if(it == trans_map.end() || it->first != start) {
				trans_map.emplace_hint(it, start, make_pair(last_trans_hash, last_trans_lt));
				WRITE(w, last_trans_hash, last_trans_lt);
			} else w.copy(4, last_trans_hash.x);
		} else WRITE(w, last_trans_hash, last_trans_lt);
	}
};

struct HmLabel {
	const uint32_t m;
	uint32_t n;
	uint8_t tag;
	uint8_t s[32];
	bool v;
	HmLabel(uint32_t m): m(m) { assert(m <= 8*32); }
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if((tag = r.read(1))) {
			if((tag = 2|r.read(1))&1) {
				READ(r, v);
				n = r.read(lenbits(m));
			} else {
				n = r.read(lenbits(m));
				r.copyb(n, s);
			}
		} else {
			n = 0;
			if(S && m <= 32) return;
			while(r.read(1)) ++n;
			r.copyb(n, s);
			if(!S && m <= 32) assert(!n);
		}
	}
	template<int S>
	void write(BitWriter &w) {
		w.write(tag ? 2 : 1, tag);
		if(tag) {
			if(tag&1) {
				WRITE(w, v);
				w.write(lenbits(m), n);
			} else {
				w.write(lenbits(m), n);
				w.copyb(n, s);
			}
		} else {
			if(S && m <= 32) return;
			for(uint32_t i = 0; i < n; ++i) w.write(1, 1);
			w.write(1, 0);
			w.copyb(n, s);
		}
	}
};

template<typename X>
struct Hashmap {
	const uint32_t n;
	HmLabel label;
	X value;
	Hashmap(uint32_t n): n(n), label(n) {}
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		label.read<S>(r, tlbs);
		const int m = n - label.n;
		if(m) {
			*tlbs++ = TLBtype2(HASHMAP, m-1, X::TLB.v);
			*tlbs++ = TLBtype2(HASHMAP, m-1, X::TLB.v);
		} else value.template read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		label.write<S>(w);
		if(label.n == n) value.template write<S>(w);
	}
};

template<typename X>
struct HashmapE {
	const uint32_t n;
	bool tag;
	HashmapE(uint32_t n): n(n) {}
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		READ(r, tag);
		if(tag) *tlbs++ = TLBtype2(HASHMAP, n, X::TLB.v);
	}
	template<int S>
	void write(BitWriter &w) { WRITE(w, tag); }
};
template<uint32_t _n, typename X>
struct _HashmapE : HashmapE<X> { _HashmapE(): HashmapE<X>(_n) {} };
using ExtraCurrencyCollection = _HashmapE<32, VarUInteger<32>>;

struct CurrencyCollection {
	static constexpr Some<TLBtype> TLB = CURRENCY_COLLECTION;
	Grams grams;
	ExtraCurrencyCollection other;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		grams.read<S>(r, tlbs);
		other.read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		grams.write<S>(w);
		other.write<S>(w);
	}
};

struct CurrencyCollection4 {
	CurrencyCollection cc[4];
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		for(CurrencyCollection &c : cc) c.read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		for(CurrencyCollection &c : cc) c.write<S>(w);
	}
};

struct DepthBalanceInfo {
	static constexpr Some<TLBtype> TLB = DEPTH_BALANCE_INFO;
	uint8_t split_depth;
	CurrencyCollection balance;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		split_depth = r.read(5);
		balance.read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		w.write(5, split_depth);
		balance.write<S>(w);
	}
};

struct ImportFees {
	static constexpr Some<TLBtype> TLB = IMPORT_FEES;
	Grams fee_collected;
	CurrencyCollection value_imported;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		fee_collected.read<S>(r, tlbs);
		value_imported.read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		fee_collected.write<S>(w);
		value_imported.write<S>(w);
	}
};

struct OutMsg {
	static constexpr Some<TLBtype> TLB = OUT_MSG;
	uint8_t tag;
	uint64_t import_block_lt, next_addr_pfx;
	Bits256 msg_env_hash;
	int next_workchain;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		tag = r.read(3);
		if(tag == 0b101) tag = 0b10100 | r.read(2);
		if(tag == 0b110) tag = 0b1100 | r.read(1);
		switch(tag) {
		case 0b000: *tlbs++ = TLBtype2(MESSAGE, ANY); *tlbs++ = TRANSACTION; break;
		case 0b010: *tlbs++ = MSG_ENVELOPE; *tlbs++ = TRANSACTION; *tlbs++ = IN_MSG; break;
		case 0b001: case 0b10100: *tlbs++ = MSG_ENVELOPE; *tlbs++ = TRANSACTION; break;
		case 0b011: case 0b111: case 0b100: case 0b10101: *tlbs++ = MSG_ENVELOPE; *tlbs++ = IN_MSG; break;
		case 0b1100: *tlbs++ = MSG_ENVELOPE; import_block_lt = r.read(63); break;
		case 0b1101:
			READ(r, msg_env_hash);
			if(S) next_workchain = r.read(1) ? -1 : 0;
			else READ(r, next_workchain);
			READ(r, next_addr_pfx);
			if(S) readLt(r, import_block_lt);
			else {
				READ(r, import_block_lt);
				assert(checkLt(import_block_lt));
			}
			break;
		}
	}
	template<int S>
	void write(BitWriter &w) {
		w.write(tag < 8 ? 3 : (tag < 16 ? 4 : 5), tag);
		if(tag == 0b1100) w.write(63, import_block_lt);
		else if(tag == 0b1101) {
			WRITE(w, msg_env_hash);
			if(S) w.write(1, next_workchain == -1 ? 1 : 0);
			else WRITE(w, next_workchain);
			WRITE(w, next_addr_pfx);
			if(S) writeLt(w, import_block_lt);
			else WRITE(w, import_block_lt);
		}
	}
};

struct InMsg {
	static constexpr Some<TLBtype> TLB = IN_MSG;
	uint8_t tag, tag2;
	Grams fee;
	uint64_t transaction_id;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		tag = r.read(3);
		switch(tag) {
		case 0b000: *tlbs++ = TLBtype2(MESSAGE, ANY); *tlbs++ = TRANSACTION; break;
		case 0b010: *tlbs++ = TLBtype2(MESSAGE, ANY); *tlbs++ = TRANSACTION; fee.read<S>(r, tlbs); *tlbs++ = ANY; break;
		case 0b011: case 0b100: *tlbs++ = MSG_ENVELOPE; *tlbs++ = TRANSACTION; fee.read<S>(r, tlbs); break;
		case 0b101: *tlbs++ = MSG_ENVELOPE; *tlbs++ = MSG_ENVELOPE; fee.read<S>(r, tlbs); break;
		case 0b110: *tlbs++ = MSG_ENVELOPE; READ(r, transaction_id); fee.read<S>(r, tlbs); break;
		case 0b111: *tlbs++ = MSG_ENVELOPE; READ(r, transaction_id); fee.read<S>(r, tlbs); *tlbs++ = ANY; break;
		case 0b001:
			if((tag2 = r.read(2))) {
				*tlbs++ = MSG_ENVELOPE; *tlbs++ = MSG_ENVELOPE;
			} else {
				*tlbs++ = MSG_ENVELOPE; *tlbs++ = TRANSACTION; fee.read<S>(r, tlbs);
			}
			break;
		}
	}
	template<int S>
	void write(BitWriter &w) {
		w.write(3, tag);
		if(tag == 0b000) return;
		if(tag == 0b001) {
			w.write(2, tag2);
			if(tag2) return; 
		}
		fee.write<S>(w);
	}
};

template<typename X, typename Y>
struct HashmapAug {
	const uint32_t n;
	HmLabel label;
	Y extra;
	X value;
	HashmapAug(uint32_t n): n(n), label(n) {}
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		label.read<S>(r, tlbs);
		const int m = n - label.n;
		if(m) {
			*tlbs++ = TLBtype2(HASHMAP_AUG, m-1, X::TLB.v, Y::TLB.v);
			*tlbs++ = TLBtype2(HASHMAP_AUG, m-1, X::TLB.v, Y::TLB.v);
		}
		extra.template read<S>(r, tlbs);
		if(!m) value.template read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		label.write<S>(w);
		extra.template write<S>(w);
		if(label.n == n) value.template write<S>(w);
	}
};
using ShardAccounts2 = HashmapAug<ShardAccount, DepthBalanceInfo>;
using OutMsgDescr2 = HashmapAug<OutMsg, CurrencyCollection>;
using InMsgDescr2 = HashmapAug<InMsg, ImportFees>;

template<typename X, typename Y>
struct HashmapAugE {
	const uint32_t n;
	bool tag;
	Y extra;
	HashmapAugE(uint32_t n): n(n) {}
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		READ(r, tag);
		if(tag) *tlbs++ = TLBtype2(HASHMAP_AUG, n, X::TLB.v, Y::TLB.v);
		extra.template read<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		WRITE(w, tag);
		extra.template write<S>(w);
	}

};
template<uint32_t _n, typename X, typename Y>
struct _HashmapAugE : HashmapAugE<X, Y> { _HashmapAugE(): HashmapAugE<X, Y>(_n) {} };
using ShardAccounts = _HashmapAugE<256, ShardAccount, DepthBalanceInfo>;
using OutMsgDescr = _HashmapAugE<256, OutMsg, CurrencyCollection>;
using InMsgDescr = _HashmapAugE<256, InMsg, ImportFees>;

struct ValueFlow {
	bool tag;
	CurrencyCollection fees_collected, burned;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		tag = S ? r.read(1) : r.read(32) == 0x3ebf98b7u;
		fees_collected.read<S>(r, tlbs);
		if(tag) burned.read<S>(r, tlbs);
		*tlbs++ = CURRENCY_COLLECTION_4;
		*tlbs++ = CURRENCY_COLLECTION_4;
	}
	template<int S>
	void write(BitWriter &w) {
		if(S) WRITE(w, tag);
		else w.write(32, tag ? 0x3ebf98b7u : 0xb8e48dfbu);
		fees_collected.write<S>(w);
		if(tag) burned.write<S>(w);
	}
};

struct MerkleUpdate {
	const TLBtype X;
	MerkleUpdate(TLBtype X): X(X) {}
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		*tlbs++ = X;
		*tlbs++ = X;
	}
	template<int S>
	void write(BitWriter &w) {
		if(S) return;
		w.write(8, 4); //special
		w.skip(2*(32+2));
	}
};

template<TLBtype tlb>
struct MaybeRef {
	bool b;
	void read(BitReader &r, TLBtype2* &tlbs) { READ(r, b); if(b) *tlbs++ = tlb; }
	void write(BitWriter &w) { WRITE(w, b); }
};

struct ShardStateUnsplit {
	static constexpr uint32_t tag = 0x9023afe2u, vert_seq_no = 1;
	static constexpr int global_id = MAINNET_ID;
	static constexpr bool before_split = false;
	ShardIdent shard_id;
	uint32_t seq_no, gen_utime, min_ref_mc_seqno;
	uint64_t gen_lt;
	MaybeRef<MC_STATE_EXTRA> custom;
	template<int S>
	void read0(BitReader &r, TLBtype2* &tlbs) {
		if(!S) assert(READ_ASSERT(r, global_id));
		shard_id.read<S>(r, tlbs);
		if(S) {
			seq_no = r.read(24)+seq_no_min;
			gen_utime = r.read(24)+gen_utime_min;
			READ(r, gen_lt);
			min_ref_mc_seqno = r.read(24)+seq_no_min;
		} else {
			READ(r, seq_no);
			assert(seq_no >= seq_no_min && seq_no-seq_no_min < (1<<24));
			assert(READ_ASSERT(r, vert_seq_no));
			READ(r, gen_utime, gen_lt, min_ref_mc_seqno);
			assert(gen_utime >= gen_utime_min);
			assert(min_ref_mc_seqno >= seq_no_min && min_ref_mc_seqno-seq_no_min < (1<<24));
			assert(READ_ASSERT(r, before_split));
		}
		*tlbs++ = OUT_MSG_QUEUE_INFO;
		*tlbs++ = SHARD_ACCOUNTS;
		*tlbs++ = SHARD_STATE_TMP;
		custom.read(r, tlbs);
	}
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(!S) assert(READ_ASSERT(r, tag));
		read0<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		if(!S) WRITE(w, tag, global_id);
		shard_id.write<S>(w);
		if(S) {
			w.write(24, seq_no-seq_no_min);
			w.write(24, gen_utime-gen_utime_min);
			WRITE(w, gen_lt);
			w.write(24, min_ref_mc_seqno-seq_no_min);
		} else WRITE(w, seq_no, vert_seq_no, gen_utime, gen_lt, min_ref_mc_seqno, before_split);
		custom.write(w);
	}
};

struct ShardState {
	bool split;
	ShardStateUnsplit unsplit;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		split = S ? r.read(1) : r.read(32) == 0x5f327da5u;
		if(split) {
			*tlbs++ = SHARD_STATE_UNSPLIT;
			*tlbs++ = SHARD_STATE_UNSPLIT;
		} else unsplit.read0<S>(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		if(S) WRITE(w, split);
		if(split) {
			if(!S) w.write(32, 0x5f327da5u);
		} else unsplit.write<S>(w);
	}
};

struct BlockExtra {
	const uint32_t tag = 0x4a33f6fdu;
	Bits256 rand_seed, created_by;
	MaybeRef<MC_BLOCK_EXTRA> custom;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(!S) assert(READ_ASSERT(r, tag));
		*tlbs++ = IN_MSG_DESCR;
		*tlbs++ = OUT_MSG_DESCR;
		*tlbs++ = SHARD_ACCOUNT_BLOCKS;
		READ(r, rand_seed, created_by);
		custom.read(r, tlbs);
	}
	template<int S>
	void write(BitWriter &w) {
		if(!S) WRITE(w, tag);
		WRITE(w, rand_seed, created_by);
		custom.write(w);
	}
};

struct Transaction {
	static constexpr uint8_t tag = 0b0111;
	// account_addr random
	// TODO now always the same
	// many zeros sometimes
	Bits256 account_addr, prev_trans_hash;
	uint64_t lt, prev_trans_lt;
	uint32_t now;
	uint16_t outmsg_cnt;
	uint8_t orig_status, end_status;
	CurrencyCollection total_fees;
	template<int S>
	void read(BitReader &r, TLBtype2* &tlbs) {
		if(!S) assert(tag == r.read(4));
		READ(r, account_addr);
		if(S) {
			if(!lt0_high) {
				lt0_high = (seq_no_min + r.read(24)) * lt_mul;
			}
			*(uint64_t*)prev_trans_hash.x = lt0_high + r.read(8);
			reverse(prev_trans_hash.x, prev_trans_hash.x+8);
			r.copy(24, prev_trans_hash.x+8);
		} else {
			READ(r, prev_trans_hash);
			if(!lt0_high) {
				lt0_high = BitReader(prev_trans_hash.x).read(64);
				lt0_high -= lt0_high % lt_mul;
			}
			assert(lt0_high/lt_mul == BitReader(prev_trans_hash.x).read(64)/lt_mul);
			assert(checkLt(BitReader(prev_trans_hash.x).read(64)));
		}
		READ(r, lt, prev_trans_lt, now);
		auto &tmn = trans_map.emplace(*(uint32_t*)(prev_trans_hash.x+8), pair<Bits256, uint64_t>{}).first->second;
		memcpy(tmn.first.x, prev_trans_hash.x+8, 24);
		*(uint64_t*)(tmn.first.x+24) = lt;
		reverse(tmn.first.x+24, tmn.first.x+32);
		tmn.second = prev_trans_lt;
		outmsg_cnt = r.read(15);
		orig_status = r.read(2);
		end_status = r.read(2);
		*tlbs++ = TRANSACTION_TMP;
		total_fees.read<S>(r, tlbs);
		*tlbs++ = TLBtype2(HASH_UPDATE, ACCOUNT);
		*tlbs++ = TRANSACTION_DESCR;
	}
	template<int S>
	void write(BitWriter &w) {
		if(!S) w.write(4, tag);
		WRITE(w, account_addr);
		if(S) {
			if(!lt0_hight_w) {
				lt0_hight_w = true;
				w.write(24, lt0_high/lt_mul - seq_no_min);
			}
			w.write(8, BitReader(prev_trans_hash.x).read(64) - lt0_high);
			w.copy(24, prev_trans_hash.x+8);
		} else WRITE(w, prev_trans_hash);
		WRITE(w, lt, prev_trans_lt, now);
		w.write(15, outmsg_cnt);
		w.write(2, orig_status);
		w.write(2, end_status);
		total_fees.write<S>(w);
	}
};

struct MyDataCell : vm::DataCell {
	void serialize(BitWriter &writer, TLBtype2 tlb, TLBtype2* tlbs) const;
};

struct BOC {
	uint64_t data_size;
	td::HashMap<vm::Cell::Hash, int> cells;

	struct CellInfo {
		td::Ref<vm::DataCell> c;
		array<uint32_t, 4> refs;
		TLBtype2 tlb = UNKNOWN;
		uint32_t nc = 0;
		CellInfo(td::Ref<vm::DataCell> _dc, const array<uint32_t, 4>& _ref_list)
			: c(std::move(_dc)), refs(_ref_list) {}
	};
	vector<CellInfo> cell_list, cell_list_tmp;

	int import_cell(td::Ref<vm::Cell> cell) {
		auto it = cells.find(cell->get_hash());
		if(it != cells.end()) return it->second;
		td::Ref<vm::DataCell> dc = cell->load_cell().move_as_ok().data_cell;
		array<uint32_t, 4> refs;
		refs.fill(0xffffffffu);
		for(uint32_t i = 0; i < dc->size_refs(); i++)
			refs[i] = import_cell(dc->get_ref(i));
		cells.emplace(dc->get_hash(), cell_list.size());
		cell_list.emplace_back(dc, refs);
		return cell_list.size()-1;
	}

	void import_cells(td::Ref<vm::Cell> root) {
		cell_list.clear();
		cells.clear();
		import_cell(root);
		reverse(cell_list.begin(), cell_list.end());
		for(CellInfo& ci : cell_list) {
			for(uint8_t j = 0; j < ci.c->size_refs(); ++j)
				ci.refs[j] = cell_list.size() - 1 - ci.refs[j];
		}
	}

	BitWriter serialize_to_impl(uint32_t ref_size) {
		BitWriter writer;
		writer.write(8, ref_size);
		(cell_list[0].tlb = BLOCK).special = false;
		array<TLBtype2, 4> tlbs;
		for(int i = 0; i < (int) cell_list.size(); ++i) {
			CellInfo &ci = cell_list[i];
			tlbs.fill(UNKNOWN);
			reinterpret_cast<const MyDataCell*>(ci.c.get())->serialize(writer, ci.tlb, tlbs.data());
			for(uint32_t j = 0; j < ci.c->size_refs(); ++j) {
				writer.write(ref_size, ci.refs[j]);
				propagate(ci.tlb, cell_list[ci.refs[j]].tlb, tlbs[j]);
			}
		}
		// uint32_t ccc[UNKNOWN];
		// fill_n(ccc, UNKNOWN, 0);
		// for(int i = cell_list.size()-1; i >= 0; --i) {
		// 	cell_list[i].nc = cell_list[i].c->get_bits();
		// 	for(uint8_t j = 0; j < cell_list[i].c->size_refs(); ++j)
		// 		cell_list[i].nc += cell_list[cell_list[i].refs[j]].nc;
		// 	if(cell_list[i].tlb.type < UNKNOWN) {
		// 		ccc[cell_list[i].tlb.type] += cell_list[i].nc;
		// 		cell_list[i].nc = 0;
		// 	}
		// }
		// int nt = END_IMPLEMENTED+1;
		// for(int t = nt+1; t < UNKNOWN; ++t) if(ccc[t] > ccc[nt]) nt = t;
		// cerr << "next " << nt << ' ' << ccc[nt] << endl;
		return writer;
	}
};

constexpr bool smallest_ref = false;

template<typename T, bool dbg=false, typename... Args>
void trans01(BitReader &r, BitWriter &w, TLBtype2* &tlbs, Args... args) {
	T x(args...); x.template read<0>(r, tlbs);
	const auto p = w.pos();
	x.template write<1>(w);
	if constexpr (dbg) cerr << w.pos()-p << endl;
}
template<typename T, bool dbg=false, typename... Args>
void trans10(BitReader &r, BitWriter &w, TLBtype2* &tlbs, Args... args) {
	T x(args...);
	const auto pr = r.pos();
	x.template read<1>(r, tlbs);
	if constexpr (dbg) cerr << r.pos()-pr << endl;
	w.write(8, 0);
	const auto pw = w.pos();
	assert(!pw.j);
	x.template write<0>(w);
	int bits = w.pos()-pw;
	uint8_t d2 = 2*(bits/8);
	bits &= 7;
	if(bits) {
		const int r = 8-bits;
		w.write(r, 1<<(r-1));
		++d2;
	}
	w.data[pw.i-1] = d2;
}

template<typename T, typename... Args>
void checkRW0(const MyDataCell &c, Args... args) {
	array<TLBtype2, 4> tlbs;
	TLBtype2 *tlbs_it = tlbs.data();
	BitReader r(c.get_data());
	BitWriter w;
	T x(args...); x.template read<0>(r, tlbs_it); x.template write<0>(w);
	cerr << c.get_bits() << ' ' << 8*w.i+w.j << endl;
	assert((int) c.get_bits() == 8*w.i+w.j);
	if(w.j) w.data[w.i] |= 1<<(8-w.j-1);
	int i = 0; while(i < (int) w.data.size() && c.get_data()[i] == w.data[i]) ++i;
	cerr << i << " / " << w.data.size() << endl;
	if(i < (int) w.data.size()) {
		((cerr << args << ' '), ...);
		cerr << endl << hex;
		for(int j = 0; j < i; ++j) cerr << (uint32_t) c.get_data()[j];
		cerr << endl << (uint32_t) c.get_data()[i] << " ==> " << (uint32_t) w.data[i] << endl;
	}
	assert(i == (int) w.data.size());
	assert(c.size_refs() == tlbs_it-tlbs.data());
}

array<uint64_t, 4> libs[] {
	{0x0674fd4d7a2d458full,0xed5972176523686bull,0xfdb576be35447305ull,0x683d7c2bafd8d54bull},
	{0x259b7e94c81829baull,0x54773583b8c19aafull,0xd6a307f812583e17ull,0x9553590947a142e6ull},
	{0xb46b82ab89ac74e8ull,0xdf958bdb7b5e3e40ull,0x66a37f18e7d29836ull,0x13a69e32ce986e24ull},
	{0xf716b52ad22e5ac9ull,0xe77845dc98489d7full,0x32688f8e44a2182full,0x79bc01f54b0b4b33ull},
	{0x5da14c62cd8e33a9ull,0xdc7d67bfd9a8e437ull,0xfbf2058fe9f0849bull,0xb481a232e3afc784ull},
	{0x83d26ba4d8d31a0full,0x72fb959163de1d32ull,0xec7f72b1319b2e60ull,0xf46d9610dc2e5ec2ull},
	{0xb702e2c80dbbbe12ull,0x6be147251e726fe2ull,0xd157f634c9aeebb9ull,0x78c8be626de7229full},
	{0x52124df9a91d8d84ull,0x35a270a690526819ull,0x043cfe1d67ffa373ull,0xccf5a4c63288aab7ull},
	{0x444e080d443608c0ull,0x215aac32613194fbull,0xb509ee29f4f47e41ull,0xe8c334b378560b56ull},
	{0x01cfa9c2f3e688cdull,0x920dec37283a00bbull,0xd9ffbf1ded8596c1ull,0xd9140adfcf5d544aull},
	{0xf958639d06326172ull,0x52609e58ec894d75ull,0xdb04110f1d9c444eull,0x90b28b6f2097d4e1ull},
	{0xd407b8cc961f2f38ull,0xf932115245256139ull,0x6dbf563a617485c3ull,0x8a64885eaafce899ull},
	{0xb9a71aa93656cabeull,0x0e04f989cbdec6e4ull,0xc2556dc371c01ba6ull,0xf688b29ad763f8c1ull}
};
void MyDataCell::serialize(BitWriter &writer, TLBtype2 tlb, TLBtype2 *tlbs) const {
	const auto pruned_branch = [&]()->void {
		assert(get_level() == 1);
		writer.write(8, (1<<3) | (get_depth(0)>>8));
		writer.copy(1, get_data()+2+32+1);
		writer.copy(32, get_data()+2);
	};
	if(tlb.type < END_COULD_BE_PRUNED && tlb.special) {
		if(is_special()) {
			assert(get_data()[0] == 1);
			return pruned_branch();
		}
		if(do_align) writer.write(8, 0);
		else writer.write(1, 1);
	}
	BitReader r(get_data());
	switch(tlb.type) {
	#define SER_STRUCT(STRUCT, ...) assert(!is_special()); trans01<STRUCT>(r, writer, tlbs, ##__VA_ARGS__); break
	case BLOCK: SER_STRUCT(Block);
	case BLOCK_INFO: SER_STRUCT(BlockInfo);
	case VALUE_FLOW: SER_STRUCT(ValueFlow);
	case CURRENCY_COLLECTION_4: SER_STRUCT(CurrencyCollection4);
	case MERKLE_UPDATE: trans01<MerkleUpdate>(r, writer, tlbs, (TLBtype)tlb.var1); break;
	case SHARD_STATE: SER_STRUCT(ShardState);
	case SHARD_STATE_UNSPLIT: SER_STRUCT(ShardStateUnsplit);
	case BLOCK_EXTRA: SER_STRUCT(BlockExtra);
	case SHARD_ACCOUNTS: SER_STRUCT(ShardAccounts);
	case OUT_MSG_DESCR: SER_STRUCT(OutMsgDescr);
	case IN_MSG_DESCR: SER_STRUCT(InMsgDescr);
	case TRANSACTION: SER_STRUCT(Transaction);
	case HASHMAP:
		if(tlb.var2 == VAR_UINT_32) { SER_STRUCT(Hashmap<VarUInteger<32>>, tlb.var1); }
		else assert(false);
	case HASHMAP_AUG:
		if(tlb.var2 == SHARD_ACCOUNT && tlb.var3 == DEPTH_BALANCE_INFO) { SER_STRUCT(ShardAccounts2, tlb.var1); }
		else if(tlb.var2 == OUT_MSG && tlb.var3 == CURRENCY_COLLECTION) { SER_STRUCT(OutMsgDescr2, tlb.var1); }
		else if(tlb.var2 == IN_MSG && tlb.var3 == IMPORT_FEES) { SER_STRUCT(InMsgDescr2, tlb.var1); }
		else assert(false);
	default: {
		if(is_special()) {
			uint8_t type = get_data()[0];
			switch(type) {
			case 1: {
				return pruned_branch();
			} case 2: {
				int i = 0;
				while(i < (int)std::size(libs) && strncmp((char*)libs[i].data(), (char*)get_data()+1, 32)) ++i;
				if(i < (int) std::size(libs)) {
					writer.write(8, 5<<3);
					writer.write(8, i);
					break;
				}
				writer.write(8, type<<3);
				writer.copy(get_bits()/8 - 1, get_data()+1);
				break;
			} default:
				writer.write(8, type<<3);
			}
		} else {
			writer.write(8, size_refs());
			writer.write(8, info_.d2());
			writer.copy((get_bits() + 7) / 8, get_data());
		}
	}}
	if(do_align && writer.j) writer.write(8-writer.j, 0);
}

td::Ref<vm::Cell> deserialise(const td::Slice &data0) {
	BitWriter writer;
	BitReader reader((const uint8_t*)data0.data());
	uint8_t tmp_buf[128];
	const auto copy = [&](int bytes) {
		reader.copy(bytes, tmp_buf);
		writer.copy(bytes, tmp_buf);
	};

	const uint32_t ref_size = reader.read(8);
	do_align = (ref_size%8) == 0;

	writer.write(32, BOC_HEAD);
	writer.write(8, (1<<6) | 2); // ref_byte_size
	writer.write(8, 3); // offset_byte_size
	const auto cell_count_pos = writer.pos();
	writer.write(REF_SIZE*8, 0); // cell_count
	writer.write(REF_SIZE*8, 1); // root_count
	writer.write(REF_SIZE*8, 0);
	const auto data_size_pos = writer.pos();
	writer.write(OFFSET_SIZE*8, 0); // data_size
	writer.write(REF_SIZE*8, 0); // root_index

	// cells
	struct CellInfo {
		td::Ref<vm::DataCell> c;
		TLBtype2 tlb = UNKNOWN;
		BitWriter::Pos start;
		array<uint32_t, 4> refs;
		uint8_t ref_size = 0;
		uint8_t special = 0;
	};
	vector<CellInfo> cells(1);
	const auto data_pos = writer.pos();
	(cells[0].tlb = BLOCK).special = false;
	array<TLBtype2, 4> tlbs;
	const auto pruned_branch = [&](CellInfo &ci, uint32_t d1) {
		writer.write(8, 2*(2+32+2));
		writer.write(8, ci.special = 1);
		writer.write(8, 1u); // layer mask
		const uint16_t depth0 = ((d1&7)<<8) | reader.read(8);
		copy(32);
		writer.write(16, depth0);
		ci.ref_size = 0;
	};
	for(uint32_t i = 0; i < cells.size(); ++i) {
		CellInfo &ci = cells[i];
		ci.start = writer.pos();
		assert(!ci.start.j);
		writer.write(8, 0); // cell_header
		uint32_t d1, d2;
		tlbs.fill(UNKNOWN);
		TLBtype2 *tlbs_it = tlbs.data();
		if(ci.tlb.type < END_COULD_BE_PRUNED && ci.tlb.special) {
			if(do_align) {
				if((d1 = reader.read(8))) {
					pruned_branch(ci, d1);
					continue;
				}
			} else if(!reader.read(1)) {
				pruned_branch(ci, reader.read(7));
				continue;
			}
		}
		switch(ci.tlb.type) {
		case BLOCK: trans10<Block>(reader, writer, tlbs_it); break;
		case BLOCK_INFO: trans10<BlockInfo>(reader, writer, tlbs_it); break;
		case VALUE_FLOW: trans10<ValueFlow>(reader, writer, tlbs_it); break;
		case CURRENCY_COLLECTION_4: trans10<CurrencyCollection4>(reader, writer, tlbs_it); break;
		case MERKLE_UPDATE: trans10<MerkleUpdate>(reader, writer, tlbs_it, (TLBtype)ci.tlb.var1); ci.special = 4; break;
		case SHARD_STATE: trans10<ShardState>(reader, writer, tlbs_it); break;
		case SHARD_STATE_UNSPLIT: trans10<ShardStateUnsplit>(reader, writer, tlbs_it); break;
		case BLOCK_EXTRA: trans10<BlockExtra>(reader, writer, tlbs_it); break;
		case SHARD_ACCOUNTS: trans10<ShardAccounts>(reader, writer, tlbs_it); break;
		case OUT_MSG_DESCR: trans10<OutMsgDescr>(reader, writer, tlbs_it); break;
		case IN_MSG_DESCR: trans10<InMsgDescr>(reader, writer, tlbs_it); break;
		case TRANSACTION: trans10<Transaction>(reader, writer, tlbs_it); break;
		case HASHMAP:
			if(ci.tlb.var2 == VAR_UINT_32) trans10<Hashmap<VarUInteger<32>>>(reader, writer, tlbs_it, ci.tlb.var1);
			else assert(false);
			break;
		case HASHMAP_AUG:
			if(ci.tlb.var2 == SHARD_ACCOUNT && ci.tlb.var3 == DEPTH_BALANCE_INFO) trans10<ShardAccounts2>(reader, writer, tlbs_it, ci.tlb.var1);
			else if(ci.tlb.var2 == OUT_MSG && ci.tlb.var3 == CURRENCY_COLLECTION) trans10<OutMsgDescr2>(reader, writer, tlbs_it, ci.tlb.var1);
			else if(ci.tlb.var2 == IN_MSG && ci.tlb.var3 == IMPORT_FEES) trans10<InMsgDescr2>(reader, writer, tlbs_it, ci.tlb.var1);
			else assert(false);
			break;
		default:
			d1 = reader.read(8);
			ci.ref_size = d1 & 7;
			ci.special = (d1>>3) & 7;
			assert(ci.special <= 5);
			switch(ci.special) {
			case 0:
				writer.write(8, d2 = reader.read(8));
				copy((d2+1)/2);
				break;
			case 1:
				pruned_branch(ci, d1);
				break;
			case 2:
				writer.write(8, 2*(1 + 32));
				writer.write(8, ci.special);
				copy(32);
				break;
			case 5: {
				writer.write(8, 2*(1 + 32));
				writer.write(8, ci.special = 2);
				writer.copy(32, (uint8_t*) libs[reader.read(8)].data());
				break;
			} default: {
				ci.ref_size = ci.special-2;
				writer.write(8, 2*(1+ci.ref_size*(32+2)));
				writer.write(8, ci.special);
				writer.skip(ci.ref_size*(32+2));
			}};
		}
		assert(!writer.j);
		if(do_align && reader.tmp_s) reader.read(8-reader.tmp_s);
		ci.ref_size += tlbs_it - tlbs.data();
		for(uint32_t j = 0; j < cells[i].ref_size; ++j) {
			cells[i].refs[j] = reader.read(ref_size);
			writer.write(REF_SIZE*8, cells[i].refs[j]);
			if(cells[i].refs[j] >= cells.size()) cells.resize(cells[i].refs[j]+1);
			propagate(cells[i].tlb, cells[cells[i].refs[j]].tlb, tlbs[j]);
		}
	}
	const auto data_end = writer.pos();
	assert(!data_end.j);

	for(int i = cells.size()-1; i >= 0; --i) {
		CellInfo &ci = cells[i];

		if(ci.special == 3) {
			td::Ref<vm::Cell> ref = cells[ci.refs[0]].c;
			const uint8_t hl = 0; // TODO: maybe wrong
			writer.setPos(ci.start);
			writer.skip(2+1);
			writer.copy(32, ref->get_hash(hl).as_array().data());
			writer.write(16, ref->get_depth(hl));
		} else if(ci.special == 4) {
			td::Ref<vm::Cell> ref1 = cells[ci.refs[0]].c;
			td::Ref<vm::Cell> ref2 = cells[ci.refs[1]].c;
			const uint8_t hl = 0; // TODO: maybe wrong
			writer.setPos(ci.start);
			writer.skip(2+1);
			writer.copy(32, ref1->get_hash(hl).as_array().data());
			writer.copy(32, ref2->get_hash(hl).as_array().data());
			writer.write(16, ref1->get_depth(hl));
			writer.write(16, ref2->get_depth(hl));
		}

		// build cell
		vm::CellBuilder cb;
		uint32_t d2 = writer.data[ci.start.i+1];
		uint32_t bits = 8 * ((d2+1)/2);
		if(d2&1) {
			uint32_t m = 0;
			const uint8_t last = writer.data[ci.start.i + 2 + (bits/8) - 1];
			while(!((last>>m)&1)) ++m;
			bits -= m+1;
		}
		cb.store_bits(writer.data.data() + ci.start.i + 2, bits);
		for(uint32_t j = 0; j < ci.ref_size; ++j)
			cb.store_ref(cells[ci.refs[j]].c);
		ci.c = cb.finalize(ci.special);

		// store first byte
		writer.setPos(ci.start);
		writer.write(8, ci.c->size_refs() | (ci.c->is_special() ? 8 : 0) | (ci.c->get_level()<<5));
	}
	writer.setPos(cell_count_pos);
	writer.write(REF_SIZE*8, cells.size());
	writer.setPos(data_size_pos);
	writer.write(OFFSET_SIZE*8, data_end.i - data_pos.i);

	const uint32_t crc = td::crc32c(td::Slice(writer.data.data(), data_end.i));
	writer.setPos(data_end);
	writer.write(32, td::bswap32(crc));

	return vm::std_boc_deserialize(td::Slice(writer.data.data(), writer.data.size())).move_as_ok();
}

struct CRangeEnc {
  uint32_t range;
  uint32_t cache;
  uint64_t low;
  uint64_t cacheSize;
  uint8_t *buf;
  uint8_t *bufLim;
  uint8_t *bufBase;
  uint8_t *data;
  uint64_t processed;
};

struct CLenEnc {
	uint16_t low[LZMA_NUM_PB_STATES_MAX << (kLenNumLowBits + 1)];
	uint16_t high[kLenNumHighSymbols];
};

using CLenPriceEnc = uint32_t[LZMA_NUM_PB_STATES_MAX][tabSize];

struct COptimal {
	uint32_t price;
	uint16_t state;
	uint16_t extra;
	uint32_t len;
	uint32_t dist;
	uint32_t reps[LZMA_NUM_REPS];
};

struct CMatchFinder {
	const uint8_t *buffer;
	uint32_t pos;
	uint32_t posLimit;
	uint32_t streamPos;
	uint32_t lenLimit;
	uint32_t cyclicBufferPos;
	uint32_t *hash;
	uint32_t *son;
	uint8_t *bufBase;
};

struct CLzmaEnc {
	uint32_t optCur;
	uint32_t optEnd;
	uint32_t longestMatchLen;
	uint32_t numPairs;
	uint32_t numAvail;
	uint32_t state;
	uint32_t additionalOffset;
	uint32_t reps[LZMA_NUM_REPS];
	uint16_t litProbs[0x300u];
	CRangeEnc rc;
	uint32_t backRes;
	int finished;
	uint64_t nowPos64;
	uint32_t matchPriceCount;
	int repLenEncCounter;
	int result;
	CMatchFinder matchFinderBase;
	uint32_t ProbPrices[kBitModelTotal >> kNumMoveReducingBits];
	uint32_t matches[fb * 2 + 2];
	uint32_t alignPrices[kAlignTableSize];
	uint32_t posSlotPrices[kNumLenToPosStates][kDistTableSizeMax];
	uint32_t distancesPrices[kNumLenToPosStates][kNumFullDistances];
	uint16_t posAlignEncoder[1 << kNumAlignBits];
	uint16_t isRep[kNumStates];
	uint16_t isRepG0[kNumStates];
	uint16_t isRepG1[kNumStates];
	uint16_t isRepG2[kNumStates];
	uint16_t isMatch[kNumStates][LZMA_NUM_PB_STATES_MAX];
	uint16_t isRep0Long[kNumStates][LZMA_NUM_PB_STATES_MAX];
	uint16_t posSlotEncoder[kNumLenToPosStates][1 << kNumPosSlotBits];
	uint16_t posEncoders[kNumFullDistances];
	CLenEnc lenProbs;
	CLenEnc repLenProbs;
	uint8_t g_FastPos[1 << kNumLogBits];
	CLenPriceEnc lenEnc;
	CLenPriceEnc repLenEnc;
	COptimal opt[kNumOpts];
};

uint32_t MatchFinder_GetNumAvailableBytes(void *p) {
	return (((CMatchFinder *)p)->streamPos - ((CMatchFinder *)p)->pos);
}

const uint8_t* MatchFinder_GetPointerToCurrentPos(void *p) {
  return ((CMatchFinder *)p)->buffer;
}

void MatchFinder_SetLimits(CMatchFinder *p) {
	uint32_t n = min(-max(p->pos, 1u) , cyclicBufferSize - p->cyclicBufferPos);
	uint32_t k = p->streamPos - p->pos, mm = fb;
	if(k > keepSizeAfter) k -= keepSizeAfter;
	else if (k >= mm) {
		k -= mm;
		k++;
	} else {
		mm = k;
		if(k) k = 1;
	}
	p->lenLimit = mm;
	p->posLimit = p->pos + min(n, k);
}

inline void _SASUB_32(const uint32_t subValue, uint32_t &i) {
	uint32_t v = i;
	if(v < subValue) v = subValue;
	i = v - subValue;
}

void LzFind_SaturSub_32(uint32_t subValue, uint32_t *items, const uint32_t *lim) {
  do {
	_SASUB_32(subValue, *(items++)); _SASUB_32(subValue, *(items++));
	_SASUB_32(subValue, *(items++)); _SASUB_32(subValue, *(items++));
	_SASUB_32(subValue, *(items++)); _SASUB_32(subValue, *(items++));
	_SASUB_32(subValue, *(items++)); _SASUB_32(subValue, *(items++));
  } while (items != lim);
}

void MatchFinder_Normalize3(uint32_t subValue, uint32_t *items, size_t numItems) {
  constexpr uint32_t LZFIND_NORM_ALIGN_BLOCK_SIZE = 1 << 7;
  for(; numItems  && ((uint32_t)(ptrdiff_t)items & (LZFIND_NORM_ALIGN_BLOCK_SIZE - 1)) ; numItems--) {
	_SASUB_32(subValue, *items);
	items++;
  }
  {
	const size_t k_Align_Mask = (LZFIND_NORM_ALIGN_BLOCK_SIZE / 4 - 1);
	uint32_t *lim = items + (numItems & ~(size_t)k_Align_Mask);
	numItems &= k_Align_Mask;
	if(items != lim) LzFind_SaturSub_32(subValue, items, lim);
	items = lim;
  }
  for(; numItems ; numItems--) {
	_SASUB_32(subValue, *items);
	items++;
  }
}

void MatchFinder_CheckLimits(CMatchFinder *p) {
	if(!p->pos && p->streamPos >= 2) {
		const uint32_t subValue = -dictSize - 1;
		p->pos -= subValue;
		p->streamPos -= subValue;
		MatchFinder_Normalize3(subValue, p->hash, hashMask + 1);
		MatchFinder_Normalize3(subValue, p->son, 2*cyclicBufferSize);
	}
	if(p->cyclicBufferPos == cyclicBufferSize) p->cyclicBufferPos = 0; 
	MatchFinder_SetLimits(p);
}

void MatchFinder_MovePos(CMatchFinder *p) {
	p->cyclicBufferPos++;
	p->buffer++;
	if(++p->pos == p->posLimit) MatchFinder_CheckLimits(p);
}

inline uint32_t* GetMatchesSpec1(uint32_t lenLimit, uint32_t curMatch, uint32_t pos, const uint8_t *cur, uint32_t *son,
	size_t _cyclicBufferPos, uint32_t cutValue, uint32_t *d, uint32_t maxLen) {
	uint32_t *ptr0 = son + (_cyclicBufferPos << 1) + 1;
	uint32_t *ptr1 = son + (_cyclicBufferPos << 1);
	uint32_t len0 = 0, len1 = 0;
	uint32_t cmCheck = pos - cyclicBufferSize;
	if(pos <= cyclicBufferSize) cmCheck = 0;
	if(cmCheck < curMatch)
	do {
		const uint32_t delta = pos - curMatch;
		uint32_t *pair = son + ((_cyclicBufferPos - delta + ((delta > _cyclicBufferPos) * cyclicBufferSize)) << 1);
		const uint8_t *pb = cur - delta;
		uint32_t len = (len0 < len1 ? len0 : len1);
		const uint32_t pair0 = pair[0];
		if(pb[len] == cur[len]) {
			if(++len != lenLimit && pb[len] == cur[len])
			while(++len != lenLimit)
				if(pb[len] != cur[len])
				break;
			if(maxLen < len) {
				maxLen = len;
				*d++ = len;
				*d++ = delta - 1;
				if(len == lenLimit) {
					*ptr1 = pair0;
					*ptr0 = pair[1];
					return d;
				}
			}
		}
		if (pb[len] < cur[len]) {
			*ptr1 = curMatch;
			curMatch = pair[1];
			ptr1 = pair + 1;
			len1 = len;
		} else {
			*ptr0 = curMatch;
			curMatch = pair[0];
			ptr0 = pair;
			len0 = len;
		}
  } while(--cutValue && cmCheck < curMatch);
  *ptr0 = *ptr1 = 0;
  return d;
}

uint32_t* Bt2_MatchFinder_GetMatches(void *_p, uint32_t *distances) {
	CMatchFinder *p = (CMatchFinder *)_p;
	if(p->lenLimit < 2) {
		MatchFinder_MovePos(p);
		return distances;
	}
	const uint8_t *cur = p->buffer;
	uint32_t hv = *(const uint16_t*)cur, curMatch = p->hash[hv];
	p->hash[hv] = p->pos;
	distances = GetMatchesSpec1(p->lenLimit, curMatch, p->pos, p->buffer, p->son, p->cyclicBufferPos, mc, distances, 1);
	MatchFinder_MovePos(p);
	return distances;
}

void SkipMatchesSpec(uint32_t lenLimit, uint32_t curMatch, uint32_t pos, const uint8_t *cur, uint32_t *son,
	size_t _cyclicBufferPos, uint32_t _cyclicBufferSize, uint32_t cutValue) {
	uint32_t *ptr0 = son + (_cyclicBufferPos << 1) + 1;
	uint32_t *ptr1 = son + (_cyclicBufferPos << 1);
	uint32_t len0 = 0, len1 = 0;
	uint32_t cmCheck = pos - _cyclicBufferSize;
	if(pos <= _cyclicBufferSize) cmCheck = 0;
	if(cmCheck < curMatch)
	do {
		const uint32_t delta = pos - curMatch;
		uint32_t *pair = son + ((size_t)(_cyclicBufferPos - delta + ((delta > _cyclicBufferPos) * _cyclicBufferSize)) << 1);
		const uint8_t *pb = cur - delta;
		uint32_t len = (len0 < len1 ? len0 : len1);
		if(pb[len] == cur[len]) {
			while(++len != lenLimit && pb[len] == cur[len]);
			if(len == lenLimit) {
				*ptr1 = pair[0];
				*ptr0 = pair[1];
				return;
			}
		}
		if(pb[len] < cur[len]) {
			*ptr1 = curMatch;
			curMatch = pair[1];
			ptr1 = pair + 1;
			len1 = len;
		} else {
			*ptr0 = curMatch;
			curMatch = pair[0];
			ptr0 = pair;
			len0 = len;
		}
	} while(--cutValue && cmCheck < curMatch); 
	*ptr0 = *ptr1 = 0;
	return;
}

void Bt2_MatchFinder_Skip(void *_p, uint32_t num) {
	CMatchFinder *p = (CMatchFinder *)_p;
	do {
		uint32_t hv; const uint8_t *cur; uint32_t curMatch;
		uint32_t lenLimit = p->lenLimit;
		if(lenLimit < 2) {
			MatchFinder_MovePos(p);
			continue;
		}
		cur = p->buffer;
		hv = *reinterpret_cast<const uint16_t*>(cur);
		curMatch = p->hash[hv];
		p->hash[hv] = p->pos;
		SkipMatchesSpec(lenLimit, curMatch, p->pos, p->buffer, p->son, p->cyclicBufferPos, cyclicBufferSize, mc);
		MatchFinder_MovePos(p);
	} while (--num);
}

void FillDistancesPrices(CLzmaEnc *p) {
  uint32_t tempPrices[kNumFullDistances], i, lps;
  const uint32_t *ProbPrices = p->ProbPrices;
  p->matchPriceCount = 0;
  for(i = kStartPosModelIndex / 2; i < kNumFullDistances / 2; i++) {
	uint32_t posSlot = p->g_FastPos[i];
	uint32_t footerBits = (posSlot >> 1) - 1;
	uint32_t base = ((2 | (posSlot & 1)) << footerBits);
	const uint16_t *probs = p->posEncoders + base * 2;
	uint32_t price = 0, m = 1, sym = i, offset = 1 << footerBits;
	base += i;
	if(footerBits)
		do {
			uint32_t bit = sym & 1;
			sym >>= 1;
			price += ProbPrices[((probs[m]) ^ (uint32_t)((-((int)(bit))) & (kBitModelTotal - 1))) >> kNumMoveReducingBits];
			m = (m << 1) + bit;
		} while(--footerBits);
	uint32_t prob = probs[m];
	tempPrices[base         ] = price + ProbPrices[(prob) >> kNumMoveReducingBits];
	tempPrices[base + offset] = price + ProbPrices[((prob) ^ (kBitModelTotal - 1)) >> kNumMoveReducingBits];
  }
  for(lps = 0; lps < kNumLenToPosStates; lps++) {
	uint32_t slot;
	uint32_t *posSlotPrices = p->posSlotPrices[lps];
	const uint16_t *probs = p->posSlotEncoder[lps];
	for(slot = 0; slot < distTableSize; slot++) {
		uint32_t price = 0, sym = slot + (1 << (kNumPosSlotBits - 1));
		for(int i = 0; i < 5; ++i) {
			int bit = sym & 1;
			sym >>= 1;
			price += ProbPrices[(probs[sym] ^ (uint32_t)((-bit) & (kBitModelTotal - 1))) >> kNumMoveReducingBits];
		}
		uint32_t prob = probs[slot + (1 << (kNumPosSlotBits - 1))];
		posSlotPrices[slot * 2    ] = price + ProbPrices[(prob) >> kNumMoveReducingBits];
		posSlotPrices[slot * 2 + 1] = price + ProbPrices[((prob) ^ (kBitModelTotal - 1)) >> kNumMoveReducingBits];
	}
	uint32_t delta = (kEndPosModelIndex / 2 - 1 - kNumAlignBits) << kNumBitPriceShiftBits;
	for(slot = kEndPosModelIndex / 2; slot < distTableSize; slot++) {
		posSlotPrices[(size_t)slot * 2    ] += delta;
		posSlotPrices[(size_t)slot * 2 + 1] += delta;
		delta += 1 << kNumBitPriceShiftBits;
	}
	uint32_t *dp = p->distancesPrices[lps];
	copy_n(posSlotPrices, 4, dp);
	for(i = 4; i < kNumFullDistances; i += 2) {
		uint32_t slotPrice = posSlotPrices[p->g_FastPos[i]];
		dp[i    ] = slotPrice + tempPrices[i];
		dp[i + 1] = slotPrice + tempPrices[i + 1];
	}
  }
}

void FillAlignPrices(CLzmaEnc *p) {
  for(uint32_t i = 0; i < kAlignTableSize / 2; i++) {
	uint32_t price = 0, sym = i, m = 1;
	for(int j = 0; j < 3; ++j) {
		uint32_t bit = sym & 1;
		sym >>= 1;
		price += p->ProbPrices[((p->posAlignEncoder[m]) ^ (uint32_t)((-((int)(bit))) & (kBitModelTotal - 1))) >> kNumMoveReducingBits];
		m = (m << 1) + bit;
	}
	uint32_t prob = p->posAlignEncoder[m];
	p->alignPrices[i    ] = price + p->ProbPrices[(prob) >> kNumMoveReducingBits];
	p->alignPrices[i + 8] = price + p->ProbPrices[((prob) ^ (kBitModelTotal - 1)) >> kNumMoveReducingBits];
  }
}

inline uint32_t _GET_PRICEa(const uint32_t *ProbPrices, uint32_t prob, uint32_t bit) {
	return ProbPrices[(prob ^ (uint32_t)((-((int)bit)) & (kBitModelTotal - 1))) >> kNumMoveReducingBits];
}
inline uint32_t _GET_PRICEa_0(const uint32_t *ProbPrices, uint32_t prob) {
	return ProbPrices[prob >> kNumMoveReducingBits];
}
inline uint32_t _GET_PRICEa_1(const uint32_t *ProbPrices, uint32_t prob) {
	return ProbPrices[(prob ^ (kBitModelTotal - 1)) >> kNumMoveReducingBits];
}

void SetPrices_3(const uint16_t *probs, uint32_t startPrice, uint32_t *prices, const uint32_t *ProbPrices) {
  for(uint32_t i = 0; i < 8; i += 2) {
	uint32_t price = startPrice;
	price += _GET_PRICEa(ProbPrices, probs[1           ], (i >> 2));
	price += _GET_PRICEa(ProbPrices, probs[2 + (i >> 2)], (i >> 1) & 1);
	uint32_t prob = probs[4 + (i >> 1)];
	prices[i    ] = price + _GET_PRICEa_0(ProbPrices, prob);
	prices[i + 1] = price + _GET_PRICEa_1(ProbPrices, prob);
  }
}

void LenPriceEnc_UpdateTables(CLenPriceEnc p, const CLenEnc *enc, const uint32_t *ProbPrices) {
	uint32_t a, b, c;
	b = _GET_PRICEa_1(ProbPrices, enc->low[0]);
	a = _GET_PRICEa_0(ProbPrices, enc->low[0]);
	c = b + _GET_PRICEa_0(ProbPrices, enc->low[kLenNumLowSymbols]);
	SetPrices_3(enc->low, a, p[0], ProbPrices);
	SetPrices_3(enc->low + kLenNumLowSymbols, c, p[0] + kLenNumLowSymbols, ProbPrices);
	uint32_t *prices = p[0] + kLenNumLowSymbols * 2;
	uint32_t i = (tabSize - kLenNumLowSymbols * 2 + 1) / 2; 
	b += _GET_PRICEa_1(ProbPrices, enc->low[kLenNumLowSymbols]);
	do {
		uint32_t sym = --i + (1 << (kLenNumHighBits - 1)), price = b;
		do {
			const uint32_t bit = sym&1;
			sym >>= 1;
			price += _GET_PRICEa(ProbPrices, enc->high[sym], bit);
		} while(sym >= 2);
		const uint32_t prob = enc->high[i + (1 << (kLenNumHighBits - 1))];
		prices[i * 2    ] = price + _GET_PRICEa_0(ProbPrices, prob);
		prices[i * 2 + 1] = price + _GET_PRICEa_1(ProbPrices, prob);
	} while(i);
}

int CheckErrors(CLzmaEnc *p) {
	if(p->result) return p->result;
	if(p->result) p->finished = 1;
	return p->result;
}

void RangeEnc_FlushStream(CRangeEnc *p) {
	const size_t num = p->buf - p->bufBase;
	memcpy(p->data, p->bufBase, num);
	p->data += num;
	p->processed += num;
	p->buf = p->bufBase;
}

void RangeEnc_ShiftLow(CRangeEnc *p) {
	const uint32_t low = p->low;
	uint32_t high = p->low >> 32;
	p->low = low << 8;
	if(low < 0xFF000000u || high) {
		*p->buf++ = p->cache + high;
		p->cache = low >> 24;
		if(p->buf == p->bufLim) RangeEnc_FlushStream(p);
		if(!p->cacheSize) return;
		high += 0xFF;
		for(;;) {
			*p->buf++ = high;
			if(p->buf == p->bufLim) RangeEnc_FlushStream(p);
			if(--p->cacheSize == 0) return;
		}
	}
	++p->cacheSize;
}

inline void RC_BIT_PRE(uint32_t &ttt, uint32_t &newBound, uint32_t range, const uint16_t* prob) {
	ttt = *(prob);
	newBound = (range >> kNumBitModelTotalBits) * ttt;
}
inline void RC_BIT_0_BASE(uint32_t ttt, uint32_t newBound, uint32_t &range, CRangeEnc* p, uint16_t* prob) {
	range = newBound;
	*(prob) = (uint16_t)(ttt + ((kBitModelTotal - ttt) >> kNumMoveBits));
}
inline void RC_BIT_1_BASE(uint32_t ttt, uint32_t newBound, uint32_t &range, CRangeEnc* p, uint16_t* prob) {
	range -= newBound; 
	p->low += newBound;
	*prob = ttt - (ttt >> kNumMoveBits);
}
inline void RC_NORM(uint32_t &range, CRangeEnc* p) {
	if(range < kTopValue) {
		range <<= 8;
		RangeEnc_ShiftLow(p);
	}
}
inline void RC_BIT_0(uint32_t ttt, uint32_t newBound, uint32_t &range, CRangeEnc* p, uint16_t* prob) {
	RC_BIT_0_BASE(ttt, newBound, range, p, prob);
	RC_NORM(range, p);
}
inline void RC_BIT_1(uint32_t ttt, uint32_t newBound, uint32_t &range, CRangeEnc* p, uint16_t* prob) {
	RC_BIT_1_BASE(ttt, newBound, range, p, prob);
	RC_NORM(range, p);
}

inline void RC_BIT(uint32_t &ttt, uint32_t &newBound, uint32_t &range, CRangeEnc* p, uint16_t* prob, uint32_t bit) {
	RC_BIT_PRE(ttt, newBound, range, prob);
	uint32_t mask = 0 - bit;
	range &= mask;
	mask &= newBound;
	range -= mask;
	p->low += mask;
	mask = bit - 1;
	range += newBound & mask;
	mask &= kBitModelTotal - ((1 << kNumMoveBits) - 1);
	mask += (1 << kNumMoveBits) - 1;
	ttt += (int)(mask - ttt) >> kNumMoveBits;
	*(prob) = ttt;
	RC_NORM(range, p);
}

int Flush(CLzmaEnc *p, uint32_t nowPos) {
  p->finished = 1;
  for(int i = 0; i < 5; i++) RangeEnc_ShiftLow(&p->rc);
  RangeEnc_FlushStream(&p->rc);
  return CheckErrors(p);
}

uint32_t ReadMatchDistances(CLzmaEnc *p, uint32_t *numPairsRes) {
	p->additionalOffset++;
	p->numAvail = MatchFinder_GetNumAvailableBytes(&p->matchFinderBase);
	uint32_t numPairs = Bt2_MatchFinder_GetMatches(&p->matchFinderBase, p->matches) - p->matches;
	*numPairsRes = numPairs;  
	if(!numPairs) return 0;
	const uint32_t len = p->matches[numPairs - 2];
	if(len != fb) return len;
	uint32_t numAvail = min(p->numAvail, fb);
	const uint8_t *p1 = MatchFinder_GetPointerToCurrentPos(&p->matchFinderBase) - 1;
	const uint8_t *p2 = p1 + len;
	const ptrdiff_t dif = -1 - (ptrdiff_t)p->matches[numPairs - 1];
	const uint8_t *lim = p1 + numAvail;
	while(p2 != lim && *p2 == p2[dif]) ++p2;
	return p2 - p1;
}

void RangeEnc_EncodeBit_0(CRangeEnc *p, uint16_t *prob) {
	uint32_t range, ttt, newBound;
	range = p->range;
	RC_BIT_PRE(ttt, newBound, range, prob);
	RC_BIT_0(ttt, newBound, range, p, prob);
	p->range = range;
}

void LitEnc_Encode(CRangeEnc *p, uint16_t *probs, uint32_t sym) {
	sym |= 0x100;
	do {
		uint32_t ttt, newBound;
		RC_BIT(ttt, newBound, p->range, p, probs + (sym >> 8), (sym >> 7)&1);
		sym <<= 1;
	} while(sym < 0x10000);
}

inline void _MOVE_POS(CLzmaEnc *p, uint32_t num) {
	p->additionalOffset += num;
	Bt2_MatchFinder_Skip(&p->matchFinderBase, num);
}

uint32_t LitEnc_Matched_GetPrice(const uint16_t *probs, uint32_t sym, uint32_t matchByte, const uint32_t *ProbPrices) {
  uint32_t price = 0;
  uint32_t offs = 0x100;
  sym |= 0x100;
  do {
	matchByte <<= 1;
	price += _GET_PRICEa(ProbPrices, probs[offs + (matchByte & offs) + (sym >> 8)], (sym >> 7) & 1);
	sym <<= 1;
	offs &= ~(matchByte ^ sym);
  } while (sym < 0x10000);
  return price;
}

uint32_t LitEnc_GetPrice(const uint16_t *probs, uint32_t sym, const uint32_t *ProbPrices) {
  uint32_t price = 0;
  sym |= 0x100;
  do {
	uint32_t bit = sym & 1;
	sym >>= 1;
	price += _GET_PRICEa(ProbPrices, probs[sym], bit);
  } while (sym >= 2);
  return price;
}

uint32_t GetPrice_PureRep(const CLzmaEnc *p, uint32_t repIndex, size_t state) {
  uint32_t price;
  uint32_t prob = p->isRepG0[state];
  if(!repIndex) {
	price = _GET_PRICEa_0(p->ProbPrices, prob);
	price += _GET_PRICEa_1(p->ProbPrices, p->isRep0Long[state][0]);
  } else {
	price = _GET_PRICEa_1(p->ProbPrices, prob);
	prob = p->isRepG1[state];
	if(repIndex == 1) price += _GET_PRICEa_0(p->ProbPrices, prob);
	else {
	  price += _GET_PRICEa_1(p->ProbPrices, prob);
	  price += _GET_PRICEa(p->ProbPrices, p->isRepG2[state], repIndex - 2);
	}
  }
  return price;
}

uint32_t Backward(CLzmaEnc *p, uint32_t cur) {
	uint32_t wr = cur + 1;
	p->optEnd = wr;
	for(;;) {
	uint32_t dist = p->opt[cur].dist,
		len = p->opt[cur].len,
		extra = p->opt[cur].extra;
	cur -= len;
	if(extra) {
		--wr;
		p->opt[wr].len = len;
		cur -= extra;
		len = extra;
		if(extra == 1) {
			p->opt[wr].dist = dist;
			dist = uint32_t(-1);
		} else {
			p->opt[wr].dist = 0;
			len--;
			wr--;
			p->opt[wr].dist = uint32_t(-1);
			p->opt[wr].len = 1;
		}
	}
	if(!cur) {
		p->backRes = dist;
		p->optCur = wr;
		return len;
	}
	--wr;
	p->opt[wr].dist = dist;
	p->opt[wr].len = len;
  }
}
inline void _MakeAs_Lit(COptimal* p) {
	p->dist = -1;
	p->extra = 0;
}
inline void _MakeAs_ShortRep(COptimal* p) {
	p->dist = 0;
	p->extra = 0;
}

inline uint32_t _GetPrice_ShortRep(CLzmaEnc *p, uint32_t state, uint32_t posState) {
	return _GET_PRICEa_0(p->ProbPrices, p->isRepG0[state]) + _GET_PRICEa_0(p->ProbPrices, p->isRep0Long[state][posState]);
}
inline uint32_t _GetPrice_Rep_0(CLzmaEnc *p, uint32_t state) {
	return _GET_PRICEa_1(p->ProbPrices, p->isMatch[state][0])
		+ _GET_PRICEa_1(p->ProbPrices, p->isRep0Long[state][0])
		+ _GET_PRICEa_1(p->ProbPrices, p->isRep[state])
		+ _GET_PRICEa_0(p->ProbPrices, p->isRepG0[state]);
}
inline uint32_t _GET_PRICE_LEN(CLenPriceEnc p, uint32_t len) {
	return p[0][len - LZMA_MATCH_LEN_MIN];
}

inline uint32_t _GetLenToPosState(uint32_t len) {
	return (len < kNumLenToPosStates + 1) ? len - 2 : kNumLenToPosStates - 1;
}
inline uint32_t _GetLenToPosState2(uint32_t len) {
	return (len < kNumLenToPosStates - 1) ? len : kNumLenToPosStates - 1;
} 

inline void _BSR2_RET(CLzmaEnc *p, uint32_t pos, uint32_t &res) {
	uint32_t zz = (pos < (1 << (kNumLogBits + 6))) ? 6 : 6 + kNumLogBits - 1;
	res = p->g_FastPos[pos >> zz] + (zz * 2);
}

uint32_t GetOptimum(CLzmaEnc *p) {
	uint32_t last, cur;
	uint32_t reps[LZMA_NUM_REPS];
	uint32_t repLens[LZMA_NUM_REPS];
	uint32_t *matches;
	uint32_t numPairs, mainLen, repMaxIndex, i;
	uint32_t matchPrice, repMatchPrice;
	uint8_t curByte, matchByte;
	p->optCur = p->optEnd = 0;
	if(!p->additionalOffset) mainLen = ReadMatchDistances(p, &numPairs);
	else {
		mainLen = p->longestMatchLen;
		numPairs = p->numPairs;
	}
	uint32_t numAvail = min(p->numAvail, fb);
	if(numAvail < 2) {
	  p->backRes = uint32_t(-1);
	  return 1;
	}
	const uint8_t *data = MatchFinder_GetPointerToCurrentPos(&p->matchFinderBase) - 1;
	repMaxIndex = 0;
	for(i = 0; i < LZMA_NUM_REPS; i++) {
		const uint8_t *data2;
		reps[i] = p->reps[i];
		data2 = data - reps[i];
		if(data[0] != data2[0] || data[1] != data2[1]) {
			repLens[i] = 0;
			continue;
		}
		uint32_t len = 2;
		while(len < numAvail && data[len] == data2[len]) ++len;
		repLens[i] = len;
		if(len > repLens[repMaxIndex]) repMaxIndex = i;
		if(len == fb) break;
	}
	if(repLens[repMaxIndex] >= fb) {
		p->backRes = repMaxIndex;
		uint32_t len = repLens[repMaxIndex];
		_MOVE_POS(p, len - 1);
		return len;
	}
	matches = p->matches;
	if(mainLen >= fb) {
		p->backRes = matches[numPairs - 1] + LZMA_NUM_REPS;
		_MOVE_POS(p, mainLen - 1);
		return mainLen;
	}
	curByte = *data;
	matchByte = *(data - reps[0]);
	last = max(repLens[repMaxIndex], mainLen);
	if(last < 2 && curByte != matchByte) {
		p->backRes = uint32_t(-1);
		return 1;
	}
	p->opt[0].state = p->state;
	p->opt[1].price = _GET_PRICEa_0(p->ProbPrices, p->isMatch[p->state][0]) +
		(p->state >= 7 ?
			LitEnc_Matched_GetPrice(p->litProbs, curByte, matchByte, p->ProbPrices) :
			LitEnc_GetPrice(p->litProbs, curByte, p->ProbPrices));
	_MakeAs_Lit(&p->opt[1]);
	matchPrice = _GET_PRICEa_1(p->ProbPrices, p->isMatch[p->state][0]);
	repMatchPrice = matchPrice + _GET_PRICEa_1(p->ProbPrices, p->isRep[p->state]);
	if(matchByte == curByte && !repLens[0]) {
	  uint32_t shortRepPrice = repMatchPrice + _GetPrice_ShortRep(p, p->state, 0);
	  if(shortRepPrice < p->opt[1].price) {
		p->opt[1].price = shortRepPrice;
		_MakeAs_ShortRep(&p->opt[1]);
	  }
	  if(last < 2) {
		p->backRes = p->opt[1].dist;
		return 1;
	  }
	}
	p->opt[1].len = 1;
	copy_n(reps, 4, p->opt[0].reps);
	for(i = 0; i < LZMA_NUM_REPS; i++) {
	  uint32_t repLen = repLens[i];
	  if(repLen < 2) continue;
	  uint32_t price = repMatchPrice + GetPrice_PureRep(p, i, p->state);
	  do {
		uint32_t price2 = price + _GET_PRICE_LEN(p->repLenEnc, repLen);
		COptimal *opt = &p->opt[repLen];
		if(price2 < opt->price) {
			opt->price = price2;
			opt->len = repLen;
			opt->dist = i;
			opt->extra = 0;
		}
	  } while(--repLen >= 2);
	}
	uint32_t len = repLens[0] + 1;
	if(len <= mainLen) {
		uint32_t offs = 0;
		uint32_t normalMatchPrice = matchPrice + _GET_PRICEa_0(p->ProbPrices, p->isRep[p->state]);
		if(len < 2) len = 2;
		else while(len > matches[offs]) offs += 2;
		for(; ; len++) {
		  uint32_t dist = matches[(size_t)offs + 1];
		  uint32_t price = normalMatchPrice + _GET_PRICE_LEN(p->lenEnc, len);
		  uint32_t lenToPosState = _GetLenToPosState(len);
		  if(dist < kNumFullDistances) price += p->distancesPrices[lenToPosState][dist & (kNumFullDistances - 1)];
		  else {
			uint32_t slot;
			_BSR2_RET(p, dist, slot);
			price += p->alignPrices[dist & kAlignMask];
			price += p->posSlotPrices[lenToPosState][slot];
		  }
		  COptimal *opt = &p->opt[len];
		  if(price < opt->price) {
			opt->price = price;
			opt->len = len;
			opt->dist = dist + LZMA_NUM_REPS;
			opt->extra = 0;
		  }
		  if(len == matches[offs] && (offs += 2) == numPairs) break;
		}
	}
	cur = 0;
	for(;;) {
	uint32_t numAvail, numAvailFull;
	uint32_t newLen, numPairs, prev, state, startLen;
	uint32_t litPrice, matchPrice, repMatchPrice;
	int nextIsLit;
	uint8_t curByte, matchByte;
	COptimal *curOpt, *nextOpt;
	if(++cur == last) break;
	if(cur >= kNumOpts - 64) {
		uint32_t best = min_element(p->opt+cur, p->opt+last+1, [](COptimal& a, COptimal &b) { return a.price < b.price; }) - p->opt;
		uint32_t delta = best - cur;
		if(delta) _MOVE_POS(p, delta);
		cur = best;
		break;
	}
	newLen = ReadMatchDistances(p, &numPairs);
	if(newLen >= fb) {
		p->numPairs = numPairs;
		p->longestMatchLen = newLen;
		break;
	}
	curOpt = &p->opt[cur];
	prev = cur - curOpt->len;
	if(curOpt->len == 1) {
		state = p->opt[prev].state;
		state = !curOpt->dist ? kShortRepNextStates[state] : kLiteralNextStates[state];
	} else {
		uint32_t dist = curOpt->dist;
		if(curOpt->extra) {
			prev -= curOpt->extra;
			state = (curOpt->extra == 1 && dist >= LZMA_NUM_REPS) ? kState_MatchAfterLit : kState_RepAfterLit;
		} else {
			state = p->opt[prev].state;
			state = dist < LZMA_NUM_REPS ? kRepNextStates[state] : kMatchNextStates[state];
		}
		const COptimal *prevOpt = &p->opt[prev];
		uint32_t b0 = prevOpt->reps[0];
		if(dist < LZMA_NUM_REPS) {
			if(!dist) {
				reps[0] = b0;
				copy_n(prevOpt->reps+1, 3, reps+1);
			} else {
				reps[1] = b0;
				b0 = prevOpt->reps[1];
				if(dist == 1) {
					reps[0] = b0;
					copy_n(prevOpt->reps+2, 2, reps+2);
				} else {
					reps[2] = b0;
					reps[0] = prevOpt->reps[dist];
					reps[3] = prevOpt->reps[dist ^ 1];
				}
			}
		} else {
			reps[0] = dist - LZMA_NUM_REPS + 1;
			reps[1] = b0;
			copy_n(prevOpt->reps+1, 2, reps+2);
		}
	}
	curOpt->state = state;
	copy_n(reps, 4, curOpt->reps);
	const uint8_t *data = MatchFinder_GetPointerToCurrentPos(&p->matchFinderBase) - 1;
	curByte = *data;
	matchByte = *(data - reps[0]);
	uint32_t curPrice = curOpt->price, prob = p->isMatch[state][0];
	matchPrice = curPrice + _GET_PRICEa_1(p->ProbPrices, prob);
	litPrice = curPrice + _GET_PRICEa_0(p->ProbPrices, prob);
	nextOpt = &p->opt[cur + 1];
	nextIsLit = 0;
	if((nextOpt->price < kInfinityPrice && matchByte == curByte) || litPrice > nextOpt->price) litPrice = 0;
	else {
		litPrice += (state >= 7 ?
			LitEnc_Matched_GetPrice(p->litProbs, curByte, matchByte, p->ProbPrices) :
			LitEnc_GetPrice(p->litProbs, curByte, p->ProbPrices));
		if(litPrice < nextOpt->price) {
			nextOpt->price = litPrice;
			nextOpt->len = 1;
			_MakeAs_Lit(nextOpt);
			nextIsLit = 1;
		}
	}
	repMatchPrice = matchPrice + _GET_PRICEa_1(p->ProbPrices, p->isRep[state]);
	numAvailFull = min(p->numAvail, kNumOpts - 1 - cur);
	if(state < 7 && matchByte == curByte && repMatchPrice < nextOpt->price)
	if(nextOpt->len < 2 || nextOpt->dist) {
		uint32_t shortRepPrice = repMatchPrice + _GetPrice_ShortRep(p, state, 0);
		if(shortRepPrice < nextOpt->price) {
			nextOpt->price = shortRepPrice;
			nextOpt->len = 1;
			_MakeAs_ShortRep(nextOpt);
			nextIsLit = 0;
		}
	}
	if(numAvailFull < 2) continue;
	numAvail = numAvailFull <= fb ? numAvailFull : fb;
	if(!nextIsLit && litPrice  && matchByte != curByte && numAvailFull > 2) {
	  const uint8_t *data2 = data - reps[0];
	  if(data[1] == data2[1] && data[2] == data2[2]) {
		uint32_t limit = min(fb + 1, numAvailFull), len = 3;
		while(len < limit && data[len] == data2[len]) ++len;
		uint32_t price = litPrice + _GetPrice_Rep_0(p, kLiteralNextStates[state]);
		uint32_t offset = cur + len;
		last = max(last, offset);
		--len;
		uint32_t price2 = price + _GET_PRICE_LEN(p->repLenEnc, len);
		COptimal *opt = &p->opt[offset];
		if(price2 < opt->price) {
			opt->price = price2;
			opt->len = len;
			opt->dist = 0;
			opt->extra = 1;
		}
	  }
	}
	startLen = 2;
	for(uint32_t repIndex = 0; repIndex < LZMA_NUM_REPS; repIndex++) {
		uint32_t price, len2, len = 2;
		const uint8_t *data2 = data - reps[repIndex];
		if(data[0] != data2[0] || data[1] != data2[1]) continue;
		while(len < numAvail && data[len] == data2[len]) ++len;
		last = max(last, cur + len);
		len2 = len;
		price = repMatchPrice + GetPrice_PureRep(p, repIndex, state);
		do {
			uint32_t price2 = price + _GET_PRICE_LEN(p->repLenEnc, len2);
			COptimal *opt = &p->opt[cur + len2];
			if(price2 < opt->price) {
				opt->price = price2;
				opt->len = len2;
				opt->dist = repIndex;
				opt->extra = 0;
			}
		} while(--len2 >= 2);
		if(!repIndex) startLen = len + 1;
		len2 = len + 1;
		uint32_t limit = min(len2 + fb, numAvailFull);
		len2 += 2;
		if(len2 <= limit)
		if(data[len2 - 2] == data2[len2 - 2])
		if(data[len2 - 1] == data2[len2 - 1]) {
			uint32_t state2 = kRepNextStates[state];
			price += _GET_PRICE_LEN(p->repLenEnc, len)
				+ _GET_PRICEa_0(p->ProbPrices, p->isMatch[state2][0])
				+ LitEnc_Matched_GetPrice(p->litProbs, data[len], data2[len], p->ProbPrices);
			state2 = kState_LitAfterRep;
			price += _GetPrice_Rep_0(p, state2);
			while(len2 < limit && data[len2] == data2[len2]) ++len2;
			len2 -= len;
			uint32_t offset = cur + len + len2;
			last = max(last, offset);
			--len2;
			uint32_t price2 = price + _GET_PRICE_LEN(p->repLenEnc, len2);
			COptimal *opt = &p->opt[offset];
			if(price2 < opt->price) {
				opt->price = price2;
				opt->len = len2;
				opt->extra = len + 1;
				opt->dist = repIndex;
			}
		}
	}
	if(newLen > numAvail) {
		newLen = numAvail;
		for(numPairs = 0; newLen > matches[numPairs]; numPairs += 2);
		matches[numPairs] = newLen;
		numPairs += 2;
	}
	if(newLen >= startLen) {
	  uint32_t normalMatchPrice = matchPrice + _GET_PRICEa_0(p->ProbPrices, p->isRep[state]);
	  uint32_t offs = 0, posSlot, len, dist;
	  last = max(last, cur + newLen);
	  while(startLen > matches[offs]) offs += 2;
	  dist = matches[offs + 1];
	  _BSR2_RET(p, dist, posSlot);
	  for(len = startLen; ; len++) {
		uint32_t price = normalMatchPrice + _GET_PRICE_LEN(p->lenEnc, len);
		uint32_t lenNorm = _GetLenToPosState2(len - 2);
		if(dist < kNumFullDistances) price += p->distancesPrices[lenNorm][dist & (kNumFullDistances - 1)];
		else price += p->posSlotPrices[lenNorm][posSlot] + p->alignPrices[dist & kAlignMask];
		COptimal *opt = &p->opt[cur + len];
		if(price < opt->price) {
			opt->price = price;
			opt->len = len;
			opt->dist = dist + LZMA_NUM_REPS;
			opt->extra = 0;
		}
		if(len == matches[offs]) {
		  const uint8_t *data2 = data - dist - 1;
		  uint32_t len2 = len + 1;
		  uint32_t limit = len2 + fb;
		  if(limit > numAvailFull) limit = numAvailFull;
		  len2 += 2;
		  if(len2 <= limit)
		  if(data[len2 - 2] == data2[len2 - 2])
		  if(data[len2 - 1] == data2[len2 - 1]) {
			while(len2 < limit && data[len2] == data2[len2]) ++len2;
			len2 -= len;
			uint32_t state2 = kMatchNextStates[state];
			price += _GET_PRICEa_0(p->ProbPrices, p->isMatch[state2][0]);
			price += LitEnc_Matched_GetPrice(p->litProbs, data[len], data2[len], p->ProbPrices);
			state2 = kState_LitAfterMatch;
			price += _GetPrice_Rep_0(p, state2);
			uint32_t offset = cur + len + len2;
			if(last < offset) last = offset;
			--len2;
			uint32_t price2 = price + _GET_PRICE_LEN(p->repLenEnc, len2);
			COptimal *opt = &p->opt[offset];
			if(price2 < opt->price) {
				opt->price = price2;
				opt->len = len2;
				opt->extra = len + 1;
				opt->dist = dist + LZMA_NUM_REPS;
			}
		  }
		  offs += 2;
		  if(offs == numPairs) break;
		  dist = matches[(size_t)offs + 1];
		  _BSR2_RET(p, dist, posSlot);
		}
	  }
	}
  }
  do p->opt[last].price = kInfinityPrice;
  while(--last);
  return Backward(p, cur);
}

void LitEnc_EncodeMatched(CRangeEnc *p, uint16_t *probs, uint32_t sym, uint32_t matchByte) {
  uint32_t range = p->range;
  uint32_t offs = 0x100;
  sym |= 0x100;
  do {
	uint32_t ttt, newBound;
	uint16_t *prob;
	uint32_t bit;
	matchByte <<= 1;
	prob = probs + (offs + (matchByte & offs) + (sym >> 8));
	bit = (sym >> 7) & 1;
	sym <<= 1;
	offs &= ~(matchByte ^ sym);
	RC_BIT(ttt, newBound, range, p, prob, bit);
  } while (sym < 0x10000);
  p->range = range;
}

void LenEnc_Encode(CLenEnc *p, CRangeEnc *rc, uint32_t sym) {
  uint32_t range, ttt, newBound;
  uint16_t *probs = p->low;
  range = rc->range;
  RC_BIT_PRE(ttt, newBound, range, probs);
  if(sym >= kLenNumLowSymbols) {
	RC_BIT_1(ttt, newBound, range, rc, probs);
	probs += kLenNumLowSymbols;
	RC_BIT_PRE(ttt, newBound, range, probs);
	if(sym >= kLenNumLowSymbols * 2) {
	  RC_BIT_1(ttt, newBound, range, rc, probs);
	  rc->range = range;
	  LitEnc_Encode(rc, p->high, sym - kLenNumLowSymbols * 2);
	  return;
	}
	sym -= kLenNumLowSymbols;
  }
	uint32_t m;
	uint32_t bit;
	RC_BIT_0(ttt, newBound, range, rc, probs);
	bit = (sym >> 2)    ; RC_BIT(ttt, newBound, range, rc, probs + 1, bit);  m = (1 << 1) + bit;
	bit = (sym >> 1) & 1; RC_BIT(ttt, newBound, range, rc, probs + m, bit);  m = (m << 1) + bit;
	bit =  sym       & 1; RC_BIT(ttt, newBound, range, rc, probs + m, bit);
	rc->range = range;
}

void RcTree_ReverseEncode(CRangeEnc *rc, uint16_t *probs, uint32_t numBits, uint32_t sym) {
	uint32_t range = rc->range;
	uint32_t m = 1;
	do {
		uint32_t ttt, newBound;
		uint32_t bit = sym & 1;
		sym >>= 1;
		RC_BIT(ttt, newBound, range, rc, probs + m, bit);
		m = (m << 1) | bit;
	} while (--numBits);
	rc->range = range;
}

int LzmaEnc_CodeOneBlock(CLzmaEnc *p) {
  if(p->finished) return p->result;
  { const int _result_ = CheckErrors(p); if(_result_ ) return _result_; }
  uint32_t nowPos32 = p->nowPos64, startPos32 = nowPos32;
  if(!p->nowPos64) {
	uint32_t numPairs, curByte;
	if(!MatchFinder_GetNumAvailableBytes(&p->matchFinderBase)) return Flush(p, nowPos32);
	ReadMatchDistances(p, &numPairs);
	RangeEnc_EncodeBit_0(&p->rc, &p->isMatch[0][0]);
	curByte = *(MatchFinder_GetPointerToCurrentPos(&p->matchFinderBase) - p->additionalOffset);
	LitEnc_Encode(&p->rc, p->litProbs, curByte);
	p->additionalOffset--;
	nowPos32++;
  }
  if(MatchFinder_GetNumAvailableBytes(&p->matchFinderBase))
  for (;;) {
	uint32_t dist;
	uint32_t len;
	uint32_t range, ttt, newBound;
	uint16_t *probs;
	uint32_t oci = p->optCur;
	if(p->optEnd == oci) len = GetOptimum(p);
	else {
		const COptimal *opt = &p->opt[oci];
		len = opt->len;
		p->backRes = opt->dist;
		p->optCur = oci + 1;
	}
	range = p->rc.range;
	probs = &p->isMatch[p->state][0];
	RC_BIT_PRE(ttt, newBound, range, probs);
	dist = p->backRes;
	if(dist == uint32_t(-1)) {
		RC_BIT_0(ttt, newBound, range, &p->rc, probs);
		p->rc.range = range;
		const uint8_t *data = MatchFinder_GetPointerToCurrentPos(&p->matchFinderBase) - p->additionalOffset;
		probs = p->litProbs;
		uint32_t state = p->state;
		p->state = kLiteralNextStates[state];
		if(state < 7) LitEnc_Encode(&p->rc, probs, *data);
		else LitEnc_EncodeMatched(&p->rc, probs, *data, *(data - p->reps[0]));
	} else {
	  RC_BIT_1(ttt, newBound, range, &p->rc, probs);
	  probs = &p->isRep[p->state];
	  RC_BIT_PRE(ttt, newBound, range, probs);
	  if (dist < LZMA_NUM_REPS) {
		RC_BIT_1(ttt, newBound, range, &p->rc, probs);
		probs = &p->isRepG0[p->state];
		RC_BIT_PRE(ttt, newBound, range, probs);
		if(!dist) {
		  RC_BIT_0(ttt, newBound, range, &p->rc, probs);
		  probs = &p->isRep0Long[p->state][0];
		  RC_BIT_PRE(ttt, newBound, range, probs);
		  if(len != 1) RC_BIT_1_BASE(ttt, newBound, range, &p->rc, probs);
		  else {
			RC_BIT_0_BASE(ttt, newBound, range, &p->rc, probs);
			p->state = kShortRepNextStates[p->state];
		  }
		} else {
		  RC_BIT_1(ttt, newBound, range, &p->rc, probs);
		  probs = &p->isRepG1[p->state];
		  RC_BIT_PRE(ttt, newBound, range, probs);
		  if(dist == 1) {
			RC_BIT_0_BASE(ttt, newBound, range, &p->rc, probs);
			dist = p->reps[1];
		  } else {
			RC_BIT_1(ttt, newBound, range, &p->rc, probs);
			probs = &p->isRepG2[p->state];
			RC_BIT_PRE(ttt, newBound, range, probs);
			if(dist == 2) {
			  RC_BIT_0_BASE(ttt, newBound, range, &p->rc, probs);
			  dist = p->reps[2];
			} else {
			  RC_BIT_1_BASE(ttt, newBound, range, &p->rc, probs);
			  dist = p->reps[3];
			  p->reps[3] = p->reps[2];
			}
			p->reps[2] = p->reps[1];
		  }
		  p->reps[1] = p->reps[0];
		  p->reps[0] = dist;
		}
		RC_NORM(range, &p->rc);
		p->rc.range = range;
		if(len != 1) {
		  LenEnc_Encode(&p->repLenProbs, &p->rc, len - LZMA_MATCH_LEN_MIN);
		  --p->repLenEncCounter;
		  p->state = kRepNextStates[p->state];
		}
	  } else {
		uint32_t posSlot;
		RC_BIT_0(ttt, newBound, range, &p->rc, probs);
		p->rc.range = range;
		p->state = kMatchNextStates[p->state];
		LenEnc_Encode(&p->lenProbs, &p->rc, len - LZMA_MATCH_LEN_MIN);
		dist -= LZMA_NUM_REPS;
		p->reps[3] = p->reps[2];
		p->reps[2] = p->reps[1];
		p->reps[1] = p->reps[0];
		p->reps[0] = dist + 1;        
		p->matchPriceCount++;
		if(dist < kNumFullDistances) posSlot = p->g_FastPos[dist & (kNumFullDistances - 1)];
		else _BSR2_RET(p, dist, posSlot);
		uint32_t sym = posSlot + (1 << kNumPosSlotBits);
		range = p->rc.range;
		probs = p->posSlotEncoder[_GetLenToPosState(len)];
		do {
			uint16_t *prob = probs + (sym >> kNumPosSlotBits);
			uint32_t bit = (sym >> (kNumPosSlotBits - 1)) & 1;
			sym <<= 1;
			RC_BIT(ttt, newBound, range, &p->rc, prob, bit);
		} while (sym < (1 << kNumPosSlotBits * 2));
		p->rc.range = range;
		if(dist >= kStartPosModelIndex) {
		  uint32_t footerBits = ((posSlot >> 1) - 1);
		  if(dist < kNumFullDistances) {
			uint32_t base = ((2 | (posSlot & 1)) << footerBits);
			RcTree_ReverseEncode(&p->rc, p->posEncoders + base, footerBits, (dist));
		  } else {
			uint32_t pos2 = (dist | 0xF) << (32 - footerBits);
			range = p->rc.range;
			do {
				range >>= 1;
				p->rc.low += range & (0 - (pos2 >> 31));
				pos2 += pos2;
				RC_NORM(range, &p->rc);
			} while (pos2 != 0xF0000000);
			uint32_t m = 1;
			uint32_t bit;
			for(int i = 0; i < 3; ++i) {
				bit = dist & 1;
				dist >>= 1;
				RC_BIT(ttt, newBound, range, &p->rc, p->posAlignEncoder + m, bit);
				m = (m << 1) + bit;
			}
			bit = dist & 1;
			RC_BIT(ttt, newBound, range, &p->rc, p->posAlignEncoder + m, bit);
			p->rc.range = range;
		  }
		}
	  }
	}
	nowPos32 += len;
	p->additionalOffset -= len;
	if(!p->additionalOffset) {
	  uint32_t processed;
		if(p->matchPriceCount >= 64) {
			FillAlignPrices(p);
			FillDistancesPrices(p);
			LenPriceEnc_UpdateTables(p->lenEnc, &p->lenProbs, p->ProbPrices);
		}
		if(p->repLenEncCounter <= 0) {
			p->repLenEncCounter = REP_LEN_COUNT;
			LenPriceEnc_UpdateTables(p->repLenEnc, &p->repLenProbs, p->ProbPrices);
		}
	  if(!MatchFinder_GetNumAvailableBytes(&p->matchFinderBase)) break;
	  processed = nowPos32 - startPos32;
	  if(processed >= (1 << 17)) {
		p->nowPos64 += nowPos32 - startPos32;
		return CheckErrors(p);
	  }
	}
  }
  p->nowPos64 += nowPos32 - startPos32;
  return Flush(p, nowPos32);
}

void LzmaEncode(uint8_t *dest, size_t *destLen, const uint8_t *src, size_t srcLen) {
	CLzmaEnc p;
	uint8_t *g = p.g_FastPos;
	g[0] = 0; g[1] = 1; g += 2;
	for(uint32_t slot = 2; slot < kNumLogBits * 2; slot++)
		g = fill_n(g, 1u << ((slot >> 1) - 1), slot);
	for(uint32_t i = 0; i < (kBitModelTotal >> kNumMoveReducingBits); ++i) {
		uint32_t w = (i << kNumMoveReducingBits) + (1 << (kNumMoveReducingBits - 1)), bitCount = 0;
		for(uint32_t j = 0; j < kNumBitPriceShiftBits; ++j) {
			w = w * w;
			bitCount <<= 1;
			while(w >= (1 << 16)) {
				w >>= 1;
				++bitCount;
			}
		}
		p.ProbPrices[i] = (kNumBitModelTotalBits << kNumBitPriceShiftBits) - 15 - bitCount;
	}
	p.rc.data = dest;
	p.finished = 0;
	p.result = 0;
	p.nowPos64 = 0;
	p.rc.buf = p.rc.bufBase = (uint8_t*) malloc(RC_BUF_SIZE);
	p.rc.bufLim = p.rc.bufBase + RC_BUF_SIZE;
	uint32_t i;
	p.state = 0;
	fill_n(p.reps, 4, 1);
	p.rc.range = 0xFFFFFFFF;
	p.rc.cache = 0;
	p.rc.low = 0;
	p.rc.cacheSize = 0;
	p.rc.processed = 0;
	fill_n(p.posAlignEncoder, (1 << kNumAlignBits), kProbInitValue);
	for(i = 0; i < kNumStates; i++) {
		for(uint16_t* x : {p.isMatch[i], p.isRep0Long[i]})
			fill_n(x, LZMA_NUM_PB_STATES_MAX, kProbInitValue);
		p.isRep[i] = p.isRepG0[i] = p.isRepG1[i] = p.isRepG2[i] = kProbInitValue;
	}
	for(i = 0; i < kNumLenToPosStates; i++)
		fill_n(p.posSlotEncoder[i], 1 << kNumPosSlotBits, kProbInitValue);
	fill_n(p.posEncoders, kNumFullDistances, kProbInitValue);
	fill_n(p.litProbs, 0x300u, kProbInitValue);
	for(CLenEnc *p : {&p.lenProbs, &p.repLenProbs}) {
		fill_n(p->low, std::size(p->low), kProbInitValue);
		fill_n(p->high, std::size(p->high), kProbInitValue);
	}
	p.optEnd = 0;
	p.optCur = 0;
	for(i = 0; i < kNumOpts; i++) p.opt[i].price = kInfinityPrice;
	p.additionalOffset = 0;
	FillDistancesPrices(&p);
	FillAlignPrices(&p);
	p.repLenEncCounter = REP_LEN_COUNT;
	LenPriceEnc_UpdateTables(p.lenEnc, &p.lenProbs, p.ProbPrices);
	LenPriceEnc_UpdateTables(p.repLenEnc, &p.repLenProbs, p.ProbPrices);
	p.matchFinderBase.buffer = src;
	size_t newSize = ((1<<16) + 2*(dictSize + 1) + 0xF) & ~(size_t)0xF;
	p.matchFinderBase.hash = (uint32_t*) malloc(newSize*4);
	p.matchFinderBase.son = p.matchFinderBase.hash + (1<<16);
	fill_n(p.matchFinderBase.hash, hashMask+1, 0u);
	p.matchFinderBase.cyclicBufferPos = p.matchFinderBase.pos = 1;
	p.matchFinderBase.streamPos = srcLen+1;
	MatchFinder_SetLimits(&p.matchFinderBase);
	while(!LzmaEnc_CodeOneBlock(&p) && !p.finished);
	*destLen = p.rc.data - dest;
	free(p.matchFinderBase.hash);
	free(p.matchFinderBase.bufBase);
	free(p.rc.bufBase);
}

td::BufferSlice encode(const td::Slice& data) {
	size_t len = 2<<20;
	td::BufferSlice buffer(len);
	LzmaEncode((uint8_t*)buffer.data(), &len, (uint8_t*)data.data(), data.size());
	return td::BufferSlice(buffer.data(), len);
}

enum ELzmaStatus {
	LZMA_STATUS_NOT_SPECIFIED,
	LZMA_STATUS_FINISHED_WITH_MARK,
	LZMA_STATUS_NOT_FINISHED,
	LZMA_STATUS_NEEDS_MORE_INPUT,
	LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK
};

struct CLzmaDec {
	uint16_t *probs;
	uint8_t *dic;
	size_t dicBufSize;
	size_t dicPos;
	const uint8_t *buf;
	uint32_t range;
	uint32_t code;
	uint32_t processedPos;
	uint32_t checkDicSize;
	uint32_t reps[4];
	uint32_t state;
	uint32_t remainLen;
	uint32_t numProbs;
	uint32_t tempBufSize;
	uint8_t tempBuf[LZMA_REQUIRED_INPUT_MAX];
};

void LzmaDec_WriteRem(CLzmaDec *p, size_t limit) {
	uint32_t len = min(p->remainLen, uint32_t(limit - p->dicPos));
	if(!len) return;
	if(!p->checkDicSize && dictSize - p->processedPos <= len)
		p->checkDicSize = dictSize;
	p->processedPos += len;
	p->remainLen -= len;
	do {
		p->dic[p->dicPos] = p->dic[p->dicPos - p->reps[0] + (p->dicPos < p->reps[0]) * p->dicBufSize];
		++p->dicPos;
	} while(--len);
}

enum ELzmaDummy {
  DUMMY_INPUT_EOF,
  DUMMY_LIT,
  DUMMY_MATCH,
  DUMMY_REP
};

#define NORMALIZE_CHECK if(range < kTopValue) { if(buf >= bufLimit) return DUMMY_INPUT_EOF; range <<= 8; code = (code << 8) | (*buf++); }
#define IF_BIT_0_CHECK(p) ttt = *(p); NORMALIZE_CHECK bound = (range >> kNumBitModelTotalBits) * ttt; if (code < bound)
#define GET_BIT2_CHECK(p, i, A0, A1) IF_BIT_0_CHECK(p) \
  { range = bound;  i = (i + i); A0; } else \
  { range -= bound; code -= bound;  i = (i + i) + 1; A1; }
#define GET_BIT_CHECK(p, i) GET_BIT2_CHECK(p, i, ; , ;)
#define TREE_DECODE_CHECK(probs, limit, i) { i = 1; do { GET_BIT_CHECK(probs + i, i) } while (i < limit); i -= limit; }

ELzmaDummy LzmaDec_TryDummy(const CLzmaDec *p, const uint8_t *buf, const uint8_t **bufOut) {
  uint32_t range = p->range;
  uint32_t code = p->code;
  const uint8_t *bufLimit = *bufOut;
  const uint16_t *probs = p->probs + 1664;
  uint32_t state = p->state;
  ELzmaDummy res;
  for(;;) {
	const uint16_t *prob;
	uint32_t bound;
	uint32_t ttt;
	prob = probs + IsMatch + state;
	IF_BIT_0_CHECK(prob) {
		range = bound;
		prob = probs + Literal;
	  if(state < kNumLitStates) {
		uint32_t symbol = 1;
		do { GET_BIT_CHECK(prob + symbol, symbol) } while(symbol < 0x100);
	  } else {
		uint32_t matchByte = p->dic[p->dicPos - p->reps[0] + (p->dicPos < p->reps[0]) * p->dicBufSize];
		uint32_t offs = 0x100, symbol = 1;
		do {
			matchByte += matchByte;
			uint32_t bit = offs;
			offs &= matchByte;
			const uint16_t *probLit = prob + (offs + bit + symbol);
			GET_BIT2_CHECK(probLit, symbol, offs ^= bit; , ; )
		} while (symbol < 0x100);
	  }
	  res = DUMMY_LIT;
	} else {
	  uint32_t len;
	  range -= bound;
	  code -= bound;
	  prob = probs + IsRep + state;
	  IF_BIT_0_CHECK(prob) {
		range = bound;
		state = 0;
		prob = probs + LenCoder;
		res = DUMMY_MATCH;
	  } else {
		range -= bound;
		code -= bound;
		res = DUMMY_REP;
		prob = probs + IsRepG0 + state;
		IF_BIT_0_CHECK(prob) {
		  range = bound;
		  prob = probs + IsRep0Long + state;
		  IF_BIT_0_CHECK(prob) {
			range = bound;
			break;
		  } else {
			range -= bound;
			code -= bound;
		  }
		} else {
		  range -= bound;
		  code -= bound;
		  prob = probs + IsRepG1 + state;
		  IF_BIT_0_CHECK(prob) {
			range = bound;
		  } else {
			range -= bound;
			code -= bound;
			prob = probs + IsRepG2 + state;
			IF_BIT_0_CHECK(prob) {
			  range = bound;
			} else {
			  range -= bound;
			  code -= bound;
			}
		  }
		}
		state = kNumStates;
		prob = probs + RepLenCoder;
	  }
		uint32_t limit, offset;
		const uint16_t *probLen = prob;
		IF_BIT_0_CHECK(probLen) {
		  range = bound;
		  probLen = prob;
		  offset = 0;
		  limit = 1 << kLenNumLowBits;
		} else {
			range -= bound;
			code -= bound;
		  probLen = prob + (1 << kLenNumLowBits);
		  IF_BIT_0_CHECK(probLen) {
			range = bound;
			probLen = prob + (1 << kLenNumLowBits);
			offset = kLenNumLowSymbols;
			limit = 1 << kLenNumLowBits;
		  } else {
			range -= bound;
			code -= bound;
			probLen = prob + LenHigh;
			offset = kLenNumLowSymbols * 2;
			limit = 1 << kLenNumHighBits;
		  }
		}
		TREE_DECODE_CHECK(probLen, limit, len)
		len += offset;
	  if(state < 4) {
		uint32_t posSlot;
		prob = probs + PosSlot + ((len < kNumLenToPosStates - 1 ? len : kNumLenToPosStates - 1) << kNumPosSlotBits);
		TREE_DECODE_CHECK(prob, 1 << kNumPosSlotBits, posSlot)
		if(posSlot >= kStartPosModelIndex) {
		  uint32_t numDirectBits = ((posSlot >> 1) - 1);
		  if(posSlot < kEndPosModelIndex) {
			prob = probs + SpecPos + ((2 | (posSlot & 1)) << numDirectBits);
		  } else {
			numDirectBits -= kNumAlignBits;
			do {
			  NORMALIZE_CHECK
			  range /= 2;
			  code -= range & (((code - range) >> 31) - 1);
			} while(--numDirectBits);
			prob = probs + Align;
			numDirectBits = kNumAlignBits;
		  }
			uint32_t i = 1, m = 1;
			do {
				IF_BIT_0_CHECK(prob + i) {
					range = bound;
					i += m;
					m += m;
				} else {
					range -= bound;
					code -= bound;
					m += m;
					i += m;
				}
			} while(--numDirectBits);
		}
	  }
	}
	break;
  }
  NORMALIZE_CHECK
  *bufOut = buf;
  return res;
}

inline void NORMALIZE(uint32_t &range, uint32_t &code, const uint8_t* &buf) {
	if(range < kTopValue) {
		range <<= 8;
		code = (code << 8) | (*buf++); 
	}
}
inline bool BIT_0(uint32_t &range, uint32_t &code, const uint8_t* &buf, uint32_t &ttt, uint32_t &bound, uint16_t *p) {
	ttt = *p;
	NORMALIZE(range, code, buf);
	bound = (range >> kNumBitModelTotalBits) * ttt;
	return code < bound;
}
inline void UPDATE_0(uint32_t &range, uint32_t ttt, uint32_t bound, uint16_t *p) {
	range = bound;
	*p = (ttt + ((kBitModelTotal - ttt) >> kNumMoveBits));
}
inline void UPDATE_1(uint32_t &range, uint32_t &code, uint32_t ttt, uint32_t bound, uint16_t *p) {
	range -= bound;
	code -= bound;
	*p = (ttt - (ttt >> kNumMoveBits));
}
inline void TREE_GET_BIT(uint32_t &range, uint32_t &code, const uint8_t* &buf, uint32_t &ttt, uint32_t &bound, uint16_t *probs, uint32_t &i) {
	probs += i;
	if(BIT_0(range, code, buf, ttt, bound, probs)) {
		UPDATE_0(range, ttt, bound, probs);
		i = i + i;
	} else {
		UPDATE_1(range, code, ttt, bound, probs);
		i = i + i + 1;
	}
}

int LZMA_DECODE_REAL(CLzmaDec *p, size_t limit, const uint8_t *bufLimit) {
	uint16_t *probs = p->probs + 1664;
	uint32_t state = p->state, rep0 = p->reps[0], rep1 = p->reps[1], rep2 = p->reps[2], rep3 = p->reps[3];
	size_t dicBufSize = p->dicBufSize, dicPos = p->dicPos;
	uint32_t processedPos = p->processedPos, checkDicSize = p->checkDicSize, len = 0;
	const uint8_t *buf = p->buf;
	uint32_t range = p->range, code = p->code;
  do {
	uint32_t bound, ttt;
	uint16_t *prob = probs + IsMatch + state;
	if(BIT_0(range, code, buf, ttt, bound, prob)) {
	  uint32_t symbol = 1;
	  UPDATE_0(range, ttt, bound, prob);
	  prob = probs + Literal;
	  processedPos++;
	  if(state < kNumLitStates) {
		state -= (state < 4) ? state : 3;
		for(int i = 0; i < 8; ++i) TREE_GET_BIT(range, code, buf, ttt, bound, prob, symbol);
	  } else {
		uint32_t matchByte = p->dic[dicPos - rep0 + (dicPos < rep0) * dicBufSize];
		uint32_t offs = 0x100, bit;
		state -= (state < 10) ? 3 : 6;
		uint16_t *probLit;
		for(int i = 0; i < 8; ++i) {
			matchByte += matchByte;
			bit = offs;
			offs &= matchByte;
			probLit = prob + (offs + bit + symbol);
			if(BIT_0(range, code, buf, ttt, bound, probLit)) {
				UPDATE_0(range, ttt, bound, probLit);
				symbol = symbol + symbol;
				offs ^= bit;
			} else {
				UPDATE_1(range, code, ttt, bound, probLit);
				symbol = symbol + symbol + 1;
			}
		}
	  }
	  p->dic[dicPos++] = symbol;
	  continue;
	}
	UPDATE_1(range, code, ttt, bound, prob);
	prob = probs + IsRep + state;
	if(BIT_0(range, code, buf, ttt, bound, prob)) {
		UPDATE_0(range, ttt, bound, prob);
		state += kNumStates;
		prob = probs + LenCoder;
	} else {
		UPDATE_1(range, code, ttt, bound, prob);
		prob = probs + IsRepG0 + state;
		if(BIT_0(range, code, buf, ttt, bound, prob)) {
		  UPDATE_0(range, ttt, bound, prob);
		  prob = probs + IsRep0Long + state;
		  if(BIT_0(range, code, buf, ttt, bound, prob)) {
			UPDATE_0(range, ttt, bound, prob);
			p->dic[dicPos] = p->dic[dicPos - rep0 + (dicPos < rep0) * dicBufSize];
			dicPos++;
			processedPos++;
			state = state < kNumLitStates ? 9 : 11;
			continue;
		  }
		  UPDATE_1(range, code, ttt, bound, prob);
		} else {
		  uint32_t distance;
		  UPDATE_1(range, code, ttt, bound, prob);
		  prob = probs + IsRepG1 + state;
		  if(BIT_0(range, code, buf, ttt, bound, prob)) {
			UPDATE_0(range, ttt, bound, prob);
			distance = rep1;
		  } else {
			UPDATE_1(range, code, ttt, bound, prob);
			prob = probs + IsRepG2 + state;
			if(BIT_0(range, code, buf, ttt, bound, prob)) {
				UPDATE_0(range, ttt, bound, prob);
				distance = rep2;
			} else {
				UPDATE_1(range, code, ttt, bound, prob);
				distance = rep3;
				rep3 = rep2;
			}
			rep2 = rep1;
		  }
		  rep1 = rep0;
		  rep0 = distance;
		}
		state = state < kNumLitStates ? 8 : 11;
		prob = probs + RepLenCoder;
	}
	uint16_t *probLen = prob;
	if(BIT_0(range, code, buf, ttt, bound, probLen)) {
		UPDATE_0(range, ttt, bound, probLen);
		probLen = prob;
		len = 1;
		for(int i = 0; i < 3; ++i) TREE_GET_BIT(range, code, buf, ttt, bound, probLen, len);
		len -= 8;
	} else {
		UPDATE_1(range, code, ttt, bound, probLen);
		probLen = prob + (1 << kLenNumLowBits);
		if(BIT_0(range, code, buf, ttt, bound, probLen)) {
			UPDATE_0(range, ttt, bound, probLen);
			probLen = prob + (1 << kLenNumLowBits);
			len = 1;
			for(int i = 0; i < 3; ++i) TREE_GET_BIT(range, code, buf, ttt, bound, probLen, len);
		} else {
			UPDATE_1(range, code, ttt, bound, probLen);
			probLen = prob + LenHigh;
			len = 1;
			do {
				TREE_GET_BIT(range, code, buf, ttt, bound, probLen, len);
			} while(len < (1 << kLenNumHighBits));
			len -= 1 << kLenNumHighBits;
			len += kLenNumLowSymbols * 2;
		}
	}
	if(state >= kNumStates) {
		prob = probs + PosSlot + ((len < kNumLenToPosStates ? len : kNumLenToPosStates - 1) << kNumPosSlotBits);
		uint32_t distance = 1;
		for(int i = 0; i < 6; ++i) TREE_GET_BIT(range, code, buf, ttt, bound, prob, distance);
		distance -= 0x40;
		if(distance >= kStartPosModelIndex) {
		  uint32_t posSlot = distance;
		  uint32_t numDirectBits = (distance >> 1) - 1;
		  distance = 2 | (distance & 1);
		  if(posSlot < kEndPosModelIndex) {
			distance <<= numDirectBits;
			prob = probs + SpecPos;
			uint32_t m = 1;
			distance++;
			do {
				if(BIT_0(range, code, buf, ttt, bound, prob+distance)) {
					UPDATE_0(range, ttt, bound, prob+distance);
					distance += m;
					m += m;
				} else {
					UPDATE_1(range, code, ttt, bound, prob+distance);
					m += m;
					distance += m;
				}
			} while (--numDirectBits);
			distance -= m;
		  } else {
			numDirectBits -= kNumAlignBits;
			do {
				NORMALIZE(range, code, buf);
				range >>= 1;
				code -= range;
				uint32_t t = 0 - (code >> 31);
				distance = (distance << 1) + (t + 1);
				code += range & t;
			} while (--numDirectBits);
			prob = probs + Align;
			distance <<= kNumAlignBits;
			uint32_t i = 1;
			for(uint32_t m : {1,2,4}) {
				if(BIT_0(range, code, buf, ttt, bound, prob+i)) {
					UPDATE_0(range, ttt, bound, prob+i);
					i += m;
				} else {
					UPDATE_1(range, code, ttt, bound, prob+i);
					i += m * 2;
				}
			}
			prob += i;
			if(BIT_0(range, code, buf, ttt, bound, prob)) {
				UPDATE_0(range, ttt, bound, prob);
				i -= 8;
			} else UPDATE_1(range, code, ttt, bound, prob);
			distance |= i;
			if(distance == 0xFFFFFFFFu) {
				len = kMatchSpecLenStart;
				state -= kNumStates;
				break;
			}
		  }
		}
		rep3 = rep2;
		rep2 = rep1;
		rep1 = rep0;
		rep0 = distance + 1;
		state = (state < kNumStates + kNumLitStates) ? kNumLitStates : kNumLitStates + 3;
		if(distance >= (!checkDicSize ? processedPos: checkDicSize)) {
			len += kMatchSpecLen_Error_Data + kMatchMinLen;
			break;
		}
	}
	len += kMatchMinLen;
	size_t rem = limit - dicPos;
	if(!rem) break;
	uint32_t curLen = (rem < len) ? (uint32_t)rem : len;
	size_t pos = dicPos - rep0 + (dicPos < rep0) * dicBufSize;
	processedPos += curLen;
	len -= curLen;
	if(curLen <= dicBufSize - pos) {
		  uint8_t *dest = p->dic + dicPos;
		  ptrdiff_t src = (ptrdiff_t)pos - (ptrdiff_t)dicPos;
		  const uint8_t *lim = dest + curLen;
		  dicPos += (size_t)curLen;
		  do *dest = *(dest + src);
		  while(++dest != lim);
	} else {
		  do {
			p->dic[dicPos++] = p->dic[pos];
			if(++pos == dicBufSize) pos = 0;
		  } while (--curLen );
	}
	} while(dicPos < limit && buf < bufLimit);
	NORMALIZE(range, code, buf);
	p->buf = buf;
	p->range = range;
	p->code = code;
	p->remainLen = len;
	p->dicPos = dicPos;
	p->processedPos = processedPos;
	p->reps[0] = rep0;
	p->reps[1] = rep1;
	p->reps[2] = rep2;
	p->reps[3] = rep3;
	p->state = state;
	if(len >= kMatchSpecLen_Error_Data) return SZ_ERROR_DATA;
	return 0;
}

int LzmaDec_DecodeReal2(CLzmaDec *p, size_t limit, const uint8_t *bufLimit) {
	if(!p->checkDicSize) limit = min(limit, dictSize - p->processedPos + p->dicPos);
	int res = LZMA_DECODE_REAL(p, limit, bufLimit);
	if(!p->checkDicSize && p->processedPos >= dictSize) p->checkDicSize = dictSize;
	return res;
}

int LzmaDec_DecodeToDic(CLzmaDec *p, size_t dicLimit, const uint8_t *src, size_t *srcLen, ELzmaStatus *status) {
  size_t inSize = *srcLen;
  *srcLen = 0;
  *status = LZMA_STATUS_NOT_SPECIFIED;
  if(p->remainLen > kMatchSpecLenStart) {
	if(p->remainLen > kMatchSpecLenStart + 2)
		return p->remainLen == kMatchSpecLen_Error_Fail ? SZ_ERROR_FAIL : SZ_ERROR_DATA;
	for(; inSize > 0 && p->tempBufSize < RC_INIT_SIZE; (*srcLen)++, inSize--)
		p->tempBuf[p->tempBufSize++] = *src++;
	if(p->tempBufSize && p->tempBuf[0]) return SZ_ERROR_DATA;
	if(p->tempBufSize < RC_INIT_SIZE) {
		*status = LZMA_STATUS_NEEDS_MORE_INPUT;
		return 0;
	}
	p->code =
		((uint32_t)p->tempBuf[1] << 24)
	  | ((uint32_t)p->tempBuf[2] << 16)
	  | ((uint32_t)p->tempBuf[3] << 8)
	  | ((uint32_t)p->tempBuf[4]);
	if(!p->checkDicSize && !p->processedPos && p->code >= kBadRepCode) return SZ_ERROR_DATA;
	p->range = 0xFFFFFFFF;
	p->tempBufSize = 0;
	if(p->remainLen > kMatchSpecLenStart + 1) {
		fill_n(p->probs, NUM_BASE_PROBS + LZMA_LIT_SIZE, kBitModelTotal >> 1);
		fill_n(p->reps, 4, 1);
		p->state = 0;
	}
	p->remainLen = 0;
  }
  for(;;) {
	if(p->remainLen == kMatchSpecLenStart) {
		if(p->code) return SZ_ERROR_DATA;
		*status = LZMA_STATUS_FINISHED_WITH_MARK;
		return 0;
	}
	LzmaDec_WriteRem(p, dicLimit);
	  int checkEndMarkNow = 0;
	  if(p->dicPos >= dicLimit) {
		if(!p->remainLen && !p->code) {
			*status = LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK;
			return 0;
		}
		*status = LZMA_STATUS_NOT_FINISHED;
		return 0;
	  }
	  if(!p->tempBufSize) {
		const uint8_t *bufLimit = src + inSize - LZMA_REQUIRED_INPUT_MAX;
		int dummyProcessed = -1;
		if(inSize < LZMA_REQUIRED_INPUT_MAX || checkEndMarkNow) {
		  const uint8_t *bufOut = src + inSize;
		  ELzmaDummy dummyRes = LzmaDec_TryDummy(p, src, &bufOut);
		  if(dummyRes == DUMMY_INPUT_EOF) {
			if(inSize >= LZMA_REQUIRED_INPUT_MAX) break;
			*srcLen += inSize;
			p->tempBufSize = inSize;
			copy_n(src, inSize, p->tempBuf);
			*status = LZMA_STATUS_NEEDS_MORE_INPUT;
			return 0;
		  }
		  dummyProcessed = bufOut - src;
		  if((uint32_t)dummyProcessed > LZMA_REQUIRED_INPUT_MAX) break;
		  if(checkEndMarkNow && dummyRes != DUMMY_MATCH) {
			*srcLen += dummyProcessed;
			p->tempBufSize = dummyProcessed;
			copy_n(src, dummyProcessed, p->tempBuf);
			*status = LZMA_STATUS_NOT_FINISHED;
			return SZ_ERROR_DATA;
		  }
		  bufLimit = src;
		}
		p->buf = src;
		int res = LzmaDec_DecodeReal2(p, dicLimit, bufLimit);
		size_t processed = (size_t)(p->buf - src);
		if(dummyProcessed < 0) {
			if(processed > inSize) break;
		} else if((uint32_t)dummyProcessed != processed) break;
		src += processed;
		inSize -= processed;
		*srcLen += processed;
		if(res) {
			p->remainLen = kMatchSpecLen_Error_Data;
			return SZ_ERROR_DATA;
		}
		continue;
	  }
		uint32_t rem = p->tempBufSize;
		uint32_t ahead = 0;
		int dummyProcessed = -1;
		while(rem < LZMA_REQUIRED_INPUT_MAX && ahead < inSize)
		  p->tempBuf[rem++] = src[ahead++];
		if(rem < LZMA_REQUIRED_INPUT_MAX || checkEndMarkNow) {
		  const uint8_t *bufOut = p->tempBuf + rem;
		  ELzmaDummy dummyRes = LzmaDec_TryDummy(p, p->tempBuf, &bufOut);
		  if(dummyRes == DUMMY_INPUT_EOF) {
			if(rem >= LZMA_REQUIRED_INPUT_MAX) break;
			p->tempBufSize = rem;
			*srcLen += ahead;
			*status = LZMA_STATUS_NEEDS_MORE_INPUT;
			return 0;
		  }
		  dummyProcessed = (int)(bufOut - p->tempBuf);
		  if((uint32_t)dummyProcessed < p->tempBufSize) break;
		  if(checkEndMarkNow && dummyRes != DUMMY_MATCH) {
			*srcLen += dummyProcessed - p->tempBufSize;
			p->tempBufSize = dummyProcessed;
			*status = LZMA_STATUS_NOT_FINISHED;
			return SZ_ERROR_DATA;
		  }
		}
		p->buf = p->tempBuf;
		int res = LzmaDec_DecodeReal2(p, dicLimit, p->buf);
		size_t processed = (size_t)(p->buf - p->tempBuf);
		rem = p->tempBufSize;
		if(dummyProcessed < 0) {
			if(processed > LZMA_REQUIRED_INPUT_MAX) break;
			if(processed < rem) break;
		} else if ((uint32_t)dummyProcessed != processed) break;
		processed -= rem;
		src += processed;
		inSize -= processed;
		*srcLen += processed;
		p->tempBufSize = 0;
		if(res) {
			p->remainLen = kMatchSpecLen_Error_Data;
			return SZ_ERROR_DATA;
		}
  }
  p->remainLen = kMatchSpecLen_Error_Fail;
  return SZ_ERROR_FAIL;
}

int LzmaDecode(uint8_t *dest, size_t *destLen, const uint8_t *src, size_t *srcLen) {
	CLzmaDec p;
	size_t outSize = *destLen, inSize = *srcLen;
	*destLen = *srcLen = 0;
	uint32_t numProbs = NUM_BASE_PROBS + LZMA_LIT_SIZE;
	p.probs = (uint16_t*) malloc(numProbs * sizeof(uint16_t));
	p.numProbs = numProbs;
	p.dic = dest;
	p.dicBufSize = outSize;
	p.dicPos = 0;
	p.tempBufSize = 0;
	p.processedPos = 0;
	p.checkDicSize = 0;
	p.remainLen = kMatchSpecLenStart + 2;
	*srcLen = inSize;
	ELzmaStatus status;
	int res = LzmaDec_DecodeToDic(&p, outSize, src, srcLen, &status);
	*destLen = p.dicPos;
	if(!res && status == LZMA_STATUS_NEEDS_MORE_INPUT) res = SZ_ERROR_INPUT_EOF;
	free(p.probs);
	return res;
}

td::BufferSlice decode(const td::Slice& data) {
	size_t len = 2<<20;
	td::BufferSlice buffer(len);
	size_t in_len = data.size();
	LzmaDecode((uint8_t*)buffer.data(), &len, (uint8_t*)data.data(), &in_len);
	return td::BufferSlice(buffer.data(), len);
}

td::BufferSlice compress(td::Slice data) {
	td::Ref<vm::Cell> root = vm::std_boc_deserialize(data).move_as_ok();
	BOC boc;
	boc.import_cells(root);
	uint32_t ref_size = 1;
	while((1ull << ref_size) <= boc.cell_list.size()) ++ref_size;
	td::BufferSlice res;
	while(ref_size <= 8*REF_SIZE) {
		clearStuff();
		do_align = (ref_size%8) == 0;
		BitWriter writer = boc.serialize_to_impl(ref_size);
		td::BufferSlice serialized = encode(writer.toSlice());
		// td::BufferSlice serialized = writer.toSlice();
		if(res.empty() || serialized.size() < res.size())
			res = std::move(serialized);
		if(smallest_ref) break;
		++ref_size;
	}
	return res;
}

td::BufferSlice decompress(td::Slice data) {
	clearStuff();
	td::BufferSlice serialized = decode(data);
	auto root = deserialise(serialized);
	// auto root = deserialise(data);
	return vm::std_boc_serialize(root, 31).move_as_ok();
}

int main() {
	string mode, base64_data;
	cin >> mode >> base64_data;
	td::BufferSlice data(td::base64_decode(base64_data).move_as_ok());
	data = mode == "compress" ? compress(data) : decompress(data);
	cout << td::base64_encode(data) << endl;
	// string s;
	// while(!cin.eof()) s.push_back(cin.get());
	// *(uint32_t*)(s.data()+s.size()-4) = td::crc32c(td::Slice{s.data(), s.data()+s.size()-4});
	// // cout << td::base64_encode(compress(td::Slice(s))) << endl;
	// cout << "compress\n" << td::base64_encode(decompress(compress(td::Slice(s)))) << endl;
}
