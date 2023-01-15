#include "../include/CLI.h"
#include "../include/FileSystem.h"
#include "../tests/tests.h"

int main(int argc, char* argv[]) {
  // Create the file system
  FileSystem fs;

  // Create the command-line interface
  CLI cli(fs);

  if (argc > 1 && std::string(argv[1]) == "--test") {
    // Run the tests
    runTests();
  } else {
    // Run the command-line interface
    cli.run();
  }

  return 0;
}