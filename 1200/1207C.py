T = int(input())
for _ in range(T):
	n, a, b = map(int, input().split())
	s = input()
	res = 0
	i = 0
	while i < n and s[i] == '0':
		res += a + b
		i += 1
	if i < n:
		res += 2*a
		while s[n-1] == '0':
			res += a + b
			n -= 1
		res += (n-i+1)*2*b + (n-i)*a
		nz = 0
		while i < n:
			if s[i] == '1':
				if nz > 1 and 2*a-(nz-1)*b < 0:
					res += 2*a-(nz-1)*b
				nz = 0
			else:
				nz += 1
			i += 1
	else:
		res += b
	print(res)