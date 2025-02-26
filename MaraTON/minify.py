keys = {
	"include",
	"iomanip",
	"setfill",
	"setw",
	"include",
	"iostream",
	"ostream",
	"cstdint",
	"int",
	"uint32_t",
	"td",
	"define",
	"static",
	"constexpr",
	"using",
	"namespace",
	"std",
	"utils",
	"h",
	"base64",
	"crypto",
	"boc",
	"vm",
	"cells",
	"CellSlice",
	"sizeof",
	"size_t",
	"uint8_t",
	"char",
	"const",
	"uint16_t",
	"void",
	"struct",
	"enum",
	"Ref",
	"vector",
	"array",
	"BufferSlice",
	"DataCell",
	"push_back",
	"size",
	"if",
	"else",
	"for",
	"while",
	"do",
	"copy_n",
	"data",
	"pop_back",
	"return",
	"uint64_t",
	"HashMap",
	"map",
	"pair",
	"lower_bound",
	"Hash",
	"Cell",
	"bool",
	"move",
	"min",
	"max",
	"fill_n",
	"auto",
	"find",
	"get_hash",
	"end",
	"first",
	"second",
	"load_cell",
	"move_as_ok",
	"data_cell",
	"get_ref",
	"size_refs",
	"emplace",
	"emplace_hint",
	"get_serialized_size",
	"emplace_back",
	"reserve",
	"clear",
	"reverse",
	"begin",
	"reinterpret_cast",
	"get",
	"this",
	"is_special",
	"get_data",
	"switch",
	"case",
	"get_level",
	"get_bits",
	"memcpy",
	"break",
	"default",
	"info_",
	"d2",
	"Slice",
	"as_array",
	"get_depth",
	"CellBuilder",
	"crc32c",
	"std_boc_deserialize",
	"store_bits",
	"store_ref",
	"finalize",
	"bswap32",
	"inline",
	"continue",
	"ptrdiff_t",
	"min_element",
	"malloc",
	"free",
	"empty",
	"string",
	"cin",
	"cout",
	"std_boc_serialize",
	"base64_encode",
	"base64_decode",
	"endl",
	"tm",
	"or",
	"main",
	"compress",
	"cerr",
	"ConstBitPtr",
	"to_hex",
	"strncmp",
	"resize",
	"make_pair",
	"operator",
	"true",
	"false",
	"fill",
	"count_leading_zeroes32",
	"tie",
	"cassert",
	"limits",
	"template",
	"typename",
	"is_same_v",
	"numeric_limits",
	"is_integer",
	"assert",
	"static_assert",
	"hex",
	"dec",
	"ws",
	"__VA_ARGS__",
	"jn",
	"yn"
}
words = {}
nw = 0
def get_next_word():
	global nw
	w = "int"
	while w in keys:
		w = ""
		nw += 1
		x = nw
		while x:
			w += chr(ord('a')+(x%26))
			x //= 26
	return w
def ispace(c):
	return c in " \t\n"
lp = True
while True:
	try:
		l = input() + '\n'
		l2 = ""
		w = ""
		for c in l:
			if 'a' <= c <= 'z' or 'A' <= c <= 'Z' or c == '_' or '0' <= c <= '9':
				w += c
				lp = False
			else:
				if w != "":
					if (w[0] < '0' or w[0] > '9') and (not w in keys):
						if w not in words: words[w] = get_next_word()
						w = words[w]
					l2 += w
					w = ""
				if lp and ispace(c): continue
				lp = ispace(c)
				if c == '/' and l2 and l2[-1] == '/':
					l2 = l2[:-1]
					lp = True
					break
				l2 += c
		print(l2, end='')
	except EOFError:
		break