#include "../include/File.h"

File::File(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
     std::time_t last_metadata_change, int hard_link_count, int size, Type type) : name_(name), serial_num_(serial_num),
        last_access_(last_access), last_data_change_(last_data_change), last_metadata_change_(last_metadata_change),
        hard_link_count_(hard_link_count), size_(size), type_(type) { }


// Getters
std::string File::getName() const {
    return name_;
}
int File::getSerialNum() const {
    return serial_num_;
}
std::time_t File::getLastAccessTime() const {
    return last_access_;
}
std::time_t File::getLastDataChangeTime() const {
    return last_data_change_;
}
std::time_t File::getLastMetadataChangeTime() const {
    return last_metadata_change_;
}
int File::getHardLinkCount() const {
    return hard_link_count_;
}
int File::getSize() const {
    return size_;
}
Type File::getType() const {
    return type_;
}

// Setters
void File::setName(const std::string& name) {
    name_ = name;
}
void File::setLastAccessTime(std::time_t last_access) {
    last_access_ = last_access;
}
void File::setLastDataChangeTime(std::time_t last_data_change) {
    last_data_change_ = last_data_change;
}
void File::setLastMetadataChangeTime(std::time_t last_metadata_change) {
    last_metadata_change_ = last_metadata_change;
}
void File::setHardLinkCount(int hard_link_count) {
    hard_link_count_ = hard_link_count;
}
void File::setSize(int size) {
    size_ = size;
}
void File::setType(const Type& type) {
    type_ = type;
}