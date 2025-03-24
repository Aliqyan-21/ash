#include "ash.h"

/* constructor
constructor to initialize the ash class */
Ash::Ash() : history_idx(-1) {}

/* get_char
this function is to get character input without pressing enter */
char Ash::get_char() {
  struct termios oldt, newt;
  char ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

/* read line function
this function read lines from the buffer, supports arrow keys and history */
std::string Ash::read_line() {
  std::string line;
  char ch;
  size_t cursor_pos{0};

  while (true) {
    ch = get_char();

    // newline
    if (ch == '\n') {
      std::cout << std::endl;
      if (!line.empty()) {
        history.push_back(line);
        history_idx = history.size();
      }
      return line;
    }
    // backspace
    else if (ch == 127) {
      if (cursor_pos > 0) {
        line.erase(line.begin() + cursor_pos - 1);
        cursor_pos--;

        std::cout << "\r> ";
        std::cout << line << " ";
        std::cout << "\r> ";

        // move the cursor too at the right position
        for (size_t i = 0; i < cursor_pos; ++i) {
          std::cout << line[i];
        }
      }
    }
    // escape sequence (arrow keys)
    else if (ch == 27) {
      if (get_char() == '[') {
        char arrow = get_char();
        // we will handle up, down, right and left arrow now
        if (arrow == 'A' && history_idx > 0) {
          history_idx--;
          line = history[history_idx];
          cursor_pos = line.size();
          std::cout << "\r\033[K> " << line;
        } else if (arrow == 'B' && history_idx < history.size() - 1) {
          history_idx++;
          line = history_idx < history.size() ? history[history_idx] : "";
          cursor_pos = line.size();
          std::cout << "\r\033[K> " << line;
        } else if (arrow == 'C' && cursor_pos < line.size()) {
          std::cout << "\033[C";
          cursor_pos++;
        } else if (arrow == 'D' && cursor_pos > 0) {
          std::cout << "\033[D";
          cursor_pos--;
        }
      }
    } else { // normal chars -> nothing special
      line.insert(line.begin() + cursor_pos, ch);
      std::cout << "\r> ";
      std::cout << line;

      cursor_pos++;
      std::cout << "\r> ";

        // move the cursor too at the right position
      for (size_t i = 0; i < cursor_pos; ++i) {
        std::cout << line[i];
      }
    }
  }
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
  signal(SIGTSTP,
         SIG_IGN); // for now we just ignore ctrl+z so it does not suspend shell
}
