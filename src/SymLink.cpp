#include "../include/SymLink.h"

SymLink::SymLink(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
                 std::time_t last_metadata_change, int hard_link_count, int size, File* target, bool isBroken)
    : File(name, serial_num, last_access, last_data_change, last_metadata_change, hard_link_count, size, Type::SYMLINK),
      target_(target), isBroken_(isBroken) {}

File* SymLink::getTarget() {
  return target_;
}

bool SymLink::isBroken() const {
  return isBroken_;
}

void SymLink::setTarget(File* target) {
  target_ = target;
}

void SymLink::setBroken(bool isBroken) {
  isBroken_ = isBroken;
}