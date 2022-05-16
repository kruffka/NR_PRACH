import matplotlib.pyplot as plt

shift = [x for x in range(280, 33280, 1000)]
detected = [280,
1280,
2280,
3280,
4280,
5280,
6280,
7280,
8280,
9280,
10280,
11280,
12280,
13280,
14280,
15280,
16280,
17280,
18280,
19280,
20280,
21280,
22280,
23280,
24280,
25280,
25465,
25465,
25465,
25476,
26348,
26625,
26625]

print(shift)
print(detected)

incorrect = [m - n for m, n in zip(shift,detected)]
print(incorrect)


plt.plot(shift, incorrect)
plt.grid()
plt.show()