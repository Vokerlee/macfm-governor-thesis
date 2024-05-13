#!bin/bash

pmu_perf_program="pmu_perf"
pmu_list="0x08 0x11 0x01 0x03 0x17 0x2a"
cpu0_freqs_path="/sys/devices/system/cpu/cpufreq/policy0/scaling_available_frequencies"
cpu0_max_freq_path="/sys/devices/system/cpu/cpufreq/policy0/scaling_max_freq"
cpu0_min_freq_path="/sys/devices/system/cpu/cpufreq/policy0/scaling_min_freq"

IFS=' ' read -r -a pmu_array <<< "$pmu_list"
n_pmus="${#pmu_array[@]}"

cmd_launch="timeout --signal=USR1 1.1 $pmu_perf_program $n_pmus $pmu_list "$@""

IFS=' ' read -r -a cpu0_freqs <<< "$(cat $cpu0_freqs_path)"
freqs="${cpu0_freqs[@]}"

echo "benchmark,cpufreq,${pmu_list// /,},time_run,time_en"

for freq in $freqs
do
    echo "${freq}" > $cpu0_max_freq_path
    echo "${freq}" > $cpu0_min_freq_path
    echo "${freq}" > $cpu0_max_freq_path

    pmus_res=$($cmd_launch)

    echo "$1,${freq},${pmus_res}"
done
