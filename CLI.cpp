#include "CLI.h"
#include "FileSystem.h"

CLI::CLI(FileSystem& fs) : fs_(fs) {}

void CLI::run() {
        while (true) {
        // Print the prompt
        std::cout << fs_.getCurrentDirectory() << " $ ";

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
        } else if (components[0] == "mv") {
        mv(components);
        } else if (components[0] == "rm") {
        rm(components);
        } else if (components[0] == "mkdir") {
        mkdir(components);
        } else if (components[0] == "rmdir") {
        rmdir(components);
        } else if (components[0] == "ln") {
        //ln(components);
        } else if (components[0] == "stat") {
        stat(components);
        } else if (components[0] == "mount") {
        //mount(components);
        } else if (components[0] == "umount") {
        //umount(components);
        } else if (components[0] == "exit") {
        break;
        } else {
        std::cout << "Invalid command" << std::endl;
        }
    }
}

void CLI::cd(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: cd <directory>" << std::endl;
        return;
    }
    FileNode* fileNode = fs_.getFileNode(args[0]);
    if (fileNode == nullptr) {
        std::cout << "Directory not found: " << args[0] << std::endl;
        return;
    }
    Directory* dir = dynamic_cast<Directory*>(fileNode->file);
    if (dir == nullptr) {
        std::cout << args[0] << " is not a directory" << std::endl;
        return;
    }
    fs_.cwd_ = fileNode;
}


void CLI::pwd() {
  std::cout << fs_.cwd_->file->getName() << std::endl;
}

void CLI::ls(const std::vector<std::string>& args) {
  // Get the path to the directory to list
  std::string path;
  if (args.empty()) {
    // No path was specified, use the current working directory
    path = fs_.getCurrentDirectory();
  } else {
    // Use the specified path
    path = args[0];
  }

  // Get the directory to list
  File* file = fs_.getFile(path);
  Directory* dir = dynamic_cast<Directory*>(file);
  if (dir == nullptr) {
    // The specified path is not a directory
    std::cout << path << " is not a directory" << std::endl;
    return;
  }

  // Print the names of the files in the directory
  for (const auto& [name, child] : dir->getChildren()) {
    std::cout << name << " ";
  }
  std::cout << std::endl;
}

void CLI::cat(const std::vector<std::string>& args) {
    if (args.size() != 1) {
    std::cout << "Error: cat requires 1 argument" << std::endl;
    return;
    }

    // Get the file from the file system
    File* file = fs_.getFile(args[0]);
    if (file == nullptr) {
    std::cout << "Error: file not found" << std::endl;
    return;
    }

    // Print the contents of the file
    std::cout << file->getContents() << std::endl;
}

void CLI::cp(const std::vector<std::string>& args) {
  // Check if the number of arguments is correct
  if (args.size() != 3) {
    std::cout << "cp: invalid number of arguments" << std::endl;
    return;
  }

  // Get the source file and destination path
  std::string src_path = args[1];
  std::string dest_path = args[2];

  // Get the source file
  File* src_file = fs_.getFile(src_path);
  if (src_file == nullptr) {
    std::cout << "cp: source file does not exist" << std::endl;
    return;
  }

  // Check if the destination path is a directory
  File* dest_file = fs_.getFile(dest_path);
  if (dest_file != nullptr && dest_file->getType() == Type::DIRECTORY) {
    // The destination is a directory, append the source file name
    dest_path += '/';
    dest_path += src_file->getName();
  }

  // Check if the destination file already exists
  dest_file = fs_.getFile(dest_path);
  if (dest_file != nullptr) {
    std::cout << "cp: destination file already exists" << std::endl;
    return;
  }

  // Add the source file to the file system tree with the new name
  fs_.addFile(src_file, dest_path);
}

void CLI::mv(const std::vector<std::string>& args) {
  // Check if there are enough arguments
  if (args.size() != 2) {
    std::cout << "Usage: mv <source> <destination>" << std::endl;
    return;
  }

  // Get the source file
  FileNode* source = fs_.getFileNode(args[0]);
  if (source == nullptr) {
    std::cout << "File not found: " << args[0] << std::endl;
    return;
  }

  // Split the destination path into components
  std::vector<std::string> components = fs_.splitPath(args[1]);

  // Find the parent directory of the destination
  FileNode* parent = fs_.cwd_;
  for (size_t i = 0; i < components.size() - 1; i++) {
    Directory* dir = dynamic_cast<Directory*>(parent->file);
    if (dir == nullptr) {
      // The parent is not a directory
      std::cout << "Not a directory: " << args[1] << std::endl;
      return;
    }
    // Use the new "Directory::getChildren" method to get the children of the current directory
    parent = &(*dir->getChildren()[components[i]]);
  }

  // Check if the destination is a directory
  Directory* dest_dir = dynamic_cast<Directory*>(parent->file);
  if (dest_dir == nullptr) {
    std::cout << "Not a directory: " << args[1] << std::endl;
    return;
  }

  // Add the file to the destination directory
  dest_dir->addEntry(source);
}

