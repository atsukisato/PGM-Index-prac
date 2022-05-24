import numpy as np
import matplotlib.pyplot as plt

file_names = ["uniform", "lognormal", "expo"]

for file_name in file_names:
    source_file_name = file_name + ".txt"
    output_file_name = file_name + ".png"

    with open(source_file_name) as f:
        l = f.readline()
        li = list(map(float, l.split(' ')))

    x = np.array(li)
    y = np.arange(1, len(li)+1, 1)

    plt.plot(x, y)
    
    plt.savefig(output_file_name)   # プロットしたグラフをファイルsin.pngに保存する
    plt.show()

