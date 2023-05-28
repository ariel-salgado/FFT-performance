#include <global.h>
#include <checkArgs.hpp>
#include <wave.hpp>

int main(int argc, char* argv[]) {

    int32_t SIZE;
    bool    SIMD;
    bool    PRINT;

    checkArgs* args = new checkArgs(argc, argv);

	SIZE  = args->getArgs().SIZE;
    SIMD  = args->getArgs().SIMD;
    PRINT = args->getArgs().PRINT;

    Wave* wave = new Wave(SIZE, SIMD, PRINT);

    wave->performFFT();

    return EXIT_SUCCESS;
}