#include "../include/FileSystem.h"

FileSystem::FileSystem() {
  // Create the root directory and set it as the current working directory
  root_ = new Directory("/", 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, nullptr);
  cwd_ = root_;

  std::srand(std::time(nullptr));
}

FileSystem::~FileSystem() {
  // Recursively delete all the nodes in the file system tree
  File* root = root_;
  deleteEntry(root);
}

void FileSystem::addFile(File* file, const std::string& path) {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);
  // Find the node to add the file to
  Directory* parent = nullptr;
  // Find the target directory
  if(path[0] == '/') {
    parent = root_;
  }else{
    parent = cwd_;
  }
  for (size_t i = 0; i < components.size() - 1; i++) {
    // Check if the component is in the children vector
    File* child = parent->getEntry(components[i]);
    if (child != nullptr) {
      // The component was found, move to the next node
      parent = (Directory*) child;
    } else {
      // The component was not found, create a new node
      // Use the new constructor of the Directory class, passing the parent directory as an argument
      std::cout << "Creating directory: " << components[i] << std::endl;
      parent->addEntry(new Directory(components[i], rand() % 100001, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, parent));
      parent = (Directory*) parent->getChildren().back();
    }
  }
  // Add the file to the current node
  parent->addEntry(file);
}

void FileSystem::deleteEntry(File* node) {
  // Recursively delete all the children of this node
  if (node == nullptr) {
    return;
  }
  if (node->getType() == Type::DIRECTORY) {
      Directory* dir = dynamic_cast<Directory*>(node);
      for (auto& child : dir->getChildren()) {
        deleteEntry(child);
      }
    }
  delete node;
  node = nullptr;
}

void FileSystem::deleteFile(const std::string& path) {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);

  // Find the parent directory of the file
  Directory* parent = nullptr;
  // Find the target directory
  if(path[0] == '/') {
    parent = root_;
  }else{
    parent = cwd_;
  }

  if (components.size() > 1) {
    for (size_t i = 0; i < components.size() - 1; i++) {
      // Use the new "Directory::getChildren" method to get the children of the current directory
      File* child = parent->getEntry(components[i]);
      if (child == nullptr || child->getType() != Type::DIRECTORY) {
        // The component was not found
        std::cout << "Directory not found: " << components[i] <<std::endl;
        return;
      }
      parent = dynamic_cast<Directory*>(child);
    }
  }
  // Find the file to delete
  if (parent != nullptr) {
    // Find the iterator to the file in the parent's children list
    // Use the new "Directory::getChildren" method to get the children of the parent directory
    File* to_be_deleted = parent->getEntry(components.back());
    if (to_be_deleted == nullptr) {
      // The file was not found
      std::cout << "File not found: " << components.back() << std::endl;
      return;
    }
    if (to_be_deleted->getType() == Type::DIRECTORY)
    {
      std::cout << "Cannot remove directories with rm. Use rmdir." << std::endl;
      return;
    }
    // Delete the file and remove it from the parent's children list
    parent->removeEntry(to_be_deleted->getName());
    deleteEntry(to_be_deleted);
  }
}


void FileSystem::changeDirectory(const std::string& path) {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);
  Directory* target = nullptr;
  // Find the target directory
  if(path[0] == '/') {
    target = root_;
  }else{
    target = cwd_;
  }
  for (const std::string& component : components) {
    if (target->getType() != Type::DIRECTORY) {
      // The target is not a directory
      std::cout << "Directory not found: " << component << std::endl;
      return;
    }
    if(component == ".") {
      continue;
    }
    if(component == "..") {
      target = target->getParent();
      if (target == nullptr) {
        std::cout << "Directory not found: " << path << std::endl;
        return;
      }
      continue;
    }
    target = (Directory*) target->getEntry(component);
    if(target == nullptr) {
      std::cout << "Directory not found: " << component << std::endl;
      return;
    }
  }

  cwd_ = target;
  cwd_->setLastAccessTime(std::time(nullptr));
  cwd_->setLastMetadataChangeTime(std::time(nullptr));
}

std::string FileSystem::getCurrentDirectory() const {
  // Recursively build the path to the current working directory
  std::string path = "/";
  Directory* node = cwd_;
  while (node != root_) {
    if (node->getType() != Type::DIRECTORY) {
      // The node is not a directory
      return "";
    }
    path = '/' + node->getName() + path;
    node = node->getParent();
  }
  node->setLastAccessTime(std::time(nullptr));
  return path;
}

File* FileSystem::getFile(const std::string& path) const {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);
  
  if(components.empty()) {
    return this->root_;
  }

  // Find the target file
  Directory* parent = nullptr;
  if (path[0] == '/') {
    parent = root_;
  } else {
    parent = cwd_;
  }

  if(components.size() == 1)
  {
    return parent->getEntry(components[0]);
  }

  for (size_t i = 0; i < components.size() - 1; i++) {
    
    if (parent->getType() != Type::DIRECTORY) {
      // The target is not a directory
      std::cout << "Wrong path" << std::endl;
      return nullptr;
    }
    if(components[i] == ".") {
      continue;
    }
    if(components[i] == "..") {
      parent = parent->getParent();
      if (parent == nullptr) {
        std::cout << "Directory not found" << std::endl;
        return nullptr;
      }
      continue;
    }

    parent = (Directory*) parent->getEntry(components[i]);
    if (parent == nullptr) {
      // The component was not found
      std::cout << "Directory not found" << std::endl;
      return nullptr;
    }
  }
  return parent->getEntry(components.back());
}

Directory*& FileSystem::getRootDirectory() {
  return root_;
}

std::vector<std::string> FileSystem::splitPath(const std::string& path) const{
  // Split the path on '/' and remove empty components
  std::vector<std::string> components;
  std::string component;
  for (char c : path) {
    if (c == '/') {
      if (!component.empty()) {
        components.push_back(component);
        component.clear();
      }
    } else {
      component += c;
    }
  }
  if (!component.empty()) {
    components.push_back(component);
  }
  return components;
}

bool FileSystem::isValidPath(const std::string& path) const {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);

  // Find the target file
  Directory* parent = nullptr;
  if (path[0] == '/') {
    parent = root_;
  } else {
    parent = cwd_;
  }

  for (size_t i = 0; i < components.size() - 2; i++) {
    if (parent->getType() != Type::DIRECTORY) {
      // The target is not a directory
      return false;
    }
    parent = (Directory*) parent->getEntry(components[i]);
    if (parent == nullptr) {
      // The component was not found
      return false;
    }
  }
  return true;
}