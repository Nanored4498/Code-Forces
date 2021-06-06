t = int(input())
for _ in range(t):
	n = int(input())
	p = 1
	while p <= n: p <<= 1
	print((p >> 1)-1)
	