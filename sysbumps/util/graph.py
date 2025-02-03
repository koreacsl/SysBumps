import matplotlib.pyplot as plt
import sys

fp = open("res", 'r')
lines = fp.readlines()

base_addr = 0xfffffe000f004000
slide = 0x4000

offset = list()
time = list()

end = int((int(lines[-1].strip(),16) - base_addr)/slide + 6000)

for idx in range(end) :
    line = lines[idx]
    line = line.strip("\n")
    token = line.split()
    offset.append(idx)
    time.append(int(token[1]))

plt.figure(figsize=(12,5))
plt.plot(offset, time)
axes = plt.gca()
plt.ylim([0,3000])
plt.xlim([0, len(offset)])
plt.xticks([0, len(offset)])

plt.ticklabel_format(axis='x',useOffset=False, style='plain')
plt.xlabel("offset")
plt.ylabel("Probing cycle")

plt.show()
