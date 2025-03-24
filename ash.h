#ifndef ASH

#include <iostream>
#include <string>
#include <vector>

class Ash {
public:
  Ash();
  void run();
  void setup_signal_handling();

private:
  std::string read_line();
  std::vector<std::string> split_line(std::string line);
  int ash_exit();
  int ash_execute(std::vector<std::string> args);
  static void handle_sigint(int sig);
};

#endif // !ASH
