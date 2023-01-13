#include "../include/NamedPipe.h"

NamedPipe::NamedPipe(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
                     std::time_t last_metadata_change, int hard_link_count, int size)
    : File(name, serial_num, last_access, last_data_change, last_metadata_change, hard_link_count, size, Type::NAMED_PIPE) {}

std::string NamedPipe::read() {
  std::string data;
  {
    // Lock the mutex and retrieve the next piece of data from the queue
    std::lock_guard<std::mutex> lock(mutex_);
    if (!data_queue_.empty()) {
      data = data_queue_.front();
      data_queue_.pop();
    }
  }
  return data;
}

void NamedPipe::write(const std::string& data) {
  // Lock the mutex and add the data to the queue
  std::lock_guard<std::mutex> lock(mutex_);
  data_queue_.push(data);
}
