import os

def build_mem(params):
    for arr_size, shift in sorted(params):
        os.system(f'make ARRAY_SIZE_LOG2={arr_size} SHIFT_LOG2={shift} POSTFIX={arr_size}_{shift}')

def params_list():
    return [(8, 1), (8, 3), (10, 3), (10, 4), (10, 5), (10, 6), (12, 4),
            (12, 5), (12, 6), (12, 7), (14, 5), (14, 6), (14, 7), (14, 8),
            (16, 6), (16, 7), (16, 8), (16, 9), (16, 10), (18, 6),
            (18, 8), (18, 10), (18, 12), (20, 6), (20, 8), (20, 10),
            (20, 12), (20, 14), (22, 8), (22, 11), (22, 14), (22, 16),
            (24, 11), (24, 14), (24, 17), (24, 19), (26, 12), (26, 15),
            (26, 18), (28, 13), (28, 7), (28, 16), (28, 19), (28, 8), (29, 20)]

if __name__ == "__main__":
    build_mem(params_list())