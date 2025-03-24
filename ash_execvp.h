#ifndef ASH_EXECVP_H

#include <string>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace ash {
std::string find_executable(const std::string &cmd);
int execvp(const std::string &command, const std::vector<std::string> &args);
} // namespace ash

#endif // !ASH_EXECVP_H
