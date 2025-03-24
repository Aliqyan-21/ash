#ifndef ASH

#include <iostream>
#include <termios.h>
#include "ash_execvp.h"
#include <string>
#include <vector>

class Ash {
public:
  Ash();
  void run();
  void setup_signal_handling();

private:
  std::vector<std::string> history;
  size_t history_idx;

  std::string read_line();
  std::vector<std::string> split_line(std::string line);
  int ash_exit();
  int ash_execute(std::vector<std::string> args);
  static void handle_sigint(int sig);
  char get_char();
};

#endif // !ASH
