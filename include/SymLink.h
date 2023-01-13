#pragma once
#include "File.h"
#include <string>

class SymLink : public File {
public:
  // Constructor
  SymLink(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
          std::time_t last_metadata_change, int hard_link_count, int size, File* target);

  // Get the target of the symbolic link
  File* getTarget() const;

  // Set the target of the symbolic link
  void setTarget(File* target);

private:
  File* target_;
};
