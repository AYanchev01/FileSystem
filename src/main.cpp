#include "../include/CLI.h"
#include "../include/FileSystem.h"
#include "../tests/tests.h"

int main(int argc, char* argv[]) {
  // Create the file system
  FileSystem fs;

  // Create the command-line interface
  CLI cli(fs);

  if (argc == 1)
  {
    // Run the command-line interface
    cli.run();
  }
  else if (argc == 2 && std::string(argv[1]) == "--test")
  {
    // Run the tests
    runTests();
  }
  else
  {
    std::cout << "Usage: .\\filesystem.exe [--test]" << std::endl;
  }

  return 0;
}