#include "Directory.h"

Directory::Directory(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
            std::time_t last_metadata_change, int hard_link_count, int size, Directory* parent)
      : File(name, serial_num, last_access, last_data_change, last_metadata_change, hard_link_count, size, Type::DIRECTORY),
        parent_(parent) {}

void Directory::addEntry(File* entry) {
  entries_.push_back(entry);
}

void Directory::removeEntry(const std::string& name) {
  for (auto entry: entries_) {
    if (entry->getName() == name) {
      entries_.erase(std::remove(entries_.begin(), entries_.end(), entry), entries_.end());
      break;
    }
  }
}

File* Directory::getEntry(const std::string& name) const{
  for (auto entry: entries_) {
    if (entry->getName() == name) {
      return entry;
    }
  }
  return nullptr;
}

std::vector<File*>& Directory::getChildren() {
  return entries_;
}

Directory* Directory::getParent() const {
  return parent_;
}

bool Directory::isEmpty() const {
  return entries_.empty();
}
