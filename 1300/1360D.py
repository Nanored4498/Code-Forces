t = int(input())
for _ in range(t):
	n, k = map(int, input().split())
	n2 = int(n**.5+1)
	res = n
	for i in range(1, n2):
		if n % i == 0:
			j = n // i
			if j <= k: res = min(res, i)
			if i <= k: res = min(res, j)
	print(res)