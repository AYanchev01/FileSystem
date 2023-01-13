#pragma once
#include "File.h"

class RegularFile : public File {
public:
    // Constructor
    RegularFile(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
        std::time_t last_metadata_change, int hard_link_count, int size, Type type=Type::REGULAR_FILE);
    ~RegularFile() { }

    // Getters
    std::string getContents() const;

    // Setters
    void setContents(const std::string& contents);

private:
    std::string contents_;
};

