#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// Forward declaration of the FileSystem class
class FileSystem;

class CLI {
public:
  // Constructor
  CLI(FileSystem& fs);

  // Run the command-line interface
  void run();

  // Change the current working directory
  void cd(const std::vector<std::string>& args);

  // Print the current working directory
  void pwd();

  // List the files in a directory
  std::string ls(const std::vector<std::string>& args);

  // Print the contents of a file
  std::string cat(const std::vector<std::string>& args);

  // Copy a file
  void cp(const std::vector<std::string>& args);

  // Remove a file
  void rm(const std::vector<std::string>& args);

  // Create a directory
  void mkdir(const std::vector<std::string>& args);

  // Remove a directory
  void rmdir(const std::vector<std::string>& args);

  // Create a symbolic link
  void ln(const std::vector<std::string>& args);

  // Get information about a file
  void stat(const std::vector<std::string>& args);

  // Split a line into components
  std::vector<std::string> splitLine(const std::string& line) const;

  FileSystem& fs_;  // The file system
};