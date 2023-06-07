#include <global.h>
#include <checkArgs.hpp>
#include <wave.hpp>

int main(int argc, char *argv[])
{
    int32_t SIZE;
    bool SIMD;
    bool PRINT;
    bool CLOCK;

    checkArgs *args = new checkArgs(argc, argv);

    SIZE = args->getArgs().SIZE;
    SIMD = args->getArgs().SIMD;
    PRINT = args->getArgs().PRINT;
    CLOCK = args->getArgs().CLOCK;

    Wave *wave = new Wave(SIZE, SIMD, PRINT, CLOCK);

    wave->performFFT();

    return EXIT_SUCCESS;
}