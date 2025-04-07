#include "ash_execvp.h"

extern char **environ;

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

/* execve function
and finally our last function in our implementation stack of highest level down
to lowest, i.e. the system call level */
int execve(const char *path_name, char *const argv[], char *const envp[]) {
  return syscall(SYS_execve, path_name, argv, envp);
}

/* execv funtion
the custom implementation of execv funtion now, that executes the
program at the given path with the given args this is implementation
from scratch that makes the syscall directly */
int execv(const std::string &full_path, const std::vector<std::string> &args) {
  char **argv = new char *[args.size() + 1];

  argv[0] = strdup(full_path.c_str());

  for (size_t i = 1; i < args.size(); ++i) {
    argv[i] = strdup(args[i].c_str());
  }

  argv[args.size()] = nullptr;

  // execve is a system call replaces the current process image with new one
  int result = ash::execve(full_path.c_str(), argv, environ);

  // if we get here then execve failed
  for (size_t i = 0; i < args.size(); ++i) {
    free(argv[i]);
  }
  delete[] argv;

  return result;
}

/* execvp function
the custom implementation of execvp function that executes the cmd and their
args by using exec */
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

    ash::execv(exec_path.c_str(), args);
    perror("Exec Failed");
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
  }
}
} // namespace ash
