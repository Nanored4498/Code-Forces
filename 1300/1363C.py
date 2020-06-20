t = int(input())
for _ in range(t):
	n, x = map(int, input().split())
	G = [[] for _ in range(n)]
	for _ in range(n-1):
		u, v = map(int, input().split())
		G[u-1].append(v-1)
		G[v-1].append(u-1)
	if len(G[x-1]) <= 1: print("Ayush")
	elif n%2 == 0: print("Ayush")
	else: print("Ashish")