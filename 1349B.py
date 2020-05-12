t = int(input())
for _ in range(t):
	n, k = map(int, input().split())
	a = list(map(int, input().split()))
	has = False
	prop = n==1
	for i in range(n):
		if a[i] == k: has = True
		if a[i] >= k and i > 0:
			if a[i-1] >= k: prop = True
			if i > 1 and a[i-2] >= k: prop = True
	print("yes" if has and prop else "no")