#include "ash.h"
#include "ash_execvp.h"
#include <csignal>

/* constructor
constructor to initialize the ash class */
Ash::Ash() {}

/* read line function
this function read lines from the buffer */
std::string Ash::read_line() {
  std::string line;
  getline(std::cin, line);
  return line;
}

/* split line function
this function takes a string line as argument and divide it into tokens
divided with space as delimiter */
std::vector<std::string> Ash::split_line(std::string line) {
  std::vector<std::string> toks;
  std::string word;

  for (const char c : line) {
    if (std::isspace(c) || c == '\n') {
      toks.push_back(word);
      word.clear();
    } else {
      word += c;
    }
  }
  toks.push_back(word);
  return toks;
}

/* ash_exit function
a trivial function to exit our shell if user enters exit or something */
int Ash::ash_exit() { return 0; }

/* ash_execute function
this function helps us executre the command by forking the process using
execvp and fork */
int Ash::ash_execute(std::vector<std::string> args) {
  if (args.empty())
    return 1;

  if (args[0] == "exit") {
    return ash_exit();
  }

  int status = ash::execvp(args[0], args);
  if (status < 0) {
    std::cerr << "ASH: Command not found: " << args[0] << std::endl;
  }

  return 1;
}

/* main loop
so this whill be our main loop in which we will let user know that ash is ready
to take prompt by printing '> ' */
void Ash::run() {
  std::string line;
  std::vector<std::string> args;
  int status = 1;

  do {
    std::cout << "> ";
    line = read_line();
    args = split_line(line);
    status = ash_execute(args);
  } while (status != 0);
}

/* handle_signal function
handle_signal function is to handle ctrl+c (SIGNINT) signal */
void Ash::handle_sigint(int sig) {
  if (sig == SIGINT) {
    std::cout << "\n> ";
    std::cout.flush();
  }
}

/* setup_signal_handling function
public function to make signal handling available
this function is to handle ctrl+c (SIGNINT) and ctrl+z (SIGTSTP) */
void Ash::setup_signal_handling() {
  signal(SIGINT, handle_sigint);
  signal(SIGTSTP, SIG_IGN); // for now we just ignore ctrl+z so it does not suspend shell
}
