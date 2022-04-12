import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif']=['SimHei']
plt.rcParams['axes.unicode_minus'] = False

x = [64,128,256,512]
y1 = [0.006922924,0.10478031,1.18446744073665,24.1844674407353]
y2 = [0.002224498,0.028168328,0.184467440737058,6.084206434]

y3 = []

for i in range(len(x)):
    y3.append(y1[i]/y2[i])


plt.plot(x,y3)
plt.legend()
plt.xlabel('规模n')
plt.ylabel('加速比')
plt.title("加速比随问题规模变化图")
plt.savefig(r"D:\AcademicLife\bingxing\SIMD\特殊高斯消去法\规模\jiasubi.jpg")
plt.show()
