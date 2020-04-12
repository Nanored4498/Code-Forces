import heapq as hq

t = int(input())
for _ in range(t):
	n = int(input())
	b = list(map(int, input().split()))
	sb = sorted([(b[i]-1, i) for i in range(n)]) + [(2*n+1, -1)]
	a = [0]*(2*n)
	pos = []
	ind = 0
	for i in range(2*n):
		if i < sb[ind][0]:
			if len(pos) == 0:
				a = ["-1"]
				break
			p = hq.heappop(pos)
			a[p] = str(i+1)
		else:
			p = 2*sb[ind][1]
			a[p] = str(i+1)
			hq.heappush(pos, p+1)
			ind += 1
	print(" ".join(a))