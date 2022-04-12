import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif']=['SimHei']
plt.rcParams['axes.unicode_minus'] = False

x = [64,128,256,512,1024]
y1 = [0.00050412,0.00400503,0.03156792,0.25638255,3.184467441]
y2 = [0.00038756,0.00317668,0.02571902,0.21013609,2.184467441]

y3 = []


plt.plot(x,y1,label='串行算法')
plt.plot(x,y2,label='并行算法')
plt.legend()
plt.xlabel('规模n')
plt.ylabel('time(s)')
plt.title("运行实际随问题规模变化图")
plt.savefig(r"D:\AcademicLife\bingxing\SIMD\neon\规模\shijian.jpg")
plt.show()
