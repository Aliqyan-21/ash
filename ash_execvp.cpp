#include "ash_execvp.h"

namespace ash {
/* find_executable function
the function that find the executables that are available in our PATH variable
*/
std::string find_executable(const std::string &cmd) {
  const char *path = getenv("PATH");

  if (!path)
    return "";

  std::stringstream ss(path);
  std::string dir;

  while (std::getline(ss, dir, ':')) {
    // constructing the full path
    std::string full_path = dir + "/" + cmd;
    if (access(full_path.c_str(), X_OK) == 0) {
      return full_path;
    }
  }

  return "";
}

/* execvp function
the implementation of execvp function that executes the cmd and their args by
using exec */
int execvp(const std::string &cmd, const std::vector<std::string> &args) {
  std::string exec_path = find_executable(cmd);

  if (exec_path.empty()) {
    return -1;
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return -1;
  } else if (pid == 0) {
    std::vector<const char *> c_args;
    c_args.push_back(exec_path.c_str());

    for (size_t i = 1; i < args.size(); ++i) {
      c_args.push_back(args[i].c_str());
    }

    c_args.push_back(nullptr); // for execv

    execv(exec_path.c_str(), const_cast<char *const *>(c_args.data()));
    perror("Exec Failed");
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
  }
}
} // namespace ash
