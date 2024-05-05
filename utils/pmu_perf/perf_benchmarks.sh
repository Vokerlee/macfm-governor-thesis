#!bin/bash

pmu_perf="perf.sh"
script_path=$(dirname $(realpath $0))
cd $script_path

benchmarks=$(echo $@ | tr "," "\n")

for bench in $benchmarks
do
    echo "Start collecting $bench"
    bash $pmu_perf $bench
done
