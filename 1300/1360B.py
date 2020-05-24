t = int(input())
for _ in range(t):
	n = int(input())
	s = list(map(int, input().split()))
	s.sort()
	res = s[1]-s[0]
	for i in range(1, n):
		res = min(res, s[i]-s[i-1])
	print(res)