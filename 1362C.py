t = int(input())
for _ in range(t):
	n = int(input())
	p2, add = 1, 1
	ans = 0
	while p2 <= n:
		ans += (1 + (n-p2) // (2*p2)) * add
		p2 *= 2
		add += 1
	print(ans)