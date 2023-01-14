#pragma once
#include "File.h"
#include "Directory.h"
#include "SymLink.h"
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cstddef>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include "RegularFile.h"


// Forward declaration of the File class
class File;

class FileSystem {
public:
  // Constructor
  FileSystem();

  // Destructor
  ~FileSystem();

  // Add a file to the file system tree
  void addFile(File* file, const std::string& path);

  // Delete a file from the file system tree
  void deleteFile(const std::string& path);

  // Change the current working directory
  void changeDirectory(const std::string& path);

  // Get the current working directory
  std::string getCurrentDirectory() const;

  // Get a file from the file system tree
  File* getFile(const std::string& path) const;

  // Get the root directory
  Directory*& getRootDirectory();

  // void mount(const std::string& path, FileSystem& fs);

  // void umount(const std::string& path);

public:
  // Recursively delete all the nodes in a subtree
  void deleteEntry(File*& node);
  std::vector<std::string> splitPath(const std::string& path) const;
  bool isValidPath(const std::string& path) const;

  Directory* root_;  // The root directory of the file system tree
  Directory* cwd_;  // The current working directory
};

