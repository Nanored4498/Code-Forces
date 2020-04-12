t = int(input())

for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	no = True
	for j in range(1, n):
		if abs(a[j]-a[j-1]) > 1:
			print("YES")
			print(j, j+1)
			no = False
			break
	if no: print("NO")