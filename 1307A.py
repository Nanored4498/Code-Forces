t = int(input())

for _ in range(t):
	n, d = map(int, input().split())
	a = list(map(int, input().split()))
	res = a[0]
	i = 1
	while i < n and a[i] == 0: i+=1
	while i < n and d >= i:
		res += 1
		d -= i
		a[i] -= 1
		while i < n and a[i] == 0: i+=1
	print(res)