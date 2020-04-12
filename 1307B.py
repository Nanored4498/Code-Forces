t = int(input())

for _ in range(t):
	n, x = map(int, input().split())
	a = list(map(int, input().split()))
	ma = max(a)
	if x in a:
		print(1)
	elif ma > x:
		print(2)
	elif x % ma == 0:
		print(x // ma)
	else:
		print(x // ma + 1)