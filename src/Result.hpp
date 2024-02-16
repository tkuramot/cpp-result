#pragma once

/*
 * Result class is a simple implementation of the Result type in Rust.
 * It can be used to return either a value or an error from a function.
 */

template <typename T, typename E>
class Result {
 public:
  Result(T value, E error, bool has_value)
      : value_(value), error_(error), has_value_(has_value) {}
  T Unwrap() const {
    if (!IsOk()) {
      throw std::runtime_error("Result does not have a value");
    }
    return value_;
  }
  E UnwrapErr() const {
    if (!IsErr()) {
      throw std::runtime_error("Result does not have an error");
    }
    return error_;
  }
  bool IsOk() const { return has_value_; }
  bool IsErr() const { return !has_value_; }

 private:
  T value_;
  E error_;
  bool has_value_;
};

namespace result {

/*
 * Value and Error are helper classes to create Result instances.
 */

template <typename T>
class Value {
 public:
  explicit Value(T value) : value_(value) {}
  template <typename V, typename E>
  operator Result<V, E>() const {
    return Result<V, E>(value_, E(), /* has_value= */ true);
  }

 private:
  T value_;
};

template <typename E>
class Error {
 public:
  explicit Error(E error) : error_(error) {}
  template <typename V, typename F>
  operator Result<V, F>() const {
    return Result<V, F>(V(), error_, /* has_value= */ false);
  }

 private:
  E error_;
};

}  // namespace result

/*
 * Helper functions to create Value and Error instances.
 * These functions can be used to create Result instances without
 * explicitly specifying the template parameters.
 */

template <typename T>
result::Value<T> Success(T value) {
  return result::Value<T>(value);
}

template <typename E>
result::Error<E> Failure(E error) {
  return result::Error<E>(error);
}
