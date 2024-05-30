for _ in range(78):
	x = input().split()
	if x[1] == "Accepted":
		x = x[6].replace('.', ',')
		# x = x[2][1:]
	else:
		x = 0
		# x = 10**4
	print(x)