t = int(input())
for _ in range(t):
	n, m = map(int, input().split())
	a = [input() for _ in range(n)]
	found = False
	for i in range(m):
		for c in range(26):
			sol = list(a[0])
			sol[i] = chr(c + ord('a'))
			sol = "".join(sol)
			found = True
			for j in range(n):
				d = 0
				for k in range(m):
					if sol[k] != a[j][k]: d += 1
				if d > 1:
					found = False
					break
			if found: break
		if found: break
	if found: print(sol)
	else: print(-1)