import math

t = int(input())
for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	T = 0
	prev = a[0]
	for i in range(1, n):
		diff = a[i]-prev
		if diff >= 0:
			prev = a[i]
		else:
			T = max(T, int(1 + math.log2(-diff)))
	print(T)