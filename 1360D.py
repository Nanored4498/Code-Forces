n, x = map(int, input().split())
d = list(map(int, input().split()))
m, i = 0, 0
m2, i2 = 0, x
s = 0
while i2 > d[m2]:
	i2 -= d[m2]
	s += (d[m2]*(d[m2]+1)) // 2
	m2 += 1
s += (i2*(i2+1)) // 2
res = s
while m < n:
	a = min(d[m2]-i2, d[m]-i)
	s += a * (i2-i)
	res = max(res, s)
	i += a
	i2 += a
	if i >= d[m]:
		i = 0
		m += 1
	if i2 >= d[m2]:
		i2 = 0
		m2 = (m2+1) % n
print(res)