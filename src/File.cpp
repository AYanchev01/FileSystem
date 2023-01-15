#include "../include/File.h"

File::File(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
     std::time_t last_metadata_change, int size, Type type) : name_(name), serial_num_(serial_num),
        last_access_(last_access), last_data_change_(last_data_change), last_metadata_change_(last_metadata_change),
        size_(size), type_(type) { }


// Getters

/**
 * @brief Get the name of the file
 * 
 * @return std::string 
 */
std::string File::getName() const {
    return name_;
}

/**
 * @brief Get the serial number of the file
 * 
 * @return int 
 */
int File::getSerialNum() const {
    return serial_num_;
}

/**
 * @brief Get the Last Access Time of the file
 * 
 * @return std::time_t 
 */
std::time_t File::getLastAccessTime() const {
    return last_access_;
}

/**
 * @brief Get the Last Data Change Time of the file
 * 
 * @return std::time_t 
 */
std::time_t File::getLastDataChangeTime() const {
    return last_data_change_;
}

/**
 * @brief Get the Last Metadata Change Time of the file
 * 
 * @return std::time_t 
 */
std::time_t File::getLastMetadataChangeTime() const {
    return last_metadata_change_;
}

/**
 * @brief Get the Size of the file
 * 
 * @return int 
 */
int File::getSize() const {
    return size_;
}

/**
 * @brief Get the Type of the file
 * 
 * @return Type 
 */
Type File::getType() const {
    return type_;
}

// Setters

/**
 * @brief Set the Name of the file
 * 
 * @param name 
 */
void File::setName(const std::string& name) {
    if (name.empty())
    {
        std::cout << "Error: File name cannot be empty" << std::endl;
        return;
    }
    name_ = name;
    this->setLastMetadataChangeTime(std::time(nullptr));
}

/**
 * @brief Set the Last Access Time of the file
 * 
 * @param last_access 
 */
void File::setLastAccessTime(std::time_t last_access) {
    last_access_ = last_access;
}

/**
 * @brief Set the Last Data Change Time of the file
 * 
 * @param last_data_change 
 */
void File::setLastDataChangeTime(std::time_t last_data_change) {
    last_data_change_ = last_data_change;
}

/**
 * @brief Set the Last Metadata Change Time of the file
 * 
 * @param last_metadata_change 
 */
void File::setLastMetadataChangeTime(std::time_t last_metadata_change) {
    last_metadata_change_ = last_metadata_change;
}

/**
 * @brief Set the Size of the file
 * 
 * @param size 
 */
void File::setSize(int size) {
    if (size < 0)
    {
        std::cout << "Error: File size cannot be negative" << std::endl;
        return;
    }
    size_ = size;
    this->setLastMetadataChangeTime(std::time(nullptr));
}

/**
 * @brief Set the Type of the file
 * 
 * @param type 
 */
void File::setType(const Type& type) {
    type_ = type;
}