void CLI::rm(const std::vector<std::string>& args) {
  if (args.size() != 1) {
    std::cout << "Invalid number of arguments for rm." << std::endl;
    return;
  }

  // Get the path of the file to delete
  std::string path = args[0];

  // Delete the file
  fs_.deleteFile(path);
}

void CLI::mkdir(const std::vector<std::string>& args) {
  if (args.size() != 2) {
    std::cout << "Usage: mkdir <path>" << std::endl;
    return;
  }

  // Split the path into components
  std::vector<std::string> components = fs_.splitPath(args[1]);

  // Find the parent directory
  FileNode* parent = fs_.cwd_;
  for (size_t i = 0; i < components.size() - 1; i++) {
    Directory* dir = dynamic_cast<Directory*>(parent->file);
    if (dir == nullptr) {
      std::cout << "Error: parent directory is not a directory" << std::endl;
      return;
    }
    // Use the new "Directory::getChildren" method to get the children of the current directory
    parent->file = &(*dir->getChildren()[components[i]]->file);
  }

  // Check if the new directory already exists
  Directory* dir = dynamic_cast<Directory*>(parent->file);
  if (dir == nullptr) {
    std::cout << "Error: parent directory is not a directory" << std::endl;
    return;
  }
  // Use the new "Directory::getChildren" method to get the children of the parent directory
  if (dir->getChildren().find(components.back()) != dir->getChildren().end()) {
    std::cout << "Error: directory already exists" << std::endl;
    return;
  }

  // Create the new directory
  FileNode new_dir;
  new_dir.file = new Directory(components.back(), 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, dir);
  dir->getChildren()[components.back()] = &new_dir;
}

void CLI::rmdir(const std::vector<std::string>& args) {
  // Check if the number of arguments is correct
  if (args.size() != 1) {
    std::cout << "Usage: rmdir <directory>" << std::endl;
    return;
  }

  // Get the path of the directory to delete
  std::string path = args[0];
  if (path[0] != '/') {
    // The path is not absolute, make it relative to the current working directory
    path = fs_.getCurrentDirectory() + '/' + path;
  }

  // Get the directory to delete
  File* file = fs_.getFile(path);
  Directory* dir = dynamic_cast<Directory*>(file);
  if (dir == nullptr) {
    std::cout << "Error: " << path << " is not a directory" << std::endl;
    return;
  }

  // Check if the directory is empty
  if (!dir->isEmpty()) {
    std::cout << "Error: " << path << " is not empty" << std::endl;
    return;
  }

  // Find the parent directory
  std::string parent_path = path.substr(0, path.find_last_of('/'));
  File* parent_file = fs_.getFile(parent_path);
  Directory* parent_dir = dynamic_cast<Directory*>(parent_file);
  if (parent_dir == nullptr) {
    std::cout << "Error: " << parent_path << " is not a directory" << std::endl;
    return;
  }

  // Remove the directory from the parent directory
  parent_dir->removeEntry(dir->getName());

  // Delete the directory
  delete dir;
}

// void CLI::ln(const std::vector<std::string>& args) {
//   if (args.size() != 3) {
//     std::cout << "Usage: ln <src> <dst>" << std::endl;
//     return;
//   }

//   // Get the source and destination paths
//   const std::string& src_path = args[1];
//   const std::string& dst_path = args[2];

//   // Get the source file
//   File* src_file = fs_.getFile(src_path);
//   if (src_file == nullptr) {
//     std::cout << "ln: failed to access '" << src_path << "': No such file or directory" << std::endl;
//     return;
//   }

//   // Check if the destination is a directory
//   Directory* dst_dir = dynamic_cast<Directory*>(fs_.getFile(dst_path));
//   if (dst_dir != nullptr) {
//     // The destination is a directory, create the symbolic link in the directory
//     // Use the source file's name as the symbolic link name
//     dst_dir->addEntry(new FileNode{new SymLink(src_file->getName(), 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0,src_file),{}});
//   } else {
//     // The destination is not a directory, create the symbolic link at the specified path
//     // Get the parent directory of the destination
//     std::vector<std::string> dst_components = splitPath(dst_path);
//     FileNode* dst_parent = fs_.cwd_;
//     for (size_t i = 0; i < dst_components.size() - 1; i++) {
//       dst_parent = &dst_parent->children[dst_components[i]];
//     }
//     // Add the symbolic link to the parent directory
//     dst_parent->children.emplace_back(FileNode{new SymLink(dst_components.back(), 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, src_file), {}});
//   }
// }

void CLI::stat(const std::vector<std::string>& args) {
  // Check if a file was specified
  if (args.empty()) {
    std::cout << "Error: No file specified" << std::endl;
    return;
  }

  // Get the file from the file system
  File* file = fs_.getFile(args[0]);
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
