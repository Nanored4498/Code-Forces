T = int(input())
for _ in range(T):
	s, i, e = map(int, input().split())
	a = max(0, (i+e-s)//2)
	while s+a <= i+e-a: a += 1
	print(max(0, e-a+1))