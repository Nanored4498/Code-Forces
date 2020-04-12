import math

n = int(input())
ns = {}
a = list(map(int, input().split()))
for v in a:
	if v == 0: continue
	if v in ns:
		ns[v] += 1
		if ns[v] >= 3:
			print(3)
			exit(0)
	else: ns[v] = 1
a = [v for v in ns]
n = len(a)
MAX_A = 10 ** 18
n_bits = math.ceil(math.log(MAX_A, 2)) + 1
if n > n_bits*3:
	print(3)
	exit(0)

comp = [[] for _ in range(n_bits)]
G = {}

def add(v):
	G[v] = set()
	v2, i = v, 0
	while v2 != 0:
		if v2 % 2 == 1: comp[i].append(v)
		v2 //= 2
		i += 1

for v in a:
	add(v)
for c in comp:
	if len(c) >= 3:
		print(3)
		exit(0)
	elif len(c) == 2:
		v, w = c
		G[v].add(w)
		G[w].add(v)

for v in a:
	if ns[v] == 2 and len(G[v]) > 0:
		print(3)
		exit(0)

res = -1
for u in a:
	level = {v:-1 for v in a}
	level[u] = 0
	l = [u]
	i = 0
	while len(l) > 0 and (res < 0 or 2*i < res):
		l2 = []
		for v in l:
			for w in G[v]:
				if level[w] == -1:
					l2.append(w)
					level[w] = i+1
				elif level[w] >= i:
					res = min(res, i+1+level[w]) if res > 0 else i+1+level[w]
		l = l2
		i += 1
print(res)