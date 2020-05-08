t = int(input())
for tt in range(t):
	a, b, p = map(int, input().split())
	s = input()
	cs = {'A':a, 'B':b}
	c = 0
	i = len(s)-1
	while i > 0 and c+cs[s[i-1]] <= p:
		c += cs[s[i-1]]
		i -= 1
		while i > 0 and s[i-1] == s[i]:
			i -= 1
	print(i+1)