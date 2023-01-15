#include "../include/SymLink.h"

SymLink::SymLink(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
                 std::time_t last_metadata_change, int size, File* target, bool isBroken)
    : File(name, serial_num, last_access, last_data_change, last_metadata_change, size, Type::SYMLINK),
      target_(target), isBroken_(isBroken) {}

/**
 * @brief Get the Target of the SymLink
 * 
 * @return File* - the target of the SymLink
 */
File* SymLink::getTarget() {
  return target_;
}

/**
 * @brief Returns true if the SymLink is broken
 * 
 * @return bool - true if the SymLink is broken 
 */
bool SymLink::isBroken() const {
  return isBroken_;
}

/**
 * @brief Set the Target of the SymLink
 * 
 * @param target - the target of the SymLink
 */
void SymLink::setTarget(File* target) {
  target_ = target;
}

/**
 * @brief Set the Broken status of the SymLink
 * 
 * @param isBroken - the broken status of the SymLink
 */
void SymLink::setBroken(bool isBroken) {
  isBroken_ = isBroken;
}