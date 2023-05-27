#ifndef WAVE_H
#define WAVE_H

#include <global.h>

class Wave {
private:
    int32_t size;
    bool simd;

    std::vector<std::complex<double>> v;

public:
    Wave(int32_t _size, bool _simd);
    ~Wave();
    
    int32_t getSize();
    void performFFT();
    void printResults();

private:
    void FFT(std::vector<std::complex<double>>& _v);
    void FFT_SIMD(std::vector<std::complex<double>>& _v);
};

Wave::Wave(int32_t _size, bool _simd) {
	size = (int32_t)pow(2, _size);
  simd = _simd;

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
}

void Wave::FFT(std::vector<std::complex<double>>& _v) {
  int32_t n = _v.size();

  if (n <= 1) return;

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  for (int32_t i = 0; i < n / 2; ++i) {
    even[i] = _v[i * 2];
     odd[i] = _v[i * 2 + 1];
  }

  FFT(even);
  FFT(odd);

  for (int32_t k = 0; k < n / 2; ++k) {
    std::complex<double> w = 
    std::exp(std::complex<double>(0, -2.0 * M_PI * k / n)) * odd[k];

    _v[k] = even[k] + w;
    _v[k + n / 2] = even[k] - w;
  }
}

void Wave::FFT_SIMD(std::vector<std::complex<double>>& _v) {
  int32_t n = _v.size();

  if (n <= 1) return;

  std::vector<std::complex<double>> even(n / 2);
  std::vector<std::complex<double>> odd(n / 2);

  for (int32_t i = 0; i < n / 2; ++i) {
    even[i] = _v[i * 2];
    odd[i] = _v[i * 2 + 1];
  }

  FFT_SIMD(even);
  FFT_SIMD(odd);

  // initialize twiddle factors
  std::vector<std::complex<double>> twiddles(n / 2);
  for (int32_t i = 0; i < n / 2; i++) {
    twiddles[i] = std::exp(std::complex<double>(0, -2.0 * M_PI * i / n));
  }

  /*
    ...
  */
}

void Wave::printResults() {
  std::cout << std::setw(10) << "real" << std::setw(15) << "img" << std::setw(20) << "result" << std::endl;

  for (const auto& x : v) {
    std::cout << std::setw(10) << x.real() << std::setw(15) << x.imag() << std::setw(20) << x << std::endl;
  }
}

#endif // WAVE_H
