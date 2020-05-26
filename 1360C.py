t = int(input())
for _ in range(t):
	x1, y1, x2, y2 = map(int, input().split())
	dx = x2-x1
	dy = y2-y1
	mi, ma = sorted([dx, dy])
	res = mi * (mi+1) + mi * (ma-mi-1) + 1
	print(res)
