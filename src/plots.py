import pandas as pd
import matplotlib.pyplot as plt

files = ["../data/random_data.csv", "../data/reverse_data.csv", "../data/almost_data.csv"]
titles = ["Случайные значения", "Обратные неповозрастанию значения", "Частично отсортированные значения"]

for file, title in zip(files, titles):
    df = pd.read_csv(file)

    plt.figure(figsize=(10, 6))
    plt.plot(df["Size"], df["QuickSort"], label="QuickSort")
    plt.plot(df["Size"], df["IntroSort"], label="IntroSort")
    plt.xlabel("Размер массива")
    plt.ylabel("Время (микросекунды)")
    plt.title(f"{title}")
    plt.legend()
    plt.grid(True)
    plt.show()