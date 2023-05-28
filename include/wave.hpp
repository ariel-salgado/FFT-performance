#ifndef WAVE_H
#define WAVE_H

#include <global.h>

class Wave {
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
    void FFT(std::vector<std::complex<double>>& v);
    void FFT_SIMD(std::vector<std::complex<double>>& v);
    void printResults();
};

Wave::Wave(int32_t _size, bool _simd, bool _print) {
	size  = (int32_t)pow(2, _size);
  simd  = _simd;
  print = _print;

  std::generate_n(std::back_inserter(v), size, [n = 0]() mutable { return n++; });
}

Wave::~Wave() {
	
}

int32_t Wave::getSize() {
	return size;
}

void Wave::performFFT() {
  if(!simd) {
    FFT(v);
  } else {
    FFT_SIMD(v);
  }

  if (print) printResults();
}

void Wave::FFT(std::vector<std::complex<double>>& v) {
  int32_t n = v.size();

  if (n <= 1) return;

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  for (int32_t i = 0; i < n / 2; ++i) {
    even[i] = v[i << 1];
    odd[i] = v[(i << 1) + 1];
  }

  FFT(even);
  FFT(odd);

  for (int32_t k = 0; k < n / 2; ++k) {
    std::complex<double> w = 
    std::exp(std::complex<double>(0, -2.0 * M_PI * k / n)) * odd[k];

    v[k] = even[k] + w;
    v[k + n / 2] = even[k] - w;
  }
}

void Wave::FFT_SIMD(std::vector<std::complex<double>>& v) {
  int32_t n = v.size();

  if (n <= 1) return;

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  for (int32_t i = 0; i < n / 2; ++i) {

    // Load two consecutive complex numbers into SSE registers
    __m128d vec1 = _mm_loadu_pd(reinterpret_cast<double*>(&v[i << 1]));
    __m128d vec2 = _mm_loadu_pd(reinterpret_cast<double*>(&v[(i << 1) + 1]));

    // Extract real and imaginary parts using SSE intrinsics
    __m128d even_real = _mm_unpacklo_pd(vec1, vec1);
    __m128d even_imag = _mm_unpackhi_pd(vec1, vec1);
    __m128d odd_real  = _mm_unpacklo_pd(vec2, vec2);
    __m128d odd_imag  = _mm_unpackhi_pd(vec2, vec2);

    // Store the results into even and odd vectors
    _mm_storeu_pd(reinterpret_cast<double*>(&even[i]), even_real);
    _mm_storeu_pd(reinterpret_cast<double*>(&even[i]) + 1, even_imag);
    _mm_storeu_pd(reinterpret_cast<double*>(&odd[i]), odd_real);
    _mm_storeu_pd(reinterpret_cast<double*>(&odd[i]) + 1, odd_imag);
  }

  FFT(even);
  FFT(odd);

  for (int32_t k = 0; k < n / 2; ++k) {
    std::complex<double> w = 
    std::exp(std::complex<double>(0, -2.0 * M_PI * k / n)) * odd[k];

    v[k] = even[k] + w;
    v[k + n / 2] = even[k] - w;
  }
}

void Wave::printResults() {
  std::cout << std::setw(10) << "real" << std::setw(15) << "img" << std::setw(20) << "result" << std::endl;

  for (const auto& x : v) {
    std::cout << std::setw(10) << x.real() << std::setw(15) << x.imag() << std::setw(20) << x << std::endl;
  }
}

#endif // WAVE_H
