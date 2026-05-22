#include "rational.hpp"
#include <stdexcept>
#include <numeric>
#include <iostream>

Rational::Rational(const std::int32_t num, const std::int32_t den)
  : num_(num), den_(den) { // num_ - числитель, den_ - знаменатель!!!!
  if (0 == den_) {
    throw std::invalid_argument("Zero denumenator in Rational ctor");
  }
  Normalize();
}

void Rational::Normalize() noexcept {
    if (den_ < 0){
        num_ = -num_;
        den_ = -den_;
    }
    const std::int32_t gcd = std::gcd(num_, den_);
    if (gcd > 1){
        num_ = num_ / gcd;
        den_ = den_ / gcd;
    }
}

bool Rational::operator==(const Rational& rhs) const noexcept {
    return (num_ == rhs.num_ && den_ == rhs.den_);
}
bool Rational::operator!=(const Rational& rhs) const noexcept { 
    return !(*this == rhs);
}
bool Rational::operator<(const Rational& rhs) const noexcept { 
    return (num_ * rhs.den_ < rhs.num_ * den_);
}
bool Rational::operator<=(const Rational& rhs) const noexcept {
    return !(*this > rhs);
}
bool Rational::operator>(const Rational& rhs) const noexcept {
    return (rhs < *this);
}
bool Rational::operator>=(const Rational& rhs) const noexcept {
    return !(*this < rhs);
}

Rational& Rational::operator+=(const Rational& rhs) noexcept {
    num_ = num_ * rhs.den_ + rhs.num_ * den_;
    den_ = den_ * rhs.den_;
    Normalize();
    return(*this);
}
Rational& Rational::operator-=(const Rational& rhs) noexcept {
    return (*this += -(rhs));
}
Rational& Rational::operator*=(const Rational& rhs) noexcept {
    num_ = num_ * rhs.num_;
    den_ = den_ * rhs.den_;
    Normalize();
    return(*this);
}
Rational& Rational::operator/=(const Rational& rhs) {
    if (0 == rhs.num_){
        throw std::invalid_argument("Division by zero in Rational::operator/=!!");
    }
    num_ = num_ * rhs.den_;
    den_ = den_ * rhs.num_;
    Normalize();
    return(*this);
}

Rational& Rational::operator+=(const int32_t rhs) noexcept { return operator+=(Rational(rhs)); };
Rational& Rational::operator-=(const int32_t rhs) noexcept { return operator-=(Rational(rhs)); };
Rational& Rational::operator*=(const int32_t rhs) noexcept { return operator*=(Rational(rhs)); };
Rational& Rational::operator/=(const int32_t rhs) { return operator/=(Rational(rhs)); };


Rational operator+(const Rational& lhs, const Rational& rhs) noexcept { return Rational{ lhs } += rhs; }
Rational operator-(const Rational& lhs, const Rational& rhs) noexcept { return Rational{ lhs } -= rhs; }
Rational operator*(const Rational& lhs, const Rational& rhs) noexcept { return Rational{ lhs } *= rhs; }
Rational operator/(const Rational& lhs, const Rational& rhs) { return Rational{lhs} /= rhs; }

Rational operator+(const Rational& lhs, const int32_t rhs) noexcept { return Rational{ lhs } += rhs; }
Rational operator-(const Rational& lhs, const int32_t rhs) noexcept { return Rational{ lhs } -= rhs; };
Rational operator*(const Rational& lhs, const int32_t rhs) noexcept { return Rational{ lhs } *= rhs; };
Rational operator/(const Rational& lhs, const int32_t rhs) { return Rational{ lhs } /= rhs; }

Rational operator+(const int32_t lhs, const Rational& rhs) noexcept { return Rational{ lhs } += rhs; }
Rational operator-(const int32_t lhs, const Rational& rhs) noexcept { return Rational{ lhs } -= rhs; }
Rational operator*(const int32_t lhs, const Rational& rhs) noexcept { return Rational{ lhs } *= rhs; }
Rational operator/(const int32_t lhs, const Rational& rhs) { return Rational{ lhs } /= rhs; }

std::ostream& Rational::WriteTo(std::ostream& ostrm) const noexcept {
    return ostrm << num_ << '/' << den_;
}


std::istream& Rational::ReadFrom(std::istream& istrm) noexcept {
    std::int32_t num = 0;
    char slash = 0;
    std::int32_t den = 1;
    
    istrm >> num >> slash >> den;
    
    if (istrm) {
      if ('/' == slash && 0 != den) {
        num_ = num;
        den_ = den;
        Normalize();
      } else {
        istrm.setstate(std::ios_base::failbit);
      }
    }
    return istrm;
  }
  
  std::ostream& operator<<(std::ostream& ostrm, const Rational& rhs) noexcept {
    return rhs.WriteTo(ostrm);
  }
  
  std::istream& operator>>(std::istream& istrm, Rational& rhs) noexcept {
    return rhs.ReadFrom(istrm);
  }
  
