CC = aarch64-linux-gnu-g++
FLAGS = -Wall -Wextra -Werror -static

SRC_FILE = pmu_perf.cpp
OUT_FILE = $(basename $(SRC_FILE))


all:
	$(CC) $(FLAGS) $(SRC_FILE) -o $(OUT_FILE)

clean:
	$(RM) $(OUT_FILE)
