t = int(input())
for _ in range(t):
	n = int(input())
	m = [input() for _ in range(n)]
	res = "YES"
	for i in range(n-1):
		for j in range(n-1):
			if m[i][j] == '1' and m[i+1][j] == '0' and m[i][j+1] == '0':
				res = "NO"
				break
		if res == "NO":
			break
	print(res)