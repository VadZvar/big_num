import sys

s = sys.argv[1]
l = map(lambda x:x[0], filter(lambda x: x[1] == '1', [(i, s[i]) for i in xrange(len(s))]))

o = ""
for i in l:
	o += "x^" + str(i) + " + "

print o[:-3]
