#ifndef ASH

#include <iostream>
#include <string>
#include <vector>

class Ash {
public:
  Ash();
  void run();

private:
  std::string read_line();
  std::vector<std::string> split_line(std::string line);
  int ash_exit();
  int ash_execute(std::vector<std::string> args);
};

#endif // !ASH
