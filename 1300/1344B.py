n, m = map(int, input().split())
r0, r1 = [None]*n, [None]*n
c0, c1 = [None]*m, [None]*m
col = [False]*m
bad = False
nr, nc = 0, 0
for i in range(n):
	r = input()
	for j in range(m):
		if r[j] == '#':
			if r0[i] is None: r0[i], r1[i], nr = j, j, nr+1
			elif r1[i] == j-1: r1[i] = j
			else: bad = True
			if c0[j] is None: c0[j], c1[j], nc = i, i, nc+1
			elif c1[j] == i-1: c1[j] = i
			else: bad = True
if bad or (nr < n) != (nc < m):
	print(-1)
	exit(0)

res = 0
for i in range(n):
	if r0[i] is None: continue
	if i == 0 or r0[i-1] is None:
		res += 1
	elif r1[i] < r0[i-1] or r0[i] > r1[i-1]:
		res += 1
print(res)