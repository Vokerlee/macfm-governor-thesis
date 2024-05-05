#ifndef ARRAY_SIZE_LOG2
#define ARRAY_SIZE_LOG2 12
#endif

#define ARRAY_SIZE (1 << ARRAY_SIZE_LOG2)

#ifndef N_ITERS
#define N_ITERS (1 << 21)
#endif

#ifndef SHIFT_LOG2
#define SHIFT_LOG2 6
#endif

#define SHIFT (1 << SHIFT_LOG2)

unsigned long seed = 123456789;
unsigned long a = 6364136223846793005;
unsigned long c = 1442695040888963407;

static int ARRAY[ARRAY_SIZE];

unsigned long rand_number()
{
    seed = a * seed + c;
    return seed;
}

int main()
{
    for (unsigned long iter = 0; iter < N_ITERS; ++iter) {
        unsigned long idx = SHIFT * iter + (rand_number() % SHIFT);
        ARRAY[idx % ARRAY_SIZE] = ARRAY[(ARRAY_SIZE - idx) % ARRAY_SIZE];
    }

    return 0;
}
