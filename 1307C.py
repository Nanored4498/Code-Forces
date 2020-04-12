s = input()
co = [0]*26
co2 = [0]*(26*26)
for a in s:
	a = ord(a)-ord('a')
	for b in range(26):
		r = a+26*b
		co2[r] += co[b]
	co[a] += 1
print(max(max(co), max(co2)))