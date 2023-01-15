#include "../include/CLI.h"
#include "../include/FileSystem.h"


CLI::CLI(FileSystem& fs) : fs_(fs) {}


/**
 * @brief runs the command interpreter loop
 * 
 */
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

        // Interpret the command
        if (components[0] == "cd") {
        cd(components);
        } else if (components[0] == "pwd") {
        pwd();
        } else if (components[0] == "ls") {
        std::cout << ls(components) << std::endl;
        } else if (components[0] == "cat") {
        std::cout << cat(components) << std::endl;
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
        } else if (components[0] == "exit") {
        break;
        } else {
        std::cout << "Invalid command" << std::endl;
        }
    }
}


/**
 * @brief change the current working directory
 * 
 * @param args - the command line arguments
 */
void CLI::cd(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: cd <directory>" << std::endl;
        return;
    }
    fs_.changeDirectory(args[1]);
}

/**
 * @brief print the current working directory
 * 
 */
void CLI::pwd() {
  std::cout << fs_.getCurrentDirectory() << std::endl;
}


/**
 * @brief list the contents of a directory
 * 
 * @param args - the command line arguments
 */
std::string CLI::ls(const std::vector<std::string>& args) {

  std::string result = "";
  // Get the path to the directory to list
  std::string path;
  if (args.empty())
  {
    return result;
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
    std::cout << "Error: no such file or directory:"<< path << std::endl;
    return result;
  }

  if (file->getType() != Type::DIRECTORY) {
    std::cout << "Error: not a directory:" << path << std::endl;
    return result;
  }
  Directory* dir = dynamic_cast<Directory*>(file);
  dir->setLastAccessTime(std::time(nullptr));

  // Print the names of the files in the directory
  for (auto child : dir->getChildren()) {
    result += child->getName() + " ";
  }
  return result;
}


/**
 * @brief print the contents of a file, or read from stdin and write to a file, or concatenate files
 * 
 * @param args - the command line arguments
 */
std::string CLI::cat(const std::vector<std::string>& args) {
    std::string result = "";
    if (args.empty())
    {
      return result;
    }
    if (args.size() == 1) {
      // No files passed, read from stdin and print to stdout

        //std::string result = "";
        std::string line;
        while (true) {
          std::getline(std::cin, line);
          if (line == ".") break;
          result += line;
        }
        //std::cout << result << std::endl;
        return result;
    }

    for (size_t i = 1; i < args.size(); i++)
    {

      // Check if we are writing to a file
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
          return "";
        }
        File* output_file = fs_.getFile(args[i + 1]);
        if (output_file == nullptr) {
          fs_.addFile(new RegularFile((fs_.splitPath(args[i + 1])).back(), rand() % 100001, std::time(nullptr), std::time(nullptr), std::time(nullptr), result.size(), Type::REGULAR_FILE), args[i + 1]);
          output_file = fs_.getFile(args[i + 1]);
        }
        if (output_file->getType() != Type::REGULAR_FILE) {
          std::cout << "Error: output file is not a file" << std::endl;
          return "";
        }
        RegularFile* res = dynamic_cast<RegularFile*>(output_file);
        res->setContents(result);
        return "";
      }

      // Get the file to read and append to the result
      File* file = fs_.getFile(args[i]);
      if (file == nullptr) {
        std::cout << "Error: file not found" << std::endl;
        return "";
      }
      if (file->getType() != Type::REGULAR_FILE) 
      {
        if (file->getType() == Type::SYMLINK)
        {
          if (dynamic_cast<SymLink*>(file)->isBroken())
          {
            std::cout << "Error: broken symlink" << file->getName() << std::endl;
            return "";
          }
          else 
          {
            if (dynamic_cast<SymLink*>(file)->getTarget() == nullptr)
            {
              std::cout << "Error: broken symlink"<< file->getName() << std::endl;
              dynamic_cast<SymLink*>(file)->setBroken(true);
              return "";
            }
            else if ( dynamic_cast<SymLink*>(file)->getTarget()->getType() != Type::REGULAR_FILE)
            {
              std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
              return "";
            }
          }
        }
        else 
        {
          // The file is not a regular file or a symlink
          std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
          return "";
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
      reg_file->setLastAccessTime(std::time(nullptr));

      if(!result.empty()) { result += "\n"; }
      result += reg_file->getContents();

      // Check if we are at the end of the arguments
      // If so, print the result to stdout
      if(i == args.size() - 1)
      {
        return result;
      }
    }
    return result;
}


