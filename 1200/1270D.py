from sys import stdout

n, k = map(int, input().split())
a = {}
for i in range(1, k+2):
	q = "?"
	for j in range(1, k+2):
		if j != i: q += " " + str(j)
	print(q)
	stdout.flush()
	p, ap = map(int, input().split())
	if ap not in a: a[ap] = 0
	a[ap] += 1

ma = max(a.keys())
m = a[ma]
print("!", m)