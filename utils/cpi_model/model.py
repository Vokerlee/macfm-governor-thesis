from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import matplotlib
import argparse

from matplotlib.gridspec  import GridSpec
from matplotlib.ticker    import MaxNLocator

matplotlib.rcParams['axes.formatter.limits'] = (-5, 4)

MAX_LOCATOR_NUMBER = 10
FIGURE_XSIZE = 9
FIGURE_YSIZE = 6

# BACKGROUND_RGB = '#F5F5F5'
BACKGROUND_RGB = '#FFFFFF'
MAJOR_GRID_RGB = '#919191'

LEGEND_FRAME_ALPHA = 0.95

def set_axis_properties(axes):
    axes.xaxis.set_major_locator(MaxNLocator(MAX_LOCATOR_NUMBER))
    axes.minorticks_on()
    axes.grid(which='major', linewidth=2, color=MAJOR_GRID_RGB)
    axes.grid(which='minor', linestyle=':')

def add_pmu_metrics(df):
    df['cpi'] = df['0x11'] / df['0x08']
    df['ipc'] = df['0x08'] / df['0x11']

    df['l1ipc'] = df['0x01'] / df['0x11']
    df['l1dpc'] = df['0x03'] / df['0x11']
    df['l2pc']  = df['0x17'] / df['0x11']
    df['l3pc']  = df['0x2a'] / df['0x11']

    df['l1ipi'] = df['0x01'] / df['0x08']
    df['l1dpi'] = df['0x03'] / df['0x08']
    df['l2pi']  = df['0x17'] / df['0x08']
    df['l3pi']  = df['0x2a'] / df['0x08']

    return df

def get_dfs_from_file(filename: str):
    df = pd.read_csv(filename)
    df = add_pmu_metrics(df)
    benchmarks = df['benchmark'].unique()
    dfs = dict(tuple(df.groupby('benchmark')))

    return benchmarks, dfs

def get_avg_pmu(df, pmu_field):
    return np.mean(df[pmu_field].to_numpy())

def get_cpi_model_params(l2pi_values, l3pi_values, linear_coeffs, benchmarks):
    assert(len(l2pi_values) == len(l3pi_values))
    assert(len(l2pi_values) == len(linear_coeffs))

    hsv = plt.get_cmap('hsv')
    colors = hsv(linear_coeffs / np.max(linear_coeffs))

    figure = plt.figure(figsize=(FIGURE_XSIZE, FIGURE_YSIZE), facecolor=BACKGROUND_RGB)
    axes = figure.add_subplot(projection='3d')
    # set_axis_properties(axes)

    linear_reg = LinearRegression(fit_intercept=False).fit(
        np.swapaxes(np.array([l2pi_values - l3pi_values, l3pi_values]), 0, 1), linear_coeffs)
    print(np.array(linear_reg.coef_) * 1024 * 1024)
    print(np.array(linear_reg.coef_))

    for i in range(len(benchmarks)):
        axes.scatter(l2pi_values[i] - l3pi_values[i], l3pi_values[i], linear_coeffs[i], label=benchmarks[i])

    X, Y = np.meshgrid(l2pi_values - l3pi_values, l3pi_values)
    axes.plot_wireframe(X, Y, linear_reg.coef_[0] * X + linear_reg.coef_[1] * Y)

    axes.set_ylabel("$npi_{ram}$", fontsize=14)
    axes.set_xlabel("$npi_{L_3}$", fontsize=14)
    axes.set_zlabel("$lat_{gen}$", fontsize=14)

def main(args):
    benchmarks, dfs = get_dfs_from_file(args.file)

    figure = plt.figure(figsize=(FIGURE_XSIZE, FIGURE_YSIZE), facecolor=BACKGROUND_RGB)
    figure.text(0.14, 0.82, "Цвет соответствует значению коэффициента наклона", fontsize=13)
    gs = GridSpec(ncols=1, nrows=1, figure=figure)
    axes = figure.add_subplot(gs[0, 0])
    set_axis_properties(axes)

    linear_coeffs = []
    l3pi_values = []
    l2pi_values = []

    for i in range(len(benchmarks)):
        l3pi_values.append(get_avg_pmu(dfs[benchmarks[i]], "l3pi"))
        l2pi_values.append(get_avg_pmu(dfs[benchmarks[i]], "l2pi"))

    for i in range(len(benchmarks)):
        cpufreq = dfs[benchmarks[i]]['cpufreq'].to_numpy()
        cpi = dfs[benchmarks[i]]['cpi'].to_numpy()

        linear_reg = LinearRegression().fit(cpufreq.reshape(-1, 1), cpi)
        linear_coeffs.append(linear_reg.coef_[0])

    hsv = plt.get_cmap('hsv')
    colors = hsv(0.8 * np.array(linear_coeffs) / np.max(linear_coeffs))

    for i in range(len(benchmarks)):
        bencmark_name = benchmarks[i].split("/")[-1]
        cpufreq = dfs[benchmarks[i]]['cpufreq'].to_numpy() / 1e6

        cpufreq_to_zero = np.linspace(0, np.min(cpufreq), 256)
        cpi = dfs[benchmarks[i]]['cpi'].to_numpy()

        linear_reg = LinearRegression().fit(cpufreq.reshape(-1, 1), cpi)

        axes.plot(cpufreq, cpi, label=f"{bencmark_name}", color=colors[i],
                  marker='o', markersize=6, linestyle='')
        axes.plot(cpufreq.reshape(-1, 1), linear_reg.predict(cpufreq.reshape(-1, 1)),
                  label=f"{bencmark_name} fitted", color=colors[i],
                  marker='*', markersize=6, linestyle='-')
        axes.plot(cpufreq_to_zero, linear_reg.predict(cpufreq_to_zero.reshape(-1, 1)),
                  color=colors[i], marker='', linestyle='--')

    get_cpi_model_params(np.array(l2pi_values), np.array(l3pi_values),
                         np.array(linear_coeffs), np.array(benchmarks))

    axes.set_xlabel('$freq_{cpu}$, ГГц')
    axes.set_ylabel('$cpi$')

    axes.set_title('Зависимость отношения тактов на инструкцию ($cpi$) от частоты ядра ЦП')

    # figure.legend(loc="upper right", fontsize="8")
    plt.show()
    figure.savefig('cpi.png')

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('file', type=argparse.FileType('r'),
                        help='csv file with PMU data of benchmarks')
    args = parser.parse_args()

    main(args)

