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

Result<void, std::string> SuccessInHalf(int a) {
  if (a % 2 != 0) {
    return Err("Not an even number");
  }
  return Ok();
}

int main() {
  Result<int, std::string> result = Divide(10, 0);
  if (result.IsOk()) {
    std::cout << "Result: " << result.Unwrap() << std::endl;
  } else {
    std::cout << "Error: " << result.UnwrapErr() << std::endl;
  }

  Result<void, std::string> result2 = SuccessInHalf(10);
  if (result2.IsOk()) {
    std::cout << "Success" << std::endl;
  } else {
    std::cout << "Error: " << result2.UnwrapErr() << std::endl;
  }
  return 0;
}