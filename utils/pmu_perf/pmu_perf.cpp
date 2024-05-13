#include "pmu_perf.hpp"

#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>

// Amount of time when PMU is not collected due to benchmark is not
// stabilized yet
static constexpr size_t N_USEC_TO_WAIT = 300000;

dev_ctl::Events events_collector;
int pid_benchmark = -1;

void finish_collecting(int unused)
{
    (void)unused;

    std::vector<float> final_pmu_values = events_collector.read_events();
    events_collector.disable_collection(true);
    events_collector.clear_events();

    kill(pid_benchmark, SIGUSR1);

    // std::cout << "Time running: " << final_pmu_values[final_pmu_values.size() - 2] << std::endl;
    // std::cout << "Time enabled: " << final_pmu_values[final_pmu_values.size() - 1] << std::endl;
    // std::cout << "PMU counters stats:" << std::endl;

    for (size_t i = 0; i < final_pmu_values.size() - 1; ++i) {
    //     std::cout << "\t0x" << std::hex << events[i].perf_id << std::dec << ": " <<
    //         final_pmu_values[i] << std::endl;
        std::cout << final_pmu_values[i] << ",";
    }

    std::cout << final_pmu_values[final_pmu_values.size() - 1] << std::endl;
}

int main(int argc, char *argv[])
{
    int status = 0;

    if (argc < 3) {
        std::cerr << "Invalid amount of arguments" << std::endl;
        std::cerr << "Example: {name}.out 2 0x8 0x2A ls -la" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<dev_ctl::event_t> events;
    size_t n_events = strtoul(argv[1], NULL, 10);
    if (errno != 0) {
        std::cerr << "strtoul() call was with error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (n_events < 1) {
        std::cerr << "amount of events should be 1 or more" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n_events; ++i) {
        uint32_t perf_id = strtol(argv[2 + i], NULL, 16);
        events.push_back({.type = PERF_TYPE_RAW, .perf_id = perf_id});
    }

    signal(SIGUSR1, finish_collecting);

    pid_benchmark = fork();
    if (pid_benchmark < 0) {
        std::cerr << "fork() returned -1: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid_benchmark == 0) {
        if (execvp(argv[2 + n_events], &(argv[2 + n_events])) == -1) {
            std::cerr << "execvp() returned -1: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // std::cerr << "DEBUG: " << "launched \"" << argv[2 + n_events] <<
    //     "\" with pid = " << pid_benchmark << std::endl;

    usleep(N_USEC_TO_WAIT);

    events_collector.add_events(pid_benchmark, events);
    events_collector.enable_collection();

    int benchmark_pid = waitpid(pid_benchmark, &status, 0);
    if (benchmark_pid == -1) {
        std::cerr << "waitpid() returned -1: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
