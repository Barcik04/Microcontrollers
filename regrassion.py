import pandas as pd
import matplotlib.pyplot as plt

df = pd.DataFrame()
df['x'] = [1, 2, 3, 4, 5]
df['y'] = [4, 6, 9, 11, 18]
print(df)

plt.scatter(df['x'], df['y'], label='Wartości Niezależne')
plt.xlabel('Wartości x')
plt.ylabel('Wartości y')
plt.legend()
plt.show()




def srednia(zbior):
    return sum(zbior) / len(zbior)

Mean_x = srednia(df['x'])
Mean_y = srednia(df['y'])

print("Mean x: ", Mean_x)
print("Mean y: ", Mean_y)


import numpy as np

srednia_x = np.mean(df['x'])
srednia_y = np.mean(df['y'])


from math import sqrt

def odchylenie(zbior):
    srednia = sum(zbior) / len(zbior)
    suma = 0
    for x in zbior:
        suma += (x - srednia) ** 2
    return sqrt(suma / (len(zbior) - 1))

Sx = odchylenie(df['x'])
Sy = odchylenie(df['y'])

print("Standard deviation x: ", Sx)
print("Standard deviation y: ", Sy)


Sx = np.std(df['x'])
Sy = np.std(df['y'])

print("Standard deviation of x and y: ", Sx, Sy)





n = len(df['x'])

pearson = pd.DataFrame(df['x'])
pearson['y2'] = df['y'] * df['y']
pearson['xy'] = df['x'] * df['y']
pearson['x2'] = df['x'] * df['x']
pearson['y'] = df['y'] * df['y']
pearson.loc['sum'] = pearson.sum()

print("n = ", n)
print()
print(pearson)



def wsp_korelacji_pearsona(n, suma_xy, suma_x, suma_y, suma_x2, suma_y2):
    return (n * suma_xy - suma_x * suma_y) / (((n * suma_x2 - suma_x ** 2) * (n * suma_y2 - suma_y ** 2)) ** 0.5)

pearson_result = wsp_korelacji_pearsona(n, 177, 15, 48, 55, 578)
print("r = ", pearson_result)


import scipy

pearson_scipy = scipy.stats.pearsonr(df['x'], df['y'])
print(pearson_scipy[0])


print("Mean x: ", Mean_x)
print("Mean y: ", Mean_y, "\n")
print("Standard deviation x: ", Sx)
print("Standard deviation y: ", Sy, "\n")
print("Pearson correlation coefficient = ", pearson_result)



b = pearson_result * (Sy / Sx)
a = Mean_y - b * Mean_x

print("b = ", b)
print("a = ", a)

def linia_regresji(x):
    return (b * x) + a

x = np.linspace(0, 5, 1000)
plt.scatter(df['x'], df['y'], label='Wartości Niezależne')
plt.plot(x, linia_regresji(x), 'r', label='Linia Regresji')
plt.xlabel('Wartości X')
plt.ylabel('Wartości Y')
plt.legend()
plt.show()

#### ZMIENILEM BO NOWSZE PANDAS DataFrame.append() juz nie istnieje
new_row = pd.DataFrame({'x': [6], 'y': [np.nan]})
df = pd.concat([df, new_row], ignore_index=True)


def predict_y(x, b, a):
    return b * x + a

df.at[5, 'y'] = predict_y(df['x'][5], b, a)
print(df)



print("Dla x=6, y =", predict_y(6, b, a))
print("Dla x=7, y =", predict_y(7, b, a))
print("Dla x=8, y =", predict_y(8, b, a))
