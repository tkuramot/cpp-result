#pragma once

#include <stdexcept>

/*
 * RustライクなResultクラス
 * 値とエラーのどちらか一方を持つ
 */

template <typename OkT, typename ErrT>
class Result {
 public:
  Result();

  Result(OkT value, ErrT error, bool has_value);

  Result(const Result &other);

  Result &operator=(const Result &other);

  ~Result();

  bool operator<(const Result &other) const;

  // Errに対してUnwrapを呼ぶと例外を投げる
  // IsOkかIsErrで値の有無を確認してから呼び出す
  OkT Unwrap() const;

  // Okに対してUnwrapErrを呼ぶと例外を投げる
  // IsOkかIsErrで値の有無を確認してから呼び出す
  ErrT UnwrapErr() const;

  bool IsOk() const;

  bool IsErr() const;

 private:
  OkT value_;
  ErrT error_;
  bool has_value_;
};

namespace details {

/*
 * ValueとErrorクラスはResultクラスに変換するためのキャスト演算子を持つ
 * これによってResultクラスを生成するためのヘルパー関数を作成できる
 */

template <typename T>
class Value {
 public:
  explicit Value(T value) : value_(value);

  Value(const Value &other);

  Value &operator=(const Value &other);

  ~Value();

  template <typename U, typename F>
  operator Result<U, F>() const;

 private:
  T value_;
};

template <typename E>
class Error {
 public:
  explicit Error(E error);

  Error(const Error &other);

  Error &operator=(const Error &other);

  ~Error();

  template <typename U, typename F>
  operator Result<U, F>() const;

 private:
  E error_;
};

}  // namespace details

/*
 * Resultクラスを使いやすくするためのヘルパー関数
 * OkとErr関数はValueとErrorクラスを生成するための関数
 * これによってテンプレートパラメータを指定しなくても暗黙の型変換でResultクラスを生成できる
 */

template <typename T>
details::Value<T> Ok(T value) {
  return details::Value<T>(value);
}

template <typename E>
details::Error<E> Err(E error) {
  return details::Error<E>(error);
}

/*
 * Resultクラスの実装
 */

template <typename OkT, typename ErrT>
Result<OkT, ErrT>::Result()
    : value_(OkT()), error_(ErrT()), has_value_(false) {}

template <typename OkT, typename ErrT>
Result<OkT, ErrT>::Result(OkT value, ErrT error, bool has_value)
    : value_(value), error_(error), has_value_(has_value) {}

template <typename OkT, typename ErrT>
Result<OkT, ErrT>::Result(const Result &other)
    : value_(other.value_),
      error_(other.error_),
      has_value_(other.has_value_) {}

template <typename OkT, typename ErrT>
Result<OkT, ErrT> &Result<OkT, ErrT>::operator=(const Result &other) {
  if (this == &other) {
    return *this;
  }

  value_ = other.value_;
  error_ = other.error_;
  has_value_ = other.has_value_;
}

template <typename OkT, typename ErrT>
Result<OkT, ErrT>::~Result() {}

template <typename OkT, typename ErrT>
bool Result<OkT, ErrT>::operator<(const Result &other) const {
  if (has_value_) {
    return value_ < other.value_;
  } else {
    return error_ < other.error_;
  }
}

template <typename OkT, typename ErrT>
OkT Result<OkT, ErrT>::Unwrap() const {
  if (!IsOk()) {
    throw std::runtime_error("Result does not have a value");
  }
  return value_;
}

template <typename OkT, typename ErrT>
ErrT Result<OkT, ErrT>::UnwrapErr() const {
  if (!IsErr()) {
    throw std::runtime_error("Result does not have an error");
  }
  return error_;
}

template <typename OkT, typename ErrT>
bool Result<OkT, ErrT>::IsOk() const {
  return has_value_;
}

template <typename OkT, typename ErrT>
bool Result<OkT, ErrT>::IsErr() const {
  return !has_value_;
}

namespace details {

/*
 * Valueクラスの実装
 */

template <typename T>
Value<T>::Value(T value) : value_(value) {}

template <typename T>
Value<T>::Value(const Value &other) : value_(other.value_) {}

template <typename T>
Value<T> &Value<T>::operator=(const Value &other) {
  if (this == &other) {
    return *this;
  }
  value_ = other.value_;
}

template <typename T>
Value<T>::~Value() {}

template <typename T>
template <typename U, typename F>
Value<T>::operator Result<U, F>() const {
  return Result<U, F>(value_, F(), /* has_value= */ true);
}

/*
 * Errorクラスの実装
 */

template <typename E>
Error<E>::Error(E error) : error_(error) {}

template <typename E>
Error<E>::Error(const Error &other) : error_(other.error_) {}

template <typename E>
Error<E> &Error<E>::operator=(const Error &other) {
  if (this == &other) {
    return *this;
  }
  error_ = other.error_;
}

template <typename E>
Error<E>::~Error() {}

template <typename E>
template <typename U, typename F>
Error<E>::operator Result<U, F>() const {
  return Result<U, F>(U(), error_, /* has_value= */ false);
}

}  // namespace details