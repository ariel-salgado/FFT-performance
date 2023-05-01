// #include <immintrin.h>
#include <algorithm>
#include <iostream>
#include <complex>
#include <vector>

void fft(std::vector<std::complex<double>>& input)
{
    size_t n = input.size();

    if (n <= 1) return;

    std::vector<std::complex<double>> even(n / 2);
    std::vector<std::complex<double>> odd(n / 2);

    for (size_t i = 0; i != n / 2; ++i) {
        even[i] = input[i * 2];
        odd[i] = input[i * 2 + 1];
    }

    fft(even);
    fft(odd);

    for (size_t k = 0; k != n / 2; ++k)
    {
        std::complex<double> w = 
        std::exp(std::complex<double>(0, -2.0 * M_PI * k / n)) * odd[k];

        input[k] = even[k] + w;
        input[k + n / 2] = even[k] - w;
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::complex<double>> v(8);

    std::generate(v.begin(), v.end(), [n = 0] () mutable { return n++; });

    fft(v);

    for (auto x : v)
    {
        std::cout << x << std::endl;
    }

    return 0;
}