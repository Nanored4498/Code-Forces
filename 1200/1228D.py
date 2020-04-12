n, m = map(int, input().split())
G = [[] for _ in range(n)]
for i in range(m):
	a, b = map(int, input().split())
	a -= 1
	b -= 1
	G[a].append(b)
	G[b].append(a)

if len(G[0]) < 2:
	print(-1)
	exit(0)

res = [1]*n
for a in G[0]:
	res[a] = 2
a2 = G[0][0]
for b in G[a2]:
	if res[b] == 2: res[b] = 3
sizes = [n-len(G[0]), n-len(G[a2]), len(G[0])+len(G[a2])-n]
if 0 in sizes:
	print(-1)
	exit(0)

for i in range(n):
	g = res[i]
	s = sizes[g-1]
	if len(G[i]) != n-s:
		print(-1)
		exit(0)
	for j in G[i]:
		if res[j] == g:
			print(-1)
			exit(0)

print(*res)