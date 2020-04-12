n = int(input())
ps = []
for _ in range(n):
	x, y = map(int, input().split())
	ps.append((x, y))
ds = {}
same = [{} for _ in range(n)]
def ads(i, d, j):
	if d not in same[i]: same[i][d] = []
	same[i][d].append(j)
for i in range(n):
	x, y = ps[i]
	d = []
	for j in range(i+1,n):
		a, b = ps[j]
		dis = (x-a)**2 + (y-b)**2
		if dis not in ds: ds[dis] = []
		else:
			ads(i, dis, j)
			ads(j, dis, i)
			if len(ds[dis]) == 1:
				k, l = ds[dis][0]
				ads(k, dis, l)
				ads(l, dis, k)
		ds[dis].append((i, j))

clus = [-1]*n
clus[0] = 0
choice = {d:None for d in ds}
def f(i):
	if i == n: return 1 in clus
	for c in range(2):
		ok = True
		clus[i] = c
		cm = []
		for d in same[i]:
			for j in same[i][d]:
				if clus[j] >= 0:
					if choice[d] == True and clus[j] != c:
						ok = False
						break
					if choice[d] == False and clus[j] == c:
						ok = False
						break
					if choice[d] == None:
						choice[d] = c == clus[j]
						cm.append(d)
			if not ok: break
		if ok and f(i+1): return True		
		clus[i] = -1
		for d in cm:
			choice[d] = None
	return False

f(1)
a = [str(i+1) for i in range(n) if clus[i] == 0]
print(len(a))
print(" ".join(a))
# print(choice)
# print(same)