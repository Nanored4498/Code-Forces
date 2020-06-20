t = int(input())
for _ in range(t):
	s = input()
	n = len(s)
	res = n
	n0 = [0]
	for i in range(n):
		if s[i] == '0': n0.append(n0[-1]+1)
		else: n0.append(n0[-1])
	for i in range(1, n+1):
		res = min(res, i-n0[i] + n0[n]-n0[i])
		res = min(res, n0[i] + n-i-(n0[n]-n0[i]))
	print(res)