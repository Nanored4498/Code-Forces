t = int(input())

for _ in range(t):
	n, k = map(int, input().split())
	s = "".join(sorted(input()))
	if s[0] != s[k-1] or k == n:
		print(s[k-1])
	elif s[k] == s[-1]:
		print(s[0] + s[k] * ((n-1)//k))
	else:
		print(s[k-1:])
