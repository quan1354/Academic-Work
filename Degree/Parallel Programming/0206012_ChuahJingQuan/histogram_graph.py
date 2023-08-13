import matplotlib.pyplot as plt
import numpy as np
import os

# load folder
PATH = 'Tutorial 1/temp/'
EXTENSION_TXT = '.txt'
PATH_GRAPH = PATH + 'hist_graph/'
EXTENSION_PNG = '.png'

bin_count = 5

f = open(PATH + 'month_list.txt', 'r')
read_month = f.readlines()

f = open(PATH + 'year.txt', 'r')
year = f.readline().rstrip()

execution_type = ['Serial', 'Parallel']
month_list = ['Overall']

for no, month in enumerate(read_month):
    month_list.append(month.strip())

print('Generating graph')

for no, month in enumerate(month_list):
    for type in execution_type:
        fileName = PATH + type + '_' + month.strip() + '_' + year + EXTENSION_TXT
        print(fileName)

        frequency = np.loadtxt(fileName, dtype=float)
        print(frequency)
        
        plt.title('Histogram for ' + month.strip() + '(' + type + ')');
        plt.xlabel('Range of Temperature')
        plt.ylabel('Frequency')
        plt.hist(frequency, bin_count, edgecolor='black', histtype='bar')
        
        plt.savefig(PATH_GRAPH + type + '_' + month.strip() + EXTENSION_PNG);
        plt.close()
        
print('Graph generated in ' + PATH_GRAPH)
os.system('pause')