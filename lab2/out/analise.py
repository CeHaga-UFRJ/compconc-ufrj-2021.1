minMult = []
minEnd = []
minInit = []
for dim in (500,1000,2000):
  for nthreads in (1,2,4):
    minInitValue = 1000
    minMultValue = 1000
    minEndValue = 1000
    for i in range(1,6):
      fileName = f'out_{i}_{dim}_{nthreads}.txt'
      with open(fileName) as f:
        lines = f.readlines()
        lines = [line.split() for line in lines]
        if(lines[2][-1] != 'certo'):
          print(f'Erro de saida no arquivo {fileName}\n')
          exit()
        minInitValue = min(minInitValue, float(lines[0][-2]))
        minMultValue = min(minMultValue, float(lines[1][-2]))
        minEndValue = min(minEndValue, float(lines[3][-2]))
    minMult.append(minMultValue)
    minEnd.append(minEndValue)
    minInit.append(minInitValue)
print(minInit)
print(minMult)
print(minEnd)
