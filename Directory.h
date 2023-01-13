#pragma once
#include "utils.h"
#include "File.h"
#include <vector>
#include <algorithm>

class Directory : public File {
public:
  // Constructor
  Directory(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
            std::time_t last_metadata_change, int hard_link_count, int size, Directory* parent = nullptr);

  // Add a file or directory to this directory
  void addEntry(File* entry);

  // Remove a file or directory from this directory
  void removeEntry(const std::string& name);

  // Get a file or directory from this directory
  File* getEntry(const std::string& name) const;

  // Get a reference to the children of this directory
  std::vector<File*>& getChildren();


  // Get the parent of this directory
  Directory* getParent() const;

  // Check if this directory is empty
  bool isEmpty() const;

private:
  //std::unordered_map<std::string, FileNode*> entries_;
  std::vector<File*> entries_;
  Directory* parent_;
};
