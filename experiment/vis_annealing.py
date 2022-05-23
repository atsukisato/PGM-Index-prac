import matplotlib.pyplot as plt

datas = []
with open('annealing.txt') as f:
    for l in f:
        list = l.split()
        datas.append(list)


with open('annealing2.txt', mode='w') as f:
    for data in datas:
        st = ''
        for i in range(0, len(data), len(data)//100):
            st += str(data[i]) + ','
        st += "\n"
        f.write(st)
