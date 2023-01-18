#include "../include/Directory.h"

Directory::Directory(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
            std::time_t last_metadata_change, int size, Directory* parent)
      : File(name, serial_num, last_access, last_data_change, last_metadata_change, size, Type::DIRECTORY),
        parent_(parent) {}


/**
 * @brief Add a Entry to the directory
 * 
 */
void Directory::addEntry(File* entry) {
  
  if(entry == nullptr) {
    std::cout << "Invalid file" << std::endl;
    return;
  }
  entries_.push_back(entry);
  this->setSize(this->getSize() + entry->getSize());
  this->setLastAccessTime(std::time(nullptr));
  this->setLastDataChangeTime(std::time(nullptr));
}

/**
 * @brief Remove a Entry from the directory
 * 
 * @param name - name of the entry
 */
void Directory::removeEntry(const std::string& name) {
  for (auto entry: entries_) {
    if (entry->getName() == name) {
      entries_.erase(std::remove(entries_.begin(), entries_.end(), entry), entries_.end());
      this->setLastDataChangeTime(std::time(nullptr));
      break;
    }
  }
  this->setLastAccessTime(std::time(nullptr));
}

/**
 * @brief Get a Entry from the directory
 * 
 * @param name - name of the entry
 * @return File* - pointer to the entry
 */
File* Directory::getEntry(const std::string& name) const{
  for (auto entry: entries_) {
    if (entry->getName() == name) {
      return entry;
    }
  }
  return nullptr;
}

/**
 * @brief Get the Children of the directory
 * 
 * @return std::vector<File*> 
 */
std::vector<File*>& Directory::getChildren() {
  return entries_;
}

/**
 * @brief Get the Parent directory of the directory
 * 
 * @return Directory* 
 */
Directory* Directory::getParent() const {
  return parent_;
}

/**
 * @brief Return true if the directory is empty
 * 
 * @return bool - true if the directory is empty else false
 */
bool Directory::isEmpty() const {
  return entries_.empty();
}
