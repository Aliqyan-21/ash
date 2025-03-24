#ifndef ASH_EXECVP_H

#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <vector>

namespace ash {
std::string find_executable(const std::string &cmd);
int execvp(const std::string &command, const std::vector<std::string> &args);
int execv(const std::string &full_path, const std::vector<std::string> &args);
int execve(const char *path_name, char *const argv[], char *const envp[]);
} // namespace ash

#endif // !ASH_EXECVP_H
