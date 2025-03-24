#include <iostream>
#include <vector>

/* main loop
so this whill be our main loop in which we will let user know that ash is ready to take prompt
by printing '> ' */
void loop() {
  std::string line;
  std::vector<std::string> args;
  int status = 1;

  do {
    std::cout << "> " ;
    line = read_line();
    flag = 0;
    args = split_line(line);
    status = dash_launch();
  } while (status);
}

int main(void) {
  return 0;
}
