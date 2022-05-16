import matplotlib.pyplot as plt

shift = [-x for x in range(720, 30720, 1000)]
detected = [291,
291,
291,
291,
291,
767,
1767,
2767,
3767,
4767,
5767,
6767,
7767,
8767,
9767,
10767,
11767,
12767,
13767,
14767,
15767,
16767,
17767,
18767,
19767,
20767,
21767,
22767,
23767,
24730]

print(shift)
print(detected)

incorrect = [(n+m)*(-1) for m,n in zip(shift,detected)]
print(incorrect)

plt.grid()
plt.plot(shift, incorrect)
plt.show()