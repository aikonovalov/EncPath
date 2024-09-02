#ifndef BIGINT
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

const double PI = acos(-1);
typedef std::complex<double> Complex;

/** @brief Structure, that can storage nums with many digits
*/
class BigInt
{
public:
    BigInt() {
    }

    BigInt(int64_t, u_long) {
    }

    BigInt(std::vector<u_long>&, u_long) {
    }

    BigInt& operator+=(BigInt&) {
    }

    BigInt operator+(BigInt&) const {
    }

    BigInt& operator-=(BigInt&) {
    }

    BigInt operator-() {
    }

    BigInt operator-(BigInt&) const {
    }

    BigInt& operator*=(const BigInt&) {
    }

    BigInt operator*(const BigInt&) const {
    }

    BigInt& operator/=(const BigInt&) {
    }

    BigInt operator/(const BigInt&) const {
    }

    void divide_by_2(BigInt&){
    }

    long longValue() {
    }

    void shift_right() {
    }

    size_t getSize() const {
    }

    bool getSign() const {
    }

private:
    std::vector<u_long> digits;  // storage digits from lowest to biggest
    u_long base;  // base of this number
    bool sign;  // sign of number (1 is positive, 0 is negative)

    void fft(std::vector<Complex>& num) const {
        if (num.size() <= 1) return;

        std::vector<Complex> even_part(num.size() / 2);
        std::vector<Complex> odd_part(num.size() / 2);
        for (int i = 0; i < num.size() / 2; ++i) {
            even_part[i] = num[i * 2];
            odd_part[i] = num[i * 2 + 1];
        }

        fft(even_part);
        fft(odd_part);

        for (int i = 0; i < num.size() / 2; ++i) {
            Complex t = std::polar(1.0, 2 * PI * i / num.size()) * odd_part[i];
            num[i] = even_part[i] + t;
            num[i + num.size() / 2] = even_part[i] - t;
        }
    }

    void ifft(std::vector<Complex>& num) const {
        for (auto& value : num) {
            value = std::conj(value);
        }
        fft(num);
        for (auto &value : num) {
            value = std::conj(value) / static_cast<double>(num.size());
        }
    }

};

#endif  // BIGINT