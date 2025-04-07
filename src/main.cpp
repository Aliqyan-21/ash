#include "ash.h"

int main(void) {
  Ash ash;
  ash.setup_signal_handling();
  ash.run();
  return 0;
}
