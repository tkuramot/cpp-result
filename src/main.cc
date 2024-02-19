//
// Created by k.t. on 2024/02/16.
//

#include <iostream>
#include <string>

#include "result.h"

Result<int, std::string> Divide(int a, int b) {
  if (b == 0) {
    return Err("Division by zero");
  }
  return Ok(a / b);
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