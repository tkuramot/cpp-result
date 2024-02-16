//
// Created by k.t. on 2024/02/16.
//

#include <iostream>
#include <string>

#include "Result.hpp"
Result<int, std::string> Divide(int a, int b) {
  if (b == 0) {
    return Failure("Division by zero");
  }
  return Success(a / b);
}

int main() {
  Result<int, std::string> result = Divide(10, 0);
  if (result.IsOk()) {
    std::cout << "Result: " << result.Unwrap() << std::endl;
  } else {
    std::cout << "Error: " << result.UnwrapErr() << std::endl;
  }
  return 0;
}