//
// Created by Алексей on 14.06.2024.
//
#include "big_integer.h"
#include <iostream>
#include <cstring>

// Метод для быстрого умножения на 10 в степени разряда
BigInteger BigInteger::Shift(int n) {
  BigInteger integer = *this;
  std::vector<int16_t> vect(n);
  vect.insert(vect.end(), integer.number_.begin(), integer.number_.end());
  integer.number_ = vect;
  if (integer.Size() > BigInteger::kMaxArraySize) {
    throw BigIntegerOverflow{};
  }
  return integer;
}

std::ostream& operator<<(std::ostream& stream, const BigInteger& integer) {
  if (integer.IsNegative()) {
    stream << "-";
  }
  for (int i = integer.Size() - 1; i >= 0; i--) {
    if (i != integer.Size() - 1) {
      stream << std::string(4 - std::to_string(integer[i]).size(), '0');
    }
    stream << integer[i];
  }
  return stream;
}

std::istream& operator>>(std::istream& stream, BigInteger& integer) {
  std::string str;
  stream >> str;
  integer = BigInteger(str.c_str());
  return stream;
}

int BigInteger::Size() const {
  return static_cast<int>(number_.size());
}

BigInteger BigInteger::ShortMultiplication(const int16_t& number) const {
  BigInteger integer;
  int n = 0;
  int remain = 0;
  for (int i = 0; i < this->Size(); i++) {
    n = this->operator[](i) * number + remain;

    if (n > BigInteger::kNumberSystem) {
      remain = n / BigInteger::kNumberSystem;
      n %= BigInteger::kNumberSystem;
    } else {
      remain = 0;
    }
    integer.number_.push_back(n);  // NOLINT
  }
  if (remain > 0) {
    integer.number_.emplace_back(remain);
  }

  if (integer.Size() > BigInteger::kMaxArraySize) {
    throw BigIntegerOverflow{};
  }
  return integer;
}

int16_t BigInteger::operator[](int n) const {
  return this->Size() <= n ? static_cast<int16_t>(0) : number_[n];
}

BigInteger::BigInteger() : number_(), is_negative_(false) {
}

BigInteger::BigInteger(int64_t n) : number_() {
  is_negative_ = n < 0;
  n = std::abs(n);
  if (n == 0) {
    number_.push_back(0);
  }
  while (n > 0) {
    number_.push_back(static_cast<int16_t>(n % kNumberSystem));
    n /= kNumberSystem;
  }
}

BigInteger::BigInteger(int n) : number_() {
  is_negative_ = n < 0;
  n = std::abs(n);
  if (n == 0) {
    number_.push_back(0);
  }
  while (n > 0) {
    number_.push_back(static_cast<int16_t>(n % kNumberSystem));
    n /= kNumberSystem;
  }
}

BigInteger::BigInteger(const char* number) {
  is_negative_ = number[0] == '-';
  int fl = (is_negative_ ? 1 : 0);
  if (number[0] == '+') {
    fl = 1;
  }
  for (int i = static_cast<int>(std::strlen(number) - 1); i >= fl; i -= 4) {

    int16_t temp = 0;
    for (int j = 0, pow = 1; pow < kNumberSystem && i - j >= fl; j++, pow *= 10) {
      temp += static_cast<int16_t>((number[i - j] - '0')) * pow;  // NOLINT
    }
    number_.emplace_back(temp);
  }
}

bool BigInteger::IsNegative() const {
  return is_negative_;
}

BigInteger BigInteger::operator+() const {
  return *this;
}

BigInteger BigInteger::operator-() const {
  BigInteger temp = *this;
  temp.is_negative_ = !is_negative_;
  return temp;
}
BigInteger operator+(const BigInteger& left_integer, const BigInteger& right_integer) {
  if (left_integer.IsNegative() && !right_integer.IsNegative()) {
    BigInteger temp = left_integer;
    temp.is_negative_ = false;
    return right_integer - temp;
  }

  if (right_integer.IsNegative() && !left_integer.IsNegative()) {
    BigInteger temp = right_integer;
    temp.is_negative_ = false;
    return left_integer - temp;
  }

  BigInteger temp;
  int remain = 0;

  for (int i = 0; i < std::max(left_integer.Size(), right_integer.Size()); ++i) {
    temp.number_.emplace_back(left_integer[i] + right_integer[i] + remain);
    remain = temp[i] / BigInteger::kNumberSystem;
    temp.number_[i] %= BigInteger::kNumberSystem;
  }

  if (remain > 0) {
    temp.number_.emplace_back(remain);
  }
  temp.is_negative_ = left_integer.is_negative_;
  if (temp.Size() > BigInteger::kMaxArraySize) {
    throw BigIntegerOverflow{};
  }
  return temp;
}

