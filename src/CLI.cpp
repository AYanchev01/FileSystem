#include "../include/CLI.h"
#include "../include/FileSystem.h"

CLI::CLI(FileSystem& fs) : fs_(fs) {}

void CLI::run() {
        while (true) {
        // Print the prompt
        std::cout << fs_.getCurrentDirectory() << "$ ";

        // Read a line of input
        std::string line;
        std::getline(std::cin, line);

        // Split the line into components
        std::vector<std::string> components = splitLine(line);
        if (components.empty()) {
        // Empty line, skip
        continue;
        }

        // Dispatch the command
        if (components[0] == "cd") {
        cd(components);
        } else if (components[0] == "pwd") {
        pwd();
        } else if (components[0] == "ls") {
        ls(components);
        } else if (components[0] == "cat") {
        cat(components);
        } else if (components[0] == "cp") {
        cp(components);
        } else if (components[0] == "rm") {
        rm(components);
        } else if (components[0] == "mkdir") {
        mkdir(components);
        } else if (components[0] == "rmdir") {
        rmdir(components);
        } else if (components[0] == "ln") {
        ln(components);
        } else if (components[0] == "stat") {
        stat(components);
        // } else if (components[0] == "mount") {
        // mount(components);
        // } else if (components[0] == "umount") {
        // umount(components);
        } else if (components[0] == "exit") {
        break;
        } else {
        std::cout << "Invalid command" << std::endl;
        }
    }
}

void CLI::cd(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: cd <directory>" << std::endl;
        return;
    }
    fs_.changeDirectory(args[1]);
}


void CLI::pwd() {
  std::cout << fs_.getCurrentDirectory() << std::endl;
}

void CLI::ls(const std::vector<std::string>& args) {
  // Get the path to the directory to list
  std::string path;
  if (args.empty())
  {
    return;
  }
  if (args.size() == 1) {
    // No path was specified, use the current working directory
    path = fs_.getCurrentDirectory();
  } else {
    // Use the specified path
    path = args[1];
  }

  // Get the directory to list
  File* file = fs_.getFile(path);
  if (file == nullptr) {
    std::cout << "Error: no such file or directory" << std::endl;
    return;
  }

  if (file->getType() != Type::DIRECTORY) {
    std::cout << "Error: not a directory" << std::endl;
    return;
  }
  Directory* dir = dynamic_cast<Directory*>(file);
  // Print the names of the files in the directory
  for (auto child : dir->getChildren()) {
    std::cout << child->getName() << " ";
  }
  std::cout << std::endl;
}

void CLI::cat(const std::vector<std::string>& args) {
    if (args.empty())
    {
      return;
    }
    if (args.size() == 1) {
      std::string result = "";
        std::string line;
        while (true) {
          std::getline(std::cin, line);
          if (line == ".") break;
          result += line;
        }
        std::cout << result << std::endl;
        return;
    }
  
    // Get the file from the file system
    std::string result = "";
    for (size_t i = 1; i < args.size(); i++)
    {
      if (args[i] == ">")
      {
        if(i == 1)
        {
          std::string line;
          while (true) {
            std::getline(std::cin, line);
            if (line == ".") break;
            result += line;
          }
        }
        if (i + 1 >= args.size())
        {
          std::cout << "Error: missing output file" << std::endl;
          return;
        }
        File* output_file = fs_.getFile(args[i + 1]);
        if (output_file == nullptr) {
          fs_.addFile(new RegularFile((fs_.splitPath(args[i + 1])).back(), 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, Type::REGULAR_FILE), args[i + 1]);
          output_file = fs_.getFile(args[i + 1]);
        }
        if (output_file->getType() != Type::REGULAR_FILE) {
          std::cout << "Error: output file is not a file" << std::endl;
          return;
        }
        RegularFile* res = dynamic_cast<RegularFile*>(output_file);
        res->setContents(result);
        return;
      }


      File* file = fs_.getFile(args[i]);
      if (file == nullptr) {
        std::cout << "Error: file not found" << std::endl;
        return;
      }
      if (file->getType() != Type::REGULAR_FILE) 
      {
        if (file->getType() == Type::SYMLINK)
        {
          if (dynamic_cast<SymLink*>(file)->isBroken())
          {
            std::cout << "Error: broken symlink" << file->getName() << std::endl;
            return;
          }
          else 
          {
            if (dynamic_cast<SymLink*>(file)->getTarget() == nullptr)
            {
              std::cout << "Error: broken symlink"<< file->getName() << std::endl;
              dynamic_cast<SymLink*>(file)->setBroken(true);
              return;
            }
            else if ( dynamic_cast<SymLink*>(file)->getTarget()->getType() != Type::REGULAR_FILE)
            {
              std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
              return;
            }
          }
        }
        else 
        {
          std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
          return;
        }
      }

      RegularFile* reg_file = nullptr;
      if (file->getType() == Type::REGULAR_FILE)
      {
        reg_file = dynamic_cast<RegularFile*>(file);
      }
      else
      {
        reg_file = dynamic_cast<RegularFile*>(dynamic_cast<SymLink*>(file)->getTarget());
      }

      if(!result.empty()) { result += "\n"; }
      result += reg_file->getContents();

      if(i == args.size() - 1)
      {
        std::cout << result << std::endl;
        return;
      }
    }
}

