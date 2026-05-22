#include "complex.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <istream>

Complex Complex::operator-() const noexcept {
    return Complex(-re, -im);
}

bool Complex::operator==(const Complex& rhs) const noexcept {
    const double eps = 2.0 * std::numeric_limits<double>::epsilon();
    return (std::abs(re - rhs.re) <= eps) && (std::abs(im - rhs.im) <= eps);
}

bool Complex::operator!=(const Complex& rhs) const noexcept { 
    return !(*this == rhs); 
}

Complex& Complex::operator+=(const Complex& rhs) noexcept {
    re += rhs.re;
    im += rhs.im;
    return (*this);
}
Complex& Complex::operator+=(const double rhs) noexcept { 
    re += rhs;
    return (*this); 
}

Complex& Complex::operator-=(const Complex& rhs) noexcept {
    re -= rhs.re;
    im -= rhs.im;
    return (*this);
}
Complex& Complex::operator-=(const double rhs) noexcept { 
    re -= rhs;
    return (*this); 
}

Complex& Complex::operator*=(const Complex& rhs) noexcept {
    const double new_re = re * rhs.re - im * rhs.im;
    const double new_im = re * rhs.im + im * rhs.re;
    re = new_re;
    im = new_im;
    return (*this);
}
Complex& Complex::operator*=(const double rhs) noexcept {
    re *= rhs;
    im *= rhs;
    return (*this);
}

Complex& Complex::operator/=(const Complex& rhs) {
    const double denominator = rhs.re * rhs.re + rhs.im * rhs.im;
    const double eps = 2.0 * std::numeric_limits<double>::epsilon();
    if (std::abs(denominator) <= eps) {
        throw std::invalid_argument("Division by zero in Complex::operator/=");
    }
    const double new_re = (re * rhs.re + im * rhs.im) / denominator;
    const double new_im = (im * rhs.re - re * rhs.im) / denominator;
    re = new_re;
    im = new_im;
    return (*this);
}

Complex& Complex::operator/=(const double rhs) {
    const double eps = 2.0 * std::numeric_limits<double>::epsilon();
    if (std::abs(rhs) <= eps) {
        throw std::invalid_argument("Division by zero in Complex::operator/=");
    }
    re /= rhs;
    im /= rhs;
    return (*this);
}

std::ostream& Complex::WriteTo(std::ostream& ostrm) const noexcept {
    return ostrm << leftBrace << re << separator << im << rightBrace;
}

std::istream& Complex::ReadFrom(std::istream& istrm) noexcept {
    char left_brace = 0;
    double real_part = 0.0;
    char sep = 0;
    double imag_part = 0.0;
    char right_brace = 0;

    istrm >> left_brace >> real_part >> sep >> imag_part >> right_brace;

    if (istrm) {
        if (left_brace == leftBrace && sep == separator && right_brace == rightBrace) {
            re = real_part;
            im = imag_part;
        } else {
            istrm.setstate(std::ios_base::failbit);
        }
    }
    return istrm;
}

Complex operator+(const Complex& lhs, const Complex& rhs) noexcept { return Complex(lhs) += rhs; }
Complex operator+(const Complex& lhs, const double rhs) noexcept { return Complex(lhs) += rhs; }
Complex operator+(const double lhs, const Complex& rhs) noexcept { return Complex(lhs) += rhs; }

Complex operator-(const Complex& lhs, const Complex& rhs) noexcept { return Complex(lhs) -= rhs; }
Complex operator-(const Complex& lhs, const double rhs) noexcept { return Complex(lhs) -= rhs; }
Complex operator-(const double lhs, const Complex& rhs) noexcept { return Complex(-rhs) += lhs; }

Complex operator*(const Complex& lhs, const Complex& rhs) noexcept { return Complex(lhs) *= rhs; }
Complex operator*(const Complex& lhs, const double rhs) noexcept { return Complex(lhs) *= rhs; }
Complex operator*(const double lhs, const Complex& rhs) noexcept { return Complex(lhs) *= rhs; }

Complex operator/(const Complex& lhs, const Complex& rhs) { return Complex(lhs) /= rhs; }
Complex operator/(const Complex& lhs, const double rhs) { return Complex(lhs) /= rhs; }
Complex operator/(const double lhs, const Complex& rhs) { return Complex(lhs) /= rhs; }
