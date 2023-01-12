#pragma once
#include "File.h"

// Forward declaration of the File class
class File;

struct FileNode {
  File* file;
  std::vector<FileNode*> children;
};

enum class Type {
  REGULAR_FILE,
  DIRECTORY,
  SYMLINK,
  HARDLINK,
  NAMED_PIPE,
};