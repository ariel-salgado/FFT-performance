#include <global.h>
#include <checkArgs.hpp>
#include <wave.hpp>

int main(int argc, char* argv[]) {

    int32_t SIZE;
    bool    SIMD;

    checkArgs* args = new checkArgs(argc, argv);

	SIZE = args->getArgs().SIZE;
    SIMD = args->getArgs().SIMD;

    Wave* wave = new Wave(SIZE, SIMD);

    wave->performFFT();
    wave->printResults();

    return EXIT_SUCCESS;
}