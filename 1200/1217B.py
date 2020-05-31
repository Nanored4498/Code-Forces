t = int(input())
for _ in range(t):
	n, x = map(int, input().split())
	d, h = [None]*n, [None]*n
	md = 0
	mr = 0
	for i in range(n):
		d[i], h[i] = map(int, input().split())
		md = max(md, d[i])
		mr = max(mr, d[i]-h[i])
	x -= md
	if x <= 0: print(1)
	elif mr <= 0: print(-1)
	else: print(1 + (x+mr-1)//mr)