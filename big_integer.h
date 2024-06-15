//
// Created by Алексей on 14.06.2024.
//
#define BIG_INTEGER_DIVISION_IMPLEMENTED
#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <complex>
#include <cstdint>
#include <vector>

class BigIntegerOverflow : public std::exception {
  const char* what() const noexcept override {
    return "BigIntegerOverflow";
  }
};

class BigIntegerDivisionByZero : public std::exception {
  const char* what() const noexcept override {
    return "BigIntegerDivisionByZero";
  }
};

class BigInteger {
 private:
  std::vector<int16_t> number_;
  bool is_negative_;

 public:
  int16_t operator[](int n) const;
  static const int16_t kNumberSystem = 10000;
  static const int kMaxArraySize = 7500;

  BigInteger DivideByTwo() const;
  int Size() const;
  BigInteger ShortMultiplication(const int16_t&) const;
  BigInteger Shift(int);

  BigInteger();
  BigInteger(int64_t);  // NOLINT
  BigInteger(int);      // NOLINT
  explicit BigInteger(const char*);
  bool IsNegative() const;
  BigInteger operator+() const;
  BigInteger operator-() const;
  BigInteger& operator++();
  BigInteger& operator--();
  BigInteger operator++(int);
  BigInteger operator--(int);

  friend BigInteger operator+(const BigInteger&, const BigInteger&);
  friend BigInteger operator-(const BigInteger&, const BigInteger&);
  friend BigInteger operator*(const BigInteger&, const BigInteger&);
  friend BigInteger operator/(const BigInteger&, const BigInteger&);
  friend BigInteger operator%(const BigInteger&, const BigInteger&);

  BigInteger& operator+=(const BigInteger&);
  BigInteger& operator-=(const BigInteger&);
  BigInteger& operator*=(const BigInteger&);
  BigInteger& operator/=(const BigInteger&);
  BigInteger& operator%=(const BigInteger&);

  friend std::istream& operator<<(std::istream&, BigInteger&);

  bool operator>(const BigInteger&) const;
  bool operator>=(const BigInteger&) const;
  bool operator<(const BigInteger&) const;
  bool operator<=(const BigInteger&) const;
  bool operator==(const BigInteger&) const;
  bool operator!=(const BigInteger&) const;
  explicit operator bool() const;
};

BigInteger operator+(const BigInteger& left_integer, const BigInteger& right_integer);
BigInteger operator-(const BigInteger& left_integer, const BigInteger& right_integer);
BigInteger operator*(const BigInteger& left_integer, const BigInteger& right_integer);
BigInteger operator/(const BigInteger& left_integer, const BigInteger& right_integer);
BigInteger operator%(const BigInteger& left_integer, const BigInteger& right_integer);

std::istream& operator>>(std::istream&, BigInteger&);
std::ostream& operator<<(std::ostream&, const BigInteger&);

#endif  // BIG_INTEGER_H