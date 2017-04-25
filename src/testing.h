#ifndef TESTING_H_
#define TESTING_H_

#include <iostream>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Super simple testing framework!
#define INIT_TEST \
        std::cout << "Starting test " << __func__ << "!" << std::endl; \
        bool passed = true;

#define EXIT_TEST \
        if (passed) { \
            std::cout << "Test " << __func__ << " passed!" << std::endl; \
        } else { \
            std::cout << "Test " << __func__ << ANSI_COLOR_RED << " FAILED!" \
                      << ANSI_COLOR_RESET << std::endl; \
        } \
        std::cout << std::endl; \
        return passed;

#define EXPECT(x) passed &= expect((x), __LINE__)
#define EXPECT_NOT(x) passed &= expect((!x), __LINE__)

inline bool expect(bool result, int line) {
  if (!result) {
    std::cout << "  EXPECT at line " << line << " returned false." << std::endl;
  }
   
  return result;
}

#endif
