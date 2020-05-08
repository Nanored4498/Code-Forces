t = int(input())
for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	a.sort()
	res = " ".join([str(a[n-1-i]) for i in range(n)])
	print(res)