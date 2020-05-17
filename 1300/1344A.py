t = int(input())

for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	b = [False]*n
	good = True
	for i in range(n):
		k = (i + a[i]) % n
		if b[k]:
			good = False
			break
		b[k] = True
	print("YES" if good else "NO")
