#include <string>

#ifdef _WIN32
#include <windows.h>
static std::string getExecutablePath() {
  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  return std::string(buffer);
}
#elif __linux__
#include <limits.h>
#include <unistd.h>
static std::string getExecutablePath() {
  char buffer[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
  if (len != -1) {
    buffer[len] = '\0';
    return std::string(buffer);
  } else {
    return std::string();
  }
}
#else
static std::string getExecutablePath() {
  return std::string();
}
#endif
