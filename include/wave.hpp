#ifndef WAVE_H
#define WAVE_H

#include <global.h>

class Wave
{
private:
    int32_t size;
    bool simd;
    bool print;

    std::vector<std::complex<double>> v;

public:
    Wave(int32_t _size, bool _simd, bool _print);
    ~Wave();

    int32_t getSize();
    void performFFT();

private:
    void FFT(std::vector<std::complex<double>> &v);
    void FFT_SIMD(std::vector<std::complex<double>> &v);
    void printResults();
};

Wave::Wave(int32_t _size, bool _simd, bool _print)
{
    size = (int32_t)pow(2, _size);
    simd = _simd;
    print = _print;

    // Generate the initial wave vector with sequential numbers
    std::generate_n(std::back_inserter(v), size, [n = 0]() mutable
                    { return n++; });
}

Wave::~Wave()
{
    // Destructor
}

int32_t Wave::getSize()
{
    return size;
}

void Wave::performFFT()
{
    // Perform FFT based on the selected method
    simd ? FFT_SIMD(v) : FFT(v);

    if (print)
        printResults();
}

/**
 * Performs the FFT recursively using the Cooley-Tukey algorithm.
 * @param v A vector with complex numbers representing the wave.
 */
void Wave::FFT(std::vector<std::complex<double>> &v)
{
    const int32_t n = v.size();

    if (n <= 1)
        return;

    std::vector<std::complex<double>> even(n / 2);
    std::vector<std::complex<double>> odd(n / 2);

    // Split the wave vector into even and odd indices
    for (int32_t i = 0; i < n / 2; ++i)
    {
        even[i] = v[i << 1];
        odd[i] = v[(i << 1) + 1];
    }

    // Perform FFT recursively on the even and odd indices
    FFT(even);
    FFT(odd);

    // Combine the results of even and odd indices
    for (int32_t k = 0; k < n / 2; ++k)
    {
        std::complex<double> w = std::exp(std::complex<double>(0, -2.0 * M_PI * k / n)) * odd[k];

        v[k] = even[k] + w;
        v[k + n / 2] = even[k] - w;
    }
}

/**
 * Performs the FFT using SIMD (Single Instruction, Multiple Data) intrinsics.
 * @param v A vector with complex numbers representing the wave.
 */
void Wave::FFT_SIMD(std::vector<std::complex<double>> &v)
{
    const int32_t n = v.size();

    if (n <= 1)
        return;

    std::vector<std::complex<double>> even(n / 2);
    std::vector<std::complex<double>> odd(n / 2);

    for (int32_t i = 0; i < n / 2; ++i)
    {
        even[i] = v[i << 1];
        odd[i] = v[(i << 1) + 1];
    }

    // Perform FFT recursively on the even and odd indices
    FFT(even);
    FFT(odd);

    __m256d _even, _w, _add, _sub;
    double a = -2.0 * M_PI / n;

    for (int32_t k = 0; k < n / 2; k += 2)
    {
        std::complex<double> w_1 = std::polar(1.0, a * k) * odd[k];
        std::complex<double> w_2 = std::polar(1.0, a * (k + 1)) * odd[k + 1];

        // Perform SIMD operations on even[k] and w
        _even = _mm256_loadu_pd(reinterpret_cast<const double *>(&even[k]));
        _w = _mm256_set_pd(w_2.imag(), w_2.real(), w_1.imag(), w_1.real());

        // Perform SIMD addition and subtraction
        _add = _mm256_add_pd(_even, _w);
        _sub = _mm256_sub_pd(_even, _w);

        std::copy((double *)&_add, (double *)&_add + 4, reinterpret_cast<double *>(&v[k]));
        std::copy((double *)&_sub, (double *)&_sub + 4, reinterpret_cast<double *>(&v[k + n / 2]));
    }
}

/**
 * Print the results of the FFT.
 */
void Wave::printResults()
{
    std::cout << std::setw(10) << "real" << std::setw(15) << "img" << std::setw(20) << "result" << std::endl;

    for (const auto &x : v)
    {
        std::cout << std::setw(10) << x.real() << std::setw(15) << x.imag() << std::setw(20) << x << std::endl;
    }
}

#endif // WAVE_H
