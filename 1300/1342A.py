t = int(input())

for _ in range(t):
	x, y = map(int, input().split())
	a, b = map(int, input().split())
	res = a * abs(x-y) + min(2*a, b) * min(abs(x), abs(y))
	print(res)