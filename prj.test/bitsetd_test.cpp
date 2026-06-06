#include <bitsetd/bitsetd.hpp>
#include <stdexcept>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../prj.thirdparty/doctest.h"

TEST_CASE("[bitsetd] - ctor default") {
    BitsetD bs;
    CHECK(bs.size() == 0);
}

TEST_CASE("[bitsetd] - ctor size") {
    BitsetD bs(35, true);
    CHECK(bs.size() == 35);
    CHECK(bs.get(0) == true);
    CHECK(bs.get(34) == true);
    CHECK_THROWS_AS(bs.get(35), std::out_of_range);
}

TEST_CASE("[bitsetd] - ctor mask") {
    BitsetD bs(0b1011ULL, 4); 
    CHECK(bs.size() == 4);
    CHECK(bs.get(0) == true);
    CHECK(bs.get(1) == true);
    CHECK(bs.get(2) == false);
    CHECK(bs.get(3) == true);
}

TEST_CASE("[bitsetd] - get and set") {
    BitsetD bs(10, false);
    bs.set(5, true);
    CHECK(bs.get(5) == true);
    CHECK(bs.get(4) == false);
    
    bs.set(5, false);
    CHECK(bs.get(5) == false);
    
    CHECK_THROWS_AS(bs.get(-1), std::out_of_range);
    CHECK_THROWS_AS(bs.set(10, true), std::out_of_range);
}

TEST_CASE("[bitsetd] - operator[] verification") {
    BitsetD bs(5, false);
    bs[2] = true;  
    CHECK(bs.get(2) == true);
    
    const BitsetD& cbs = bs;
    bool val = cbs[2]; 
    CHECK(val == true);
}

TEST_CASE("[bitsetd] - invert and fill") {
    BitsetD bs(5, false);
    bs.invert();
    CHECK(bs.get(0) == true);
    CHECK(bs.get(4) == true);
    
    bs.fill(false);
    CHECK(bs.get(0) == false);
}

TEST_CASE("[bitsetd] - bitwise operations") {
    BitsetD a(0b1010ULL, 4);
    BitsetD b(0b1100ULL, 4);
    
    BitsetD and_res = a & b;
    CHECK(static_cast<uint32_t>(and_res) == 0b1000);
    
    BitsetD or_res = a | b;
    CHECK(static_cast<uint32_t>(or_res) == 0b1110);
    
    BitsetD xor_res = a ^ b;
    CHECK(static_cast<uint32_t>(xor_res) == 0b0110);
}

TEST_CASE("[bitsetd] - shifts") {
    BitsetD bs(0b0001ULL, 4);
    bs <<= 2;
    CHECK(static_cast<uint32_t>(bs) == 0b0100);
    
    bs >>= 1;
    CHECK(static_cast<uint32_t>(bs) == 0b0010);
}

TEST_CASE("[bitsetd] - to_string formatting") {
    BitsetD bs(5, false);
    bs.set(0, true);
    bs.set(4, true); 
    
    CHECK(bs.to_string(BitsetD::StrFormat::BinNoPreSep) == "10001");
    CHECK(bs.to_string(BitsetD::StrFormat::Bin) == "b1'0001");
}
