t = int(input())
for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	ns = [0]*100
	p = [0]*2
	for ai in a:
		ns[ai-1] += 1
		p[(ai-1)%2] += 1
	if p[0]%2 == 0:
		res = True
	else:
		res = False
		for i in range(99):
			if ns[i] > 0 and ns[i+1] > 0:
				res = True
				break
	print("YES" if res else "NO")