import csv
import sys
import numpy as np
import matplotlib.pyplot as plt


def main():
    if len(sys.argv) != 2:
        print(f"Usage python3 plot_csv.py foo.csv")
        exit(1)

    csv_path = sys.argv[1]
    print(f"Plotting {csv_path}")

    with open(csv_path, "r") as file:
        raw_data = file.read()

    lines = raw_data.strip("\n").split("\n")
    lines = [line.strip(",").split(",") for line in lines]
    data = []
    for line in lines:
        data.append([float(cell) for cell in line if cell])

    if len(lines) not in [1, 2]:
        print("Error: csv format Y or X\\nY")

    if len(lines) == 1:
        print("plotting input line as Y")
        values = np.array(data[0])
        indices = np.arange(0, len(values))
    else:
        print("plotting input lines as X Y")
        indices = np.array(data[0])
        values = np.array(data[1])

    fig, ax = plt.subplots()
    ax.plot(indices, values)
    plt.show()


if __name__ == "__main__":
    main()
