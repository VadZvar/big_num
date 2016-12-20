p = 223
g = 160
a = 166
tupx = 0 % (p - 1)
tupx1 = 1
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 0 % (p - 1)
tupx1 = 1
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 1 % (p - 1)
tupx1 = 2
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 1 % (p - 1)
tupx1 = 2
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 2 % (p - 1)
tupx1 = 3
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 3 % (p - 1)
tupx = 2 % (p - 1)
tupx1 = 3
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 3 % (p - 1)
tupx1 = 4
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 4 % (p - 1)
tupx = 3 % (p - 1)
tupx = 4 % (p - 1)
tupx = 4 % (p - 1)
tupx1 = 8
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 3 % (p - 1)
tupx1 = 4
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 8 % (p - 1)
tupx1 = 9
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 9 % (p - 1)
tupx1 = 10
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 4 % (p - 1)
tupx = 10 % (p - 1)
tupx1 = 20
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 20 % (p - 1)
tupx = 4 % (p - 1)
tupx = 20 % (p - 1)
tupx = 20 % (p - 1)
tupx = 4 % (p - 1)
tupx1 = 8
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 20 % (p - 1)
tupx = 20 % (p - 1)
tupx1 = 21
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 8 % (p - 1)
tupx1 = 9
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 21 % (p - 1)
tupx1 = 42
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 42 % (p - 1)
tupx = 9 % (p - 1)
tupx1 = 10
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 42 % (p - 1)
tupx = 42 % (p - 1)
tupx1 = 84
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 10 % (p - 1)
tupx1 = 20
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 84 % (p - 1)
tupx1 = 85
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 85 % (p - 1)
tupx1 = 170
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 20 % (p - 1)
tupx = 170 % (p - 1)
tupx = 170 % (p - 1)
tupx = 20 % (p - 1)
tupx = 170 % (p - 1)
tupx1 = 171
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 171 % (p - 1)
tupx1 = 120
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 20 % (p - 1)
tupx = 120 % (p - 1)
tupx1 = 121
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 121 % (p - 1)
tupx1 = 20
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 20 % (p - 1)
tupx = 20 % (p - 1)
tupx1 = 21
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 21 % (p - 1)
tupx1 = 22
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 20 % (p - 1)
tupx1 = 21
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 22 % (p - 1)
tupx1 = 23
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 23 % (p - 1)
tupx1 = 24
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 21 % (p - 1)
tupx1 = 42
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 24 % (p - 1)
tupx = 24 % (p - 1)
tupx1 = 25
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 42 % (p - 1)
tupx = 25 % (p - 1)
tupx = 25 % (p - 1)
tupx = 42 % (p - 1)
tupx = 25 % (p - 1)
tupx1 = 50
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 50 % (p - 1)
tupx1 = 51
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 42 % (p - 1)
tupx1 = 84
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 51 % (p - 1)
tupx1 = 52
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 52 % (p - 1)
tupx1 = 104
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 84 % (p - 1)
tupx1 = 85
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 104 % (p - 1)
tupx = 104 % (p - 1)
tupx = 85 % (p - 1)
tupx1 = 170
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 104 % (p - 1)
tupx = 104 % (p - 1)
tupx = 170 % (p - 1)
tupx = 104 % (p - 1)
tupx1 = 105
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 105 % (p - 1)
tupx1 = 210
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 170 % (p - 1)
tupx = 210 % (p - 1)
tupx = 210 % (p - 1)
tupx = 170 % (p - 1)
tupx1 = 171
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 210 % (p - 1)
tupx1 = 198
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 198 % (p - 1)
tupx1 = 199
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 171 % (p - 1)
tupx1 = 120
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 199 % (p - 1)
tupx1 = 176
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 176 % (p - 1)
tupx = 120 % (p - 1)
tupx1 = 121
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 176 % (p - 1)
tupx = 176 % (p - 1)
tupx1 = 177
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 121 % (p - 1)
tupx1 = 20
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 177 % (p - 1)
tupx1 = 132
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 132 % (p - 1)
tupx1 = 133
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 20 % (p - 1)
tupx1 = 21
assert tupx1 == (tupx + 1) % (p - 1)
tupx = 133 % (p - 1)
tupx1 = 44
assert tupx1 == (tupx << 1) % (p - 1)
tupx = 44 % (p - 1)
tupx1 = 45
assert tupx1 == (tupx + 1) % (p - 1)
sb = 21
sy = 108
fb = 45
fy = 168
n = 222
r = 24
d = 16
r = 166
correct