/**
 * @brief copy a file to a destination
 * 
 * @param args - the command line arguments
 */
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
          dynamic_cast<SymLink*>(src_file)->setLastAccessTime(std::time(nullptr));
          std::cout << "Error: broken symlink" << src_file->getName() << std::endl;
          return;
        }
        else 
        {
          if (dynamic_cast<SymLink*>(src_file)->getTarget() == nullptr)
          {
            std::cout << "Error: broken symlink"<< src_file->getName() << std::endl;
            dynamic_cast<SymLink*>(src_file)->setBroken(true);
            dynamic_cast<SymLink*>(src_file)->setLastAccessTime(std::time(nullptr));
            dynamic_cast<SymLink*>(src_file)->setLastMetadataChangeTime(std::time(nullptr));
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
            dynamic_cast<SymLink*>(src_file)->setLastAccessTime(std::time(nullptr));
          }
        }
      }
      else 
      {
        std::cout << "Error: source should be a file or a symbolic link to a regular file" << std::endl;
        return;
      }
    }

    src_file->setLastAccessTime(std::time(nullptr));

    // Create a new file with the same properties as the source file
    file_to_add = new RegularFile(src_file->getName(), rand() % 100001, std::time(nullptr), std::time(nullptr),
      std::time(nullptr), src_file->getSize(), Type::REGULAR_FILE);
    file_to_add->setContents(dynamic_cast<RegularFile*>(src_file)->getContents());

    // Check if the destination path is a directory
    File* dest_file = fs_.getFile(dest_path);
    std::vector<std::string> components = fs_.splitPath(dest_path);
    bool path_is_valid = (components.size() <= 1) ? dest_path[0] == '/' : fs_.isValidPath(dest_path);

    if (dest_file != nullptr && dest_file->getType() == Type::DIRECTORY) {
      // The destination is a directory, append the source file name
      dest_path += '/';
      dest_path += src_file->getName();
    }
    else if (dest_file != nullptr && dest_file->getType() != Type::DIRECTORY) 
    {
      // The destination is not a directory and it already exists
      std::cout << "cp: destination file already exists" << std::endl;
      return;
    }
    else if (dest_file == nullptr && path_is_valid && dest_path.back() != '/') 
    {
      // The destination includes the new file name and its parent is a directory
      // Nothing to be done
      file_to_add->setName(components.back());
    }
    else
    {
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


/**
 * @brief remove a file from the file system
 * 
 * @param args - the command line arguments
 */
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


/**
 * @brief Create a new directory
 * 
 * @param args - the command line arguments
 */
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
    // Get the next directory in the path
    parent = dynamic_cast<Directory*>(parent->getEntry(components[i]));
  }

  // Check if the parent is a directory
  if (parent->getType() != Type::DIRECTORY) {
    std::cout << "Error: " << components[components.size() - 2] << "is not a directory!" << std::endl;
    return;
  }

  // Check if the new directory already exists
  if (parent->getEntry(components.back()) != nullptr) {
    std::cout << "Error: directory already exists" << std::endl;
    return;
  }

  // Create the new directory and add it to the parents children
  Directory* child;
  child = new Directory(components.back(), rand() % 100001, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, parent);
  parent->getChildren().push_back(child);
}


/**
 * @brief remove a directory from the file system if it is empty
 * 
 * @param args - the command line arguments
 */
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


/**
 * @brief create a symbolic link to a file
 * 
 * @param args - the command line arguments
 */
void CLI::ln(const std::vector<std::string>& args) {
  if (args.size() != 4) {
    std::cout << "Usage: ln -s <src> <dst>" << std::endl;
    return;
  }else if(args[1] != "-s"){
    std::cout << "Usage: ln -s <src> <dst>" << std::endl;
    return;
  }

  // Get the source and destination paths
  const std::string& src_path = args[2];
  const std::string& dst_path = args[3];
  std::vector<std::string> dst_components = fs_.splitPath(dst_path);

  // Get the source file
  File* src_file = fs_.getFile(src_path);
  if (src_file == nullptr) {
    std::cout << "ln: failed to access '" << src_path << "': No such file or directory" << std::endl;
    return;
  }
  src_file->setLastAccessTime(std::time(nullptr));

  // Check if the destination file already exists
  File* dst_file = fs_.getFile(dst_path);
  if (dst_file != nullptr) {
    std::cout << "ln: failed to create hard link '" << dst_path << "': File exists" << std::endl;
    return;
  }

  // Create the symbolic link
  File* symlink = new SymLink(dst_components.back(), rand() % 100001, std::time(nullptr), std::time(nullptr), std::time(nullptr), sizeof(File*), src_file);

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


/**
 * @brief print the meta data of a file
 * 
 * @param args - the command line arguments
 */
void CLI::stat(const std::vector<std::string>& args) {
  // Check if a file was specified
  if (args .size() < 2) {
    std::cout << "Usage: stat <file> [<file> ... <file>]" << std::endl;
    return;
  }

  for (size_t i = 1; i < args.size(); i++)
  {
    // Get the file from the file system
    File* file = fs_.getFile(args[i]);
    if (file == nullptr) {
      std::cout << "Error: File not found" << std::endl;
      return;
    }

    file->setLastAccessTime(std::time(nullptr));
    
    // Print the file's information
    std::cout << "Name: " << file->getName() << std::endl;
    std::cout << "Serial number: " << file->getSerialNum() << std::endl;
    std::cout << "Last access: " << file->getLastAccessTime() << std::endl;
    std::cout << "Last data change: " << file->getLastDataChangeTime() << std::endl;
    std::cout << "Last metadata change: " << file->getLastMetadataChangeTime() << std::endl;
    std::cout << "Size: " << file->getSize() << std::endl;
  }
}


/**
 * @brief Split a line into its components, separated by spaces
 * 
 * @param line - the line to split
 * @return std::vector<std::string> - the components of the line
 */
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