void CLI::cp(const std::vector<std::string>& args) {
  // Check if the number of arguments is correct
  if (args.size() < 3) {
    std::cout << "cp: invalid number of arguments" << std::endl;
    std::cout << "Usage: cp <source> [<source> .. <source>] <destination>" << std::endl;
    return;
  }

  // Get the destination path
  std::string dest_path = args[args.size() - 1];

  for (size_t i = 1; i < args.size() - 1; i++)
  {
    RegularFile* file_to_add = nullptr;
    // Get the source file
    std::string src_path = args[i];
    File* src_file = fs_.getFile(src_path);

    if (src_file == nullptr) {
      std::cout << "cp: source file does not exist" << std::endl;
      return;
    }
    
    if (src_file->getType() != Type::REGULAR_FILE) 
    {
      if (src_file->getType() == Type::SYMLINK)
      {
        if (dynamic_cast<SymLink*>(src_file)->isBroken())
        {
          std::cout << "Error: broken symlink" << src_file->getName() << std::endl;
          return;
        }
        else 
        {
          if (dynamic_cast<SymLink*>(src_file)->getTarget() == nullptr)
          {
            std::cout << "Error: broken symlink"<< src_file->getName() << std::endl;
            dynamic_cast<SymLink*>(src_file)->setBroken(true);
            return;
          }
          else if ( dynamic_cast<SymLink*>(src_file)->getTarget()->getType() != Type::REGULAR_FILE)
          {
            std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
            return;
          }
          else
          {
            src_file = dynamic_cast<SymLink*>(src_file)->getTarget();
          }
        }
      }
      else 
      {
        std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
        return;
      }
    }

    file_to_add = new RegularFile(src_file->getName(), src_file->getSerialNum(), src_file->getLastAccessTime(), src_file->getLastDataChangeTime(),
      src_file->getLastMetadataChangeTime(), src_file->getHardLinkCount(), src_file->getSize(), Type::REGULAR_FILE);
    file_to_add->setContents(dynamic_cast<RegularFile*>(src_file)->getContents());

    // Check if the destination path is a directory
    File* dest_file = fs_.getFile(dest_path);
    std::vector<std::string> components = fs_.splitPath(dest_path);
    bool path_is_valid = (components.size() <= 1) ? dest_path[0] == '/' : fs_.isValidPath(dest_path);

    if (dest_file != nullptr && dest_file->getType() == Type::DIRECTORY) {
      // The destination is a directory, append the source file name
      dest_path += '/';
      dest_path += src_file->getName();
    }else if (dest_file == nullptr && path_is_valid && dest_path.back() != '/') {
      // The destination includes the new file name and its parent is a directory
      // Nothing to be done
      file_to_add->setName(components.back());
    }else{
      std::cout << "The destination is not a directory" << std::endl;
      return;
    }

    // Check if the destination file already exists
    dest_file = fs_.getFile(dest_path);
    if (dest_file != nullptr) {
      std::cout << "cp: destination file already exists" << std::endl;
      return;
    }

    fs_.addFile(file_to_add, dest_path);
  }
}

void CLI::rm(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "Invalid number of arguments for rm." << std::endl;
    std::cout << "Usage: rm <path> [<path> ... <path>]" << std::endl;
    return;
  }

  for(size_t i = 1; i < args.size(); i++)
  {
    // Get the path of the file to delete
    std::string path = args[i];

    // Delete the file
    fs_.deleteFile(path);
  }
}

void CLI::mkdir(const std::vector<std::string>& args) {
  if (args.size() != 2) {
    std::cout << "Usage: mkdir <path>" << std::endl;
    return;
  }

  // Split the path into components
  std::string path = args[1];
  std::vector<std::string> components = fs_.splitPath(path);

  // Find the parent directory
  Directory* parent = nullptr;
  if (path[0] == '/') {
    // Absolute path
    parent = fs_.root_;
  } else {
    // Relative path
    parent = fs_.cwd_;
  }

  for (size_t i = 0; i < components.size() - 1; i++) {
    if (parent->getType() != Type::DIRECTORY) {
      std::cout << "Error: " << components[i] << "is not a directory!" << std::endl;
      return;
    }
    // Use the new "Directory::getChildren" method to get the children of the current directory
    parent = dynamic_cast<Directory*>(parent->getEntry(components[i]));
  }

  // Check if the new directory already exists
  if (parent->getType() != Type::DIRECTORY) {
    std::cout << "Error: " << components[components.size() - 2] << "is not a directory!" << std::endl;
    return;
  }

  if (parent->getEntry(components.back()) != nullptr) {
    std::cout << "Error: directory already exists" << std::endl;
    return;
  }

  // Create the new directory
  Directory* new_dir;
  new_dir = new Directory(components.back(), 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, parent);
  parent->getChildren().push_back(new_dir);
}

