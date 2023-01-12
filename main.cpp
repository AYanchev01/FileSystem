#include "CLI.h"
#include "FileSystem.h"

int main() {
  // Create the file system
  FileSystem fs;

  // Create the command-line interface
  CLI cli(fs);

  // Run the command-line interface
  cli.run();

  return 0;
}