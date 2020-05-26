t = int(input())
for _ in range(t):
	s = input()
	l = [-len(s)-1]*3
	res = len(s)
	for i in range(len(s)):
		c = ord(s[i]) - ord('1')
		l[c] = i
		le = i - min(l) + 1
		res = min(res, le)
	if min(l) < 0: res = 0
	print(res)