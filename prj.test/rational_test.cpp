#include "rational.hpp"

#include <sstream>
#include <stdexcept>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../prj.thirdparty/doctest.h"

TEST_CASE("[rational] - ctor default") {
    Rational r;
    CHECK(r.num() == 0);
    CHECK(r.den() == 1);
}

TEST_CASE("[rational] - ctor int") {
    Rational r(3);
    CHECK(r.num() == 3);
    CHECK(r.den() == 1);
}

TEST_CASE("[rational] - ctor int int") {
    Rational r(6, 8); 
    CHECK(r.num() == 3);
    CHECK(r.den() == 4);
}

TEST_CASE("[rational] - ctor zero den") {
    CHECK_THROWS_AS(Rational(1, 0), std::invalid_argument);
}

TEST_CASE("[rational] - ctor negative den") {
    Rational r(1, -2);
    CHECK(r.num() == -1);
    CHECK(r.den() == 2);
}

TEST_CASE("[rational] - op== op!=") {
    CHECK(Rational(1, 2) == Rational(2, 4));
    CHECK(!(Rational(1, 2) == Rational(1, 3)));
    CHECK(Rational(1, 2) != Rational(1, 3));
    CHECK(!(Rational(1, 2) != Rational(2, 4)));
}

TEST_CASE("[rational] - op< op<= op> op>=") {
    CHECK(Rational(1, 3) < Rational(1, 2));
    CHECK(Rational(1, 2) <= Rational(2, 4));
    CHECK(Rational(1, 2) > Rational(1, 3));
    CHECK(Rational(2, 4) >= Rational(1, 2));
}

TEST_CASE("[rational] - op- unary") {
    Rational r(3, 4);
    const Rational neg = -r;
    CHECK(neg == Rational(-3, 4));
    CHECK(r == Rational(3, 4)); 
}

TEST_CASE("[rational] - op+= Rational") {
    Rational r(1, 2);
    r += Rational(1, 3);
    CHECK(r == Rational(5, 6));
}

TEST_CASE("[rational] - op+= int32_t") {
    Rational r(1, 2);
    r += 2;
    CHECK(r == Rational(5, 2));
}

TEST_CASE("[rational] - op-= Rational") {
    Rational r(1, 2);
    r -= Rational(1, 3);
    CHECK(r == Rational(1, 6));
}

TEST_CASE("[rational] - op-= int32_t") {
    Rational r(5, 2);
    r -= 2;
    CHECK(r == Rational(1, 2));
}

TEST_CASE("[rational] - op*= Rational") {
    Rational r(2, 3);
    r *= Rational(3, 4);
    CHECK(r == Rational(1, 2));
}

TEST_CASE("[rational] - op*= int32_t") {
    Rational r(3, 4);
    r *= 2;
    CHECK(r == Rational(3, 2));
}

TEST_CASE("[rational] - op/= Rational") {
    Rational r(1, 2);
    r /= Rational(2, 3);
    CHECK(r == Rational(3, 4));
}

TEST_CASE("[rational] - op/= int32_t") {
    Rational r(3, 4);
    r /= 2;
    CHECK(r == Rational(3, 8));
}

TEST_CASE("[rational] - op/= zero") {
    Rational r(3, 4);
    CHECK_THROWS_AS(r /= Rational(0, 1), std::invalid_argument);
    CHECK_THROWS_AS(r /= 0, std::invalid_argument);
}

TEST_CASE("[rational] - op+ Rational") {
    CHECK(Rational(1, 2) + Rational(1, 3) == Rational(5, 6));
}

TEST_CASE("[rational] - op+ int32_t") {
    CHECK(Rational(1, 2) + 2 == Rational(5, 2));
    CHECK(2 + Rational(1, 2) == Rational(5, 2));
}

TEST_CASE("[rational] - op- Rational") {
    CHECK(Rational(1, 2) - Rational(1, 3) == Rational(1, 6));
}

TEST_CASE("[rational] - op- int32_t") {
    CHECK(Rational(5, 2) - 2 == Rational(1, 2));
    CHECK(2 - Rational(1, 2) == Rational(3, 2));
}

TEST_CASE("[rational] - op* Rational") {
    CHECK(Rational(2, 3) * Rational(3, 4) == Rational(1, 2));
}

TEST_CASE("[rational] - op* int32_t") {
    CHECK(Rational(3, 4) * 2 == Rational(3, 2));
    CHECK(2 * Rational(3, 4) == Rational(3, 2));
}

TEST_CASE("[rational] - op/ Rational") {
    CHECK(Rational(1, 2) / Rational(2, 3) == Rational(3, 4));
    CHECK_THROWS_AS(static_cast<void>(Rational(1, 2) / Rational(0, 1)), std::invalid_argument);
}

TEST_CASE("[rational] - op/ int32_t") {
    CHECK(Rational(3, 4) / 2 == Rational(3, 8));
    CHECK_THROWS_AS(static_cast<void>(Rational(3, 4) / 0), std::invalid_argument);
}

TEST_CASE("[rational] - op/ int32_t Rational") {
    CHECK(2 / Rational(3, 4) == Rational(8, 3));
    CHECK_THROWS_AS(static_cast<void>(1 / Rational(0, 1)), std::invalid_argument);
}

TEST_CASE("[rational] - write_to") {
    std::ostringstream ss;
    ss << Rational(3, 4);
    CHECK("3/4" == ss.str());

    ss.str("");
    ss << Rational(-5, 7);
    CHECK("-5/7" == ss.str());
}

TEST_CASE("[rational] - read_from") {
    Rational r;
    std::istringstream ss("5/7");
    ss >> r;
    CHECK(r == Rational(5, 7));
}

TEST_CASE("[rational] - read_from bad format") {
    Rational r;
    std::istringstream ss("5a7");
    ss >> r;
    CHECK(ss.fail());
}
