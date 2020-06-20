t = int(input())
for _ in range(t):
	a, b = map(int, input().split())
	x, y = 0, 0
	while not a&(1<<x): x += 1
	while not b&(1<<y): y += 1
	a >>= x
	b >>= y
	if a == b: print((abs(x-y)+2)//3)
	else: print(-1)