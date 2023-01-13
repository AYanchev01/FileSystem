#pragma once
#include "File.h"
#include <string>
#include <mutex>
#include <queue>

class NamedPipe : public File {
public:
  // Constructor
  NamedPipe(const std::string& name, int serial_num, std::time_t last_access, std::time_t last_data_change,
            std::time_t last_metadata_change, int hard_link_count, int size);

  // Read data from the pipe
  std::string read();

  // Write data to the pipe
  void write(const std::string& data);

private:
  std::mutex mutex_;  // Mutex to protect the data queue
  std::queue<std::string> data_queue_;  // Queue of data written to the pipe
};