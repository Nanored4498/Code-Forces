t = int(input())
for _ in range(t):
	a, b, q = map(int, input().split())
	p = a*b
	count = [0]
	for x in range(p):
		if (x % a) % b != (x % b) % a:
			count.append(count[-1]+1)
		else:
			count.append(count[-1])
	res = []
	for _ in range(q):
		l, r = map(int, input().split())
		sol = count[r%p+1] - count[l%p] + count[-1] * (r // p - l // p)
		res.append(sol)

	print(" ".join(str(r) for r in res))