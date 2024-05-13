import os

def build_mem(params):
    for arr_size, shift in sorted(params):
        os.system(f'make ARRAY_SIZE_LOG2={arr_size} SHIFT_LOG2={shift} POSTFIX={arr_size}_{shift}')

def params_list():
    return [(10, 4), (14, 7), (16, 9), (17, 9), (18, 9), (18, 12), (19, 9), (19, 12), (20, 9),
            (20, 12), (20, 14), (21, 9), (21, 12), (21, 14), (22, 10), (22, 14),
            (23, 11), (23, 14), (24, 12), (24, 17), (25, 7), (25, 13), (25, 18),
            (26, 18), (28, 13), (28, 17), (28, 20), (29, 10), (29, 19), (29, 24)]

if __name__ == "__main__":
    build_mem(params_list())