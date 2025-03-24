#include <iostream>
#include <sys/wait.h>
#include <vector>

/* read line function
this function read lines from the buffer */
std::string read_line() {
  std::string line;
  getline(std::cin, line);
  return line;
}

/* split line function
this function takes a string line as argument and divide it into tokens
divided with space as delimiter */
std::vector<std::string> split_line(std::string line) {
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
int ash_exit() { return 0; }

/* ash_execvp function
here we implemented our own execvp function for abstraction, why?
because original execvp function takes this *args and **argv as arguments
and we have vector and string so for that we made this*/
int ash_execvp(const std::string &command,
               const std::vector<std::string> &args) {
  std::vector<char *> c_args;
  c_args.reserve(args.size() + 2); // args + command + NULL

  for (const std::string &arg : args) {
    c_args.push_back(const_cast<char *>(arg.c_str()));
  }
  c_args.push_back(nullptr);

  // now we can run the original execvp function
  return execvp(c_args[0], c_args.data());
}

/* ash_execute function
this function helps us executre the command by forking the process using
execvp and fork */
int ash_execute(std::vector<std::string> args) {
  if (args.empty())
    return 1;
  int status;

  if (args[0] == "exit") {
    return ash_exit();
  }

  pid_t cpid = fork();

  if (cpid == 0) {
    if (ash_execvp(args[0], args) < 0) {
      std::cerr << "Ash comand not found: " << args[0] << std::endl;
      exit(EXIT_FAILURE);
    }
  } else if (cpid < 0) {
    std::cerr << "Error Forking" << std::endl;
  } else {
    waitpid(cpid, &status, WUNTRACED);
  }

  return 1;
}

/* main loop
so this whill be our main loop in which we will let user know that ash is ready
to take prompt by printing '> ' */
void loop() {
  std::cout << "path: " << getenv("PATH") << std::endl;
  std::cout << "shell: " << getenv("SHELL") << std::endl;
  std::string line;
  std::vector<std::string> args;
  int status = 1;

  do {
    std::cout << "> ";
    line = read_line();
    args = split_line(line);
    status = ash_execute(args);
  } while (status);
}

int main(void) {
  loop();
  return 0;
}
