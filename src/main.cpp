#include "../include/CLI.h"
#include "../include/FileSystem.h"
#include "../tests/tests.h"

int main() {
  // Create the file system
  FileSystem fs;

  // Create the command-line interface
  CLI cli(fs);

  // Run the command-line interface
  cli.run();

  //Run the tests
  //runTests();
  return 0;
}