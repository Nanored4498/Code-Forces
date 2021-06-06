t = int(input())
for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	m = {}
	ans = 0
	for i in range(n):
		if a[i] not in m: m[a[i]] = 0
		ans += m[a[i]] * (n-i)
		m[a[i]] += i+1
	print(ans)
