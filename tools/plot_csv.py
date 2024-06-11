import csv
import numpy as np
import matplotlib.pyplot as plt
import argparse


def main():
    args = fetch_args()
    indices, values = fetch_data(args.csv_path)

    print(f"Plotting {args.csv_path}")
    plot_data(indices, values, args)


def fetch_args():
    parser = argparse.ArgumentParser(description="plot csv to monitor your data")
    parser.add_argument(
        "csv_path",
        metavar="csv path",
        type=str,
        help="path to the csv file you want to plot",
    )
    parser.add_argument(
        "--logx", help="set x scale to logarithmic", action="store_true"
    )
    parser.add_argument(
        "--logy", help="set x scale to logarithmic", action="store_true"
    )

    return parser.parse_args()


def fetch_data(csv_path):
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

    return indices, values


def plot_data(indices, values, args):
    fig, ax = plt.subplots()

    if args.logx:
        ax.set_xscale("log")

    ax.plot(indices, values)
    plt.show()


if __name__ == "__main__":
    main()
