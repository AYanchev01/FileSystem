#include "RegularFile.h"

RegularFile::RegularFile(const std::string& name,int serial_num, std::time_t last_access, std::time_t last_data_change,
    std::time_t last_metadata_change, int hard_link_count, int size, Type type)
    : File(name, serial_num, last_access, last_data_change, last_metadata_change, hard_link_count, size, type) {
}

std::string RegularFile::getContents() const {
    return contents_;
}

void RegularFile::setContents(const std::string& contents) {
    contents_ = contents;
}