n, l, r = map(int, input().split())
a, b = n-l+1, (n-r+1) * 2**(r-1)
for i in range(1, l):
	a += 2 ** i
for i in range(r-1):
	b += 2 ** i
print(a, b)