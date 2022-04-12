import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

plt.rcParams['font.sans-serif']=['SimHei']
plt.rcParams['axes.unicode_minus'] = False


 
def draw_bar(labels,quants):
    width = 0.4
    ind = np.linspace(0,4,4)
    # make a square figure
    fig = plt.figure(1)
    ax  = fig.add_subplot(111)
    # Bar Plot
    ax.bar(ind-width/2,quants,width,color='green')
    # Set the ticks on x-axis
    ax.set_xticks(ind)
    ax.set_xticklabels(labels)
    # labels
    ax.set_xlabel('并行程度')
    ax.set_ylabel('运行时间')
    # title
    ax.set_title('运行时间与并行程度关系图', bbox={'facecolor':'0.8', 'pad':5})
    plt.grid(True)
    plt.savefig(r"D:\AcademicLife\bingxing\SIMD\neon\优化位置\weizhi.jpg")
    plt.show()
    plt.close()
 
labels   = ['串行', '优化1', '优化2','优化3']
 
quants   = [3.18446744072802,2.16224836,1.75995998,1.70947478]
 
draw_bar(labels,quants)


