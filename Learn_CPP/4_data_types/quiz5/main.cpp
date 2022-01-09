#include <cstdint>
#include <iostream>

int main()
{
  std::cout << "How old are you?\n";

  std::int16_t age{};
  std::cin >> age;

  std::cout << "Allowed to drive a car in Texas [";

  if (age >= 16)
    std::cout << "x";
  else
    std::cout << " ";

  std::cout << "]\n";

  return 0;
}