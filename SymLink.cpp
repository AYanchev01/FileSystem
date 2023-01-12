#include "SymLink.h"

SymLink::SymLink(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
                 std::time_t last_metadata_change, int hard_link_count, int size, File* target)
    : File(name, serial_num, last_access, last_data_change, last_metadata_change, hard_link_count, size, Type::SYMLINK),
      target_(target) {}

File* SymLink::getTarget() const {
  return target_;
}

void SymLink::setTarget(File* target) {
  target_ = target;
}