#pragma once
#include <string>
#include <ctime>
#include <vector>
#include "utils.h"
#include <iostream>

enum class Type;

class File {
public:
  // Constructor
  File(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
       std::time_t last_metadata_change, int size, Type type);

  // Getters
  std::string getName() const;
  int getSerialNum() const;
  std::time_t getLastAccessTime() const;
  std::time_t getLastDataChangeTime() const;
  std::time_t getLastMetadataChangeTime() const;
  int getSize() const;
  Type getType() const;

  // Setters
  void setName(const std::string& name);
  void setLastAccessTime(std::time_t last_access);
  void setLastDataChangeTime(std::time_t last_data_change);
  void setLastMetadataChangeTime(std::time_t last_metadata_change);
  void setSize(int size);
  void setType(const Type& type);

  virtual ~File() { }
private:
  std::string name_;
  int serial_num_;
  std::time_t last_access_;
  std::time_t last_data_change_;
  std::time_t last_metadata_change_;
  int size_;
  Type type_;
};
