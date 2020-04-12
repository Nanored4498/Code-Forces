s = input()
ls = len(s)
if s == 0:
	print(0)
else:
	res = (ls+1) // 2
	b = ls % 2 == 1
	for i in range(1, ls):
		if s[i] == '1':
			b = False
			break
	if b: res -= 1
	print(res)