void CLI::rmdir(const std::vector<std::string>& args) {
  // Check if the number of arguments is correct
  if (args.size() != 2) {
    std::cout << "Usage: rmdir <directory>" << std::endl;
    return;
  }

  // Get the path of the directory to delete
  std::string path = args[1];
  if (path[0] != '/') {
    // The path is not absolute, make it relative to the current working directory
    path = fs_.getCurrentDirectory() + '/' + path;
  }

  // Get the directory to delete
  File* file = fs_.getFile(path);
  if (file->getType() != Type::DIRECTORY) {
    std::cout << "Error: " << path << " is not a directory" << std::endl;
    return;
  }
  Directory* dir = dynamic_cast<Directory*>(file);

  // Check if the directory is empty
  if (!dir->isEmpty()) {
    std::cout << "Error: " << path << " is not empty" << std::endl;
    return;
  }

  // Find the parent directory
  Directory* parent_dir = dir->getParent();
  if(parent_dir == nullptr)
  {
    std::cout << "Error: " << parent_dir->getName() << " is not a directory" << std::endl;
    return;
  }

  // Remove the directory from the parent directory
  parent_dir->removeEntry(dir->getName());

  // Delete the directory
  delete dir;
}

void CLI::ln(const std::vector<std::string>& args) {
  if (args.size() != 3) {
    std::cout << "Usage: ln <src> <dst>" << std::endl;
    return;
  }

  // Get the source and destination paths
  const std::string& src_path = args[1];
  const std::string& dst_path = args[2];
  std::vector<std::string> dst_components = fs_.splitPath(dst_path);

  // Get the source file
  File* src_file = fs_.getFile(src_path);
  if (src_file == nullptr) {
    std::cout << "ln: failed to access '" << src_path << "': No such file or directory" << std::endl;
    return;
  }

  // Check if the destination file already exists
  File* dst_file = fs_.getFile(dst_path);
  if (dst_file != nullptr) {
    std::cout << "ln: failed to create hard link '" << dst_path << "': File exists" << std::endl;
    return;
  }

  File* symlink = new SymLink(dst_components.back(), 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, src_file);

  if (dst_components.size() == 1) {
    fs_.addFile(symlink, dst_path);
  }
  else if (dst_components.size() > 1 && fs_.isValidPath(dst_path))
  {
    fs_.addFile(symlink, dst_path);
  }
  else {
    std::cout << "ln: failed to create symbolic link '" << dst_path << "': Invalid path." << std::endl;
    return;
  }
}

void CLI::stat(const std::vector<std::string>& args) {
  // Check if a file was specified
  if (args .size() != 2) {
    std::cout << "Usage: stat <file>" << std::endl;
    return;
  }

  // Get the file from the file system
  File* file = fs_.getFile(args[1]);
  if (file == nullptr) {
    std::cout << "Error: File not found" << std::endl;
    return;
  }

  // Print the file's information
  std::cout << "Name: " << file->getName() << std::endl;
  std::cout << "Serial number: " << file->getSerialNum() << std::endl;
  std::cout << "Last access: " << file->getLastAccessTime() << std::endl;
  std::cout << "Last data change: " << file->getLastDataChangeTime() << std::endl;
  std::cout << "Last metadata change: " << file->getLastMetadataChangeTime() << std::endl;
  std::cout << "Hard link count: " << file->getHardLinkCount() << std::endl;
  std::cout << "Size: " << file->getSize() << std::endl;
}

// void CLI::mount(const std::vector<std::string>& args) {
//   if (args.size() != 3) {
//     std::cout << "Usage: mount <device> <mount point>" << std::endl;
//     return;
//   }

//   // Extract the device and mount point from the arguments
//   std::string device = args[1];
//   std::string mount_point = args[2];

//   // Mount the device at the specified mount point
//   if (!fs_.mount(device, mount_point)) {
//     std::cout << "Error: Failed to mount device" << std::endl;
//   }
// }

// void CLI::umount(const std::vector<std::string>& args) {
//   if (args.size() != 1) {
//     std::cout << "Invalid number of arguments for umount" << std::endl;
//     return;
//   }

//   // Check if the file system is mounted at the given path
//   if (fs_.isMounted(args[0])) {
//     // Unmount the file system
//     fs_.unmount(args[0]);
//     std::cout << "Successfully unmounted file system at " << args[0] << std::endl;
//   } else {
//     std::cout << "No file system is mounted at " << args[0] << std::endl;
//   }
// }

std::vector<std::string> CLI::splitLine(const std::string& line) const {
  // Create a vector to store the components of the line
  std::vector<std::string> components;

  // Create a string stream from the line
  std::istringstream stream(line);

  // Read each component from the stream
  std::string component;
  while (std::getline(stream, component, ' ')) {
    // Add the component to the vector
    components.push_back(component);
  }

  return components;
}