bool BigInteger::operator<(const BigInteger& integer) const {
  if (!integer.IsNegative() && this->IsNegative()) {
    return true;
  }
  if (integer.IsNegative() && !this->IsNegative()) {
    return false;
  }
  if (integer.IsNegative() && this->IsNegative()) {
    return -integer < -(*this);
  }
  if (this->Size() != integer.Size()) {
    return this->Size() < integer.Size();
  }
  for (int i = this->Size() - 1; i >= 0; i--) {
    if (this->operator[](i) == integer[i]) {
      continue;
    }
    return this->operator[](i) < integer[i];
  }
  return false;
}
bool BigInteger::operator>(const BigInteger& integer) const {
  return integer < *this;
}
bool BigInteger::operator>=(const BigInteger& integer) const {
  return !(*this < integer);
}
bool BigInteger::operator<=(const BigInteger& integer) const {
  return !(*this > integer);
}
bool BigInteger::operator==(const BigInteger& integer) const {
  if ((this->IsNegative() ^ integer.IsNegative()) || (this->Size() != integer.Size())) {
    return false;
  }
  for (int i = 0; i < this->Size(); i++) {
    if (this->operator[](i) != integer[i]) {
      return false;
    }
  }
  return true;
}

bool BigInteger::operator!=(const BigInteger& integer) const {
  return !(*this == integer);
}

BigInteger operator-(const BigInteger& left_integer, const BigInteger& right_integer) {
  if (right_integer == left_integer) {
    return 0;
  }

  if (left_integer.IsNegative() && !right_integer.IsNegative()) {
    BigInteger temp = right_integer;
    temp.is_negative_ = true;
    return (left_integer + temp);
  }

  if (right_integer.IsNegative() && !left_integer.IsNegative()) {
    BigInteger temp = right_integer;
    temp.is_negative_ = false;
    return left_integer + temp;
  }

  // Определяем больший и меньший по модулю для того,
  // чтобы вычитать из большего меньший.
  // Left integer должен быть больше right integer.

  if (right_integer.IsNegative()) {
    if (left_integer > right_integer) {
      return -(right_integer - left_integer);
    }
  } else {
    if (left_integer < right_integer) {
      return -(right_integer - left_integer);
    }
  }
  BigInteger answer;
  int helper = 0;
  for (int i = 0; i < left_integer.Size(); i++) {
    answer.number_.emplace_back(left_integer[i] - right_integer[i] - helper);
    if (answer.number_[i] < 0) {
      helper = 1;
      answer.number_[i] += BigInteger::kNumberSystem;
    } else {
      helper = 0;
    }
  }
  while (answer[answer.Size() - 1] == 0) {
    answer.number_.erase(answer.number_.end() - 1);
  }

  answer.is_negative_ = left_integer.IsNegative();

  if (answer.Size() > BigInteger::kMaxArraySize) {
    throw BigIntegerOverflow{};
  }
  return answer;
}

BigInteger& BigInteger::operator+=(const BigInteger& integer) {
  return *this = *this + integer;
}

BigInteger& BigInteger::operator-=(const BigInteger& integer) {
  return *this = *this - integer;
}

BigInteger operator*(const BigInteger& left_integer, const BigInteger& right_integer) {
  BigInteger integer;
  if (!left_integer || !right_integer) {
    return {0};
  }

  for (int i = 0; i < right_integer.Size(); i++) {
    integer += left_integer.ShortMultiplication(right_integer[i]).Shift(i);
  }

  integer.is_negative_ = left_integer.IsNegative() ^ right_integer.IsNegative();
  if (integer.Size() > BigInteger::kMaxArraySize) {
    throw BigIntegerOverflow{};
  }

  return integer;
}

BigInteger& BigInteger::operator*=(const BigInteger& integer) {
  return *this = *this * integer;
}

BigInteger BigInteger::DivideByTwo() const {
  BigInteger integer = *this;
  int remain = 0;
  int prev_remain = 0;
  for (int i = integer.Size() - 1; i >= 0; i--) {
    prev_remain = integer[i] % 2;
    integer.number_[i] = (remain * BigInteger::kNumberSystem + integer[i]) / 2;  // NOLINT
    remain = prev_remain;
  }
  return integer;
}

BigInteger operator/(const BigInteger& left_integer, const BigInteger& right_integer) {

  if (!right_integer) {
    throw BigIntegerDivisionByZero{};
  }
  BigInteger left_abs = left_integer;
  left_abs.is_negative_ = false;

  BigInteger right_abs = right_integer;
  right_abs.is_negative_ = false;

  if (left_abs < right_abs) {
    return {0};
  }

  BigInteger left_bound = {0};
  BigInteger right_bound = left_abs;
  while (left_bound < 1 + right_bound) {
    BigInteger middle = (left_bound + right_bound).DivideByTwo();
    if (middle * right_abs > left_abs) {
      right_bound = middle - 1;
    } else {
      left_bound = middle + 1;
    }
  }
  right_bound.is_negative_ = left_integer.IsNegative() ^ right_integer.IsNegative();
  return right_bound;
}

BigInteger& BigInteger::operator/=(const BigInteger& integer) {
  return *this = *this / integer;
}

BigInteger operator%(const BigInteger& left_integer, const BigInteger& right_integer) {
  return left_integer - right_integer * (left_integer / right_integer);
}

BigInteger& BigInteger::operator%=(const BigInteger& integer) {
  return *this = *this % integer;
}

BigInteger& BigInteger::operator++() {
  return *this = *this + BigInteger(1);
}
BigInteger& BigInteger::operator--() {
  return *this = *this - BigInteger(1);
}

BigInteger BigInteger::operator++(int) {
  auto temp = *this;
  *this += 1;
  return temp;
}

BigInteger BigInteger::operator--(int) {
  auto temp = *this;
  *this -= 1;
  return temp;
}

BigInteger::operator bool() const {
  return *this != BigInteger(0);
}
