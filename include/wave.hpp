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
    void bitReversal(std::vector<std::complex<double>>& v);
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

void Wave::bitReversal(std::vector<std::complex<double>>& v) {
  const int32_t size = v.size();
  const int32_t bits = static_cast<int>(std::log2(size));

  for (int32_t i = 0; i < size; ++i) {
    int32_t j = 0;
    for (int32_t k = 0; k < bits; ++k) {
      j |= ((i >> k) & 1) << (bits - 1 - k);
    }

    if (i < j) {
      std::swap(v[i], v[j]);
    }
  }
}

void Wave::FFT(std::vector<std::complex<double>>& v) {
  const int32_t n = v.size();

  if (n <= 1) return;

  bitReversal(v);

  for (int32_t s = 1; (1 << s) <= n; ++s) {
    int32_t m = 1 << s;
    double angle = 2 * M_PI / m;
    std::complex<double> w_m = std::exp(std::complex<double>(0, -angle));

    for (int32_t k = 0; k < n; k += m) {
      std::complex<double> w = 1;
      for (int32_t j = 0; j < m / 2; ++j) {
        std::complex<double> t = w * v[k + j + m / 2];
        std::complex<double> u = v[k + j];
        v[k + j] = u + t;
        v[k + j + m / 2] = u - t;
        w *= w_m;
      }
    }
  }
}

void Wave::FFT_SIMD(std::vector<std::complex<double>>& v) {
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
