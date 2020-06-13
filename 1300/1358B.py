t = int(input())
for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	a.sort()
	res = 1
	for i in range(n):
		if 1+i >= a[i]:
			res = max(res, i+2)
	print(res)