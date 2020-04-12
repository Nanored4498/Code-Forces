n = int(input())
G = [[] for _ in range(n)]
for i in range(n):
	l = input()
	for j in range(n):
		if l[j] == '1':
			G[i].append(j)

ds = [[-1]*n for _ in range(n)]
for i in range(n):
	ds[i][i] = 0
	l = [i]
	while len(l) > 0:
		l2 = []
		for j in l:
			for k in G[j]:
				if ds[i][k] == -1:
					ds[i][k] = ds[i][j]+1
					l2.append(k)
		l = l2

m = int(input())
p = list(map(int, input().split()))
res = 2
vs = [str(p[0])]
i, j = 0, 1
while j < m:
	if ds[p[i]-1][p[j]-1] < j-i:
		res += 1
		i = j-1
		vs.append(str(p[j-1]))
	j += 1
vs.append(str(p[-1]))
print(res)
print(" ".join(vs))