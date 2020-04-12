l, r = map(int, input().split())
for i in range(l, r+1):
	ds = set()
	j = i
	good = True
	while j > 0:
		d = j % 10
		j //= 10
		if d in ds:
			good = False
			break
		else: ds.add(d)
	if good:
		print(i)
		exit(0)
print(-1)