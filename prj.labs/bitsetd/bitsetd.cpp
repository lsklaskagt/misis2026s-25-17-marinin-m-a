#include "bitsetd.hpp"
#include <stdexcept>
#include <algorithm>

void trim_chunks(std::int32_t size, std::vector<std::uint32_t>& chunks) noexcept {
    if (size == 0) return;
    std::int32_t last_bits = size % 32;
    if (last_bits != 0) {
        std::uint32_t mask = (std::uint32_t(1) << last_bits) - 1;
        chunks.back() &= mask;
    }
}

BitsetD::BitsetD(const std::int32_t size, const bool val)
  : size_(size)
  , chunks_((size + chunk_bi_s - 1) / chunk_bi_s)
{
  if (size_ <= 0) {
    throw std::invalid_argument("BitsetD::BitsetD - non positive size");
  }
  if (val) {
    std::fill(chunks_.begin(), chunks_.end(), 0xFFFFFFFFU);
    trim_chunks(size_, chunks_);
  }
}

BitsetD::BitsetD(const std::uint64_t mask, const std::int32_t size) 
  : size_(size)
  , chunks_{static_cast<uint32_t>(mask),
            static_cast<uint32_t>(mask >> chunk_bi_s)} {
  if (size_ <= 0) {
    throw std::invalid_argument("BitsetD::BitsetD - non positive size");
  }
  chunks_.resize(chunks_count(), 0U);
  trim_chunks(size_, chunks_);
}

bool BitsetD::operator==(const BitsetD& rhs) const noexcept {
  if (size_ != rhs.size_) {
    return false;
  }
  for (size_t i = 0; i < chunks_.size(); ++i) {
    if (chunks_[i] != rhs.chunks_[i]) {
      return false;
    }
  }
  return true;
}

void BitsetD::resize(const std::int32_t new_size, const bool val) {
  if (new_size <= 0) {
    throw std::invalid_argument("BitsetD::resize non positive new size");
  }
  std::int32_t old_size = size_;
  size_ = new_size;
  chunks_.resize(chunks_count(), val ? 0xFFFFFFFFU : 0U);
  
  if (new_size > old_size && val) {
    for (std::int32_t i = old_size; i < new_size; ++i) {
        set(i, val);
    }
  }
  trim_chunks(size_, chunks_);
}

bool BitsetD::get(const std::int32_t idx) const {
  if (idx < 0 || size_ <= idx) {
    throw std::out_of_range("BitsetD::get invalid index");
  }
  return (chunks_[idx / chunk_bi_s] & (UINT32_C(1) << (idx % chunk_bi_s))) != 0;
}

void BitsetD::set(const std::int32_t idx, const bool val) {
  if (idx < 0 || size_ <= idx) {
    throw std::out_of_range("BitsetD::set invalid index");
  }
  if (val) {
    chunks_[idx / chunk_bi_s] |= (UINT32_C(1) << (idx % chunk_bi_s));
  } else {
    chunks_[idx / chunk_bi_s] &= ~(UINT32_C(1) << (idx % chunk_bi_s));
  }
}

BitsetD& BitsetD::invert() noexcept {
  for (auto& bits : chunks_) {
    bits = ~bits;
  }
  trim_chunks(size_, chunks_);
  return *this;
}

void BitsetD::fill(const bool val) noexcept {
  std::fill(chunks_.begin(), chunks_.end(), val ? 0xFFFFFFFFU : 0U);
  trim_chunks(size_, chunks_);
}

BitsetD& BitsetD::shift(const std::int32_t shift_val) noexcept {
  if (size_ == 0) return *this;
  BitsetD copy(size_);
  for (int32_t i = 0; i < size_; i += 1) {
    int32_t target = (i + shift_val) % size_;
    if (target < 0) {
        target += size_;
    }
    if (get(i)) {
        copy.set(target, true);
    }
  }
  std::swap(chunks_, copy.chunks_);
  return *this;
}

BitsetD& BitsetD::operator<<=(const std::int32_t shift) {
  if (shift < 0) {
    return operator>>=(-shift);
  }
  if (shift >= size_) {
    fill(false);
    return *this;
  }
  for (int32_t i = size_ - 1; i >= 0; i -= 1) {
    if (i >= shift) {
        set(i, get(i - shift));
    } else {
        set(i, false);
    }
  }
  return *this;
}

BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
  if (shift < 0) {
    return operator<<=(-shift);
  }
  if (shift >= size_) {
    fill(false);
    return *this;
  }
  for (int32_t i = 0; i < size_; i += 1) {
    if (i + shift < size_) {
        set(i, get(i + shift));
    } else {
        set(i, false);
    }
  }
  return *this;
}

BitsetD& BitsetD::operator&=(const BitsetD& rhs) {
  if (size_ != rhs.size_) {
    throw std::invalid_argument("BitsetD::operator&= different size");
  }
  for (size_t i = 0; i < chunks_.size(); ++i) {
    chunks_[i] &= rhs.chunks_[i];
  }
  return *this;
}

BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
  if (size_ != rhs.size_) {
    throw std::invalid_argument("BitsetD::operator|= different size");
  }
  for (size_t i = 0; i < chunks_.size(); ++i) {
    chunks_[i] |= rhs.chunks_[i];
  }
  return *this;
}

BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
  if (size_ != rhs.size_) {
    throw std::invalid_argument("BitsetD::operator^= different size");
  }
  for (size_t i = 0; i < chunks_.size(); ++i) {
    chunks_[i] ^= rhs.chunks_[i];
  }
  return *this;
}

std::string BitsetD::to_string(const BitsetD::StrFormat fmt, const int32_t len) const {
  if (size_ == 0) return "empty";
  
  if (fmt == StrFormat::BinNoPreSep) {
    std::string res;
    for (int32_t i = size_ - 1; i >= 0; --i) {
      res += (get(i) ? '1' : '0');
    }
    return res;
  }
  
  std::string res = "b0";
  for (int32_t i = size_ - 1; i >= 0; --i) {
    res += (get(i) ? '1' : '0');
    if (i > 0 && (i % 4 == 0)) {
      res += '\'';
    }
  }
  return res;
}
