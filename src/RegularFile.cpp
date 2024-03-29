#include "../include/RegularFile.h"

RegularFile::RegularFile(const std::string& name,int serial_num, std::time_t last_access, std::time_t last_data_change,
    std::time_t last_metadata_change, int size, Type type)
    : File(name, serial_num, last_access, last_data_change, last_metadata_change, size, type) {
}

/**
 * @brief Get the Contents of the file
 * 
 * @return std::string - the contents of the file
 */
std::string RegularFile::getContents() const {
    return contents_;
}

/**
 * @brief Set the Contents of the file
 * 
 * @param contents - the contents of the file
 */
void RegularFile::setContents(const std::string& contents) {
    contents_ = contents;
}