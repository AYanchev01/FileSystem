#include "Directory.h"

Directory::Directory(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
            std::time_t last_metadata_change, int hard_link_count, int size, Directory* parent)
      : File(name, serial_num, last_access, last_data_change, last_metadata_change, hard_link_count, size, Type::DIRECTORY),
        parent_(parent) {}

void Directory::addEntry(FileNode* entry) {
  entries_[entry->file->getName()] = entry;
}

void Directory::removeEntry(const std::string& name) {
  entries_.erase(name);
}

FileNode* Directory::getEntry(const std::string& name) const {
    auto it = entries_.find(name);
    return it == entries_.end() ? nullptr : it->second;
}

std::unordered_map<std::string, FileNode*>& Directory::getChildren() {
  return entries_;
}

Directory* Directory::getParent() const {
  return parent_;
}

bool Directory::isEmpty() const {
  return entries_.empty();
}
