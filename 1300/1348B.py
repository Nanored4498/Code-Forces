t = int(input())

for _ in range(t):
	n, k = map(int, input().split())
	a = list(map(int, input().split()))
	s = set(a)
	if len(s) > k:
		print(-1)
		continue
	s = [str(i) for i in s]
	s += [s[0]] * (k - len(s))
	print(k*n)
	res = (" ".join(s) + " ") * n
	print(res[:-1])