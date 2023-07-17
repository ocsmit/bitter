#ifndef TESTS_H_
#define TESTS_H_
/* 
 * Usage:
 *   #include "tests.h"
 *
 *   BEGIN_TESTING
 *
 *   TEST("Passing test") {
 *      assert(1 == 1)
 *   }
 *
 *   TEST("Failing test") {
 *    assert(1 == 2)
 *    }
 *
 *  END_TESTING
 *
 *
 */

#include <string.h>


// TODO: Add printing to indicate test completion and logging

#define BEGIN_TESTING int main(int argc, char **argv) {
#define END_TESTING return 0; }
#define TEST(TEST_NAME) if (run_test(TEST_NAME, argc, argv))

#define CHECK_EQUAL_INT(EXPECTED, ACTUAL) \
          if (EXPECTED != ACTUAL) \
            { \
              fprintf (stderr, \
                       "%s, line %d: For '%s', expected %d, got %d\n", \
                       __FILE__, __LINE__, #ACTUAL, EXPECTED, ACTUAL); \
            } \

int run_test(const char* test_name, int argc, char **argv)
{
  // Run all tests by default
  if (argc == 1) return 1;

  // Run specified test
  for (int i = 1; i < argc; i++) {
    if (!strcmp(test_name, argv[i])) return 0;
  }
  return 0;
}


#endif // !TESTS_H_
