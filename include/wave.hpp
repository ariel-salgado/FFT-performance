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
  simd ? FFT_SIMD(v) : FFT(v);

  if (print) printResults();
}

void Wave::FFT(std::vector<std::complex<double>>& v) {
  const int32_t n = v.size();

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
  const int32_t n = v.size();

  if (n <= 1) return;

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  __m128d _tmp, _even, _w, _add, _sub;

  for (int32_t i = 0; i < n / 2; ++i) {
    std::complex<double>* evenPtr = even.data();
    std::complex<double>* oddPtr = odd.data();

    _tmp = _mm_set_pd(v[i << 1].imag(), v[i << 1].real());
    _mm_store_pd(reinterpret_cast<double*>(&evenPtr[i]), _tmp);

    _tmp = _mm_set_pd(v[(i << 1) + 1].imag(), v[(i << 1) + 1].real());
    _mm_store_pd(reinterpret_cast<double*>(&oddPtr[i]), _tmp);
  }

  FFT(even);
  FFT(odd);

  for (int32_t k = 0; k < n / 2; ++k) {
    std::complex<double> w = std::exp(std::complex<double>(0, -2.0 * M_PI * k / n)) * odd[k];

    // Perform SIMD operations on even[k] and w
    _even = _mm_load_pd(reinterpret_cast<double*>(&even[k]));
    _w = _mm_set_pd(w.imag(), w.real());

    // Perform SIMD addition and subtraction
    _add = _mm_add_pd(_even, _w);
    _sub = _mm_sub_pd(_even, _w);

    // Store the results back to the vector v
    _mm_store_pd(reinterpret_cast<double*>(&v[k]), _add);
    _mm_store_pd(reinterpret_cast<double*>(&v[k + n / 2]), _sub);
  }
}

void Wave::printResults() {
  std::cout << std::setw(10) << "real" << std::setw(15) << "img" << std::setw(20) << "result" << std::endl;

  for (const auto& x : v) {
    std::cout << std::setw(10) << x.real() << std::setw(15) << x.imag() << std::setw(20) << x << std::endl;
  }
}

#endif // WAVE_H
