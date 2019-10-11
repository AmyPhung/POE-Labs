f = open("raw_data/raw_data.txt", "r")
data_str = f.read()
# data_list = data_str.split("\n")
# data_list2 = [i[15:] for i in data_list]

from io import StringIO
import csv

f2 = StringIO(data_str)
reader = csv.reader(f, delimiter = ',')
# for row in reader:
#     print('\t'.join(row))

for row in reader:
    print(row)