#pragma once
#include <bigint.h>

BigInt::BigInt(int64_t num, u_long base) {
    this->base = base;
    this->sign = (num >= 0);
    while (num != 0) {
        this->digits.push_back(num % base);
        num /= base;
    }
}

BigInt::BigInt(std::vector<u_long>& num, u_long base) {
    this->digits = num;
    this->base = base;
    this->sign = (num.front() >= 0);
}

BigInt& BigInt::operator+=(BigInt& other) {
    if (this->getSign()) {
        if (other.getSign()) {
            // alignment of digit vectors
            while (this->getSize() < other.getSize()) {
                this->digits.push_back(0);
            }
            for (size_t i = 0; i < std::min(this->getSize(), other.getSize()); ++i) {
                this->digits[i] += other.digits[i];
                if (i == std::min(this->getSize(), other.getSize()) - 1 && this->digits[i] > base) {
                    this->digits.push_back(0);
                }
                if (this->digits[i] > base) {
                    this->digits[i + 1] += this->digits[i] / base;
                    this->digits[i] %= base;
                }
            }
            return *this;

        }
        *this = *this - (-(other));
        return *this;

    }
    *this = -((-(*this)) - (other));
    return *this;

}

BigInt BigInt::operator+(BigInt& other) const {
    BigInt res;
    res = *this;
    res += other;
    return res;
}

BigInt& BigInt::operator-=(BigInt& other) {
    *this = *this -(other);
    return *this;
}

BigInt BigInt::operator-(BigInt& other) const {
    BigInt res = *this;
    res -= other;
    return res;
}

BigInt BigInt::operator-() {
    BigInt other = *this;
    other.sign = ~(this->sign);
    return other;
}

BigInt& BigInt::operator*=(const BigInt& other) {
        if (this->base != other.base) {
            throw std::exception();
        };
        size_t result_size = 1;
        while (result_size < this->digits.size() + other.digits.size()) {
            result_size <<= 1;
        }

        std::vector<Complex> lhs(result_size);
        std::vector<Complex> rhs(result_size);
        for (size_t i = 0; i < this->digits.size(); ++i) {
            lhs[i] = Complex(this->digits[i], 0);
        }
        for (size_t i = 0; i < other.digits.size(); ++i) {
            rhs[i] = Complex(other.digits[i], 0);
        }

        fft(lhs);
        fft(rhs);
        for (size_t i = 0; i < result_size; ++i) {
            lhs[i] *= rhs[i];
        }
        ifft(lhs);

        BigInt result(0, other.base);
        result.digits.resize(this->digits.size() + other.digits.size());
        for (size_t i = 0; i < this->digits.size() + other.digits.size(); ++i) {
            result.digits[i] = static_cast<u_long>(std::round(lhs[i].real()));
        }

        while (result.digits.size() > 1 && result.digits.back() == 0) {
            result.digits.pop_back();
        }
        result.sign = this->sign != other.sign;
        *this = result;
        return *this;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
    BigInt temp;
	temp = *this;
	temp *= rhs;
	return temp;
}

void BigInt::divide_by_2(BigInt& num){
	int add = 0;
	for (size_t i = num.digits.size() - 1; i >= 0; i--){
		int digit = (num.digits[i] >> 1) + add;
		add = ((num.digits[i] & 1) * 32);
		num.digits[i] = digit;
	}
	while(num.digits.size() > 1 && !num.digits.back()) {
		num.digits.pop_back();
    }
    return;
}

long BigInt::longValue() {
    long res = 0;
    for (size_t i = 0; i < this->digits.size(); ++i) {
        res *= this->base;
        res += this->digits[this->digits.size() - 1 - i];
    }
    return res;
}

void BigInt::shift_right() {
    if (this->digits.size() == 0) {
        this->digits.push_back(0);
        return;
    }
    this->digits.push_back(this->digits[this->digits.size() - 1]);
    for (size_t i = this->digits.size() - 2; i > 0; --i) {
        this->digits[i] = this->digits[i - 1];
    }
    this->digits[0] = 0;
}

size_t BigInt::getSize() const {
    return this->digits.size();
}

bool BigInt::getSign() const {
    return this->sign;
}
