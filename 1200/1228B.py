MOD = 1000000007

h, w = map(int, input().split())
r = list(map(int, input().split()))
c = list(map(int, input().split()))
m = [[None]*w for _ in range(h)]
for i in range(h):
	for j in range(r[i]):
		m[i][j] = True
	if r[i] < w: m[i][r[i]] = False
for j in range(w):
	for i in range(c[j]):
		if m[i][j] == False:
			print(0)
			exit(0)
		m[i][j] = True
	if c[j] < h:
		if m[c[j]][j] == True:
			print(0)
			exit(0)
		m[c[j]][j] = False
res = 1
for i in range(h):
	for j in range(w):
		if m[i][j] == None:
			res = (res*2) % MOD
print(res)