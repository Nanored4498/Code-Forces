t = int(input())
for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	a.sort()
	res = []
	for i in range(n):
		if i % 2 == 0:
			res.append(str(a[i//2]))
		else:
			res.append(str(a[n-1-i//2]))
	print(" ".join(res[n-1-i] for i in range(n)))