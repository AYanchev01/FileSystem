#include "../include/FileSystem.h"

FileSystem::FileSystem() {
  // Create the root directory and set it as the current working directory
  root_ = new Directory("/", 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, nullptr);
  cwd_ = root_;
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
  for (int i = 0; i < components.size() - 1; i++) {
    // Check if the component is in the children vector
    File* child = parent->getEntry(components[i]);
    if (child != nullptr) {
      // The component was found, move to the next node
      parent = (Directory*) child;
    } else {
      // The component was not found, create a new node
      // Use the new constructor of the Directory class, passing the parent directory as an argument
      parent->addEntry(new Directory(components[i], 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, parent));
      parent = (Directory*) parent->getChildren().back();
    }
  }
  // Add the file to the current node
  parent->addEntry(file);
}

void FileSystem::deleteEntry(File*& node) {
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
  Directory* parent = cwd_;
  for (size_t i = 0; i < components.size() - 1; i++) {
    // Use the new "Directory::getChildren" method to get the children of the current directory
    auto it = parent->getEntry(components[i]);
    if (it == nullptr) {
      // The component was not found
      std::cout << "The component was not found" << std::endl;
      return;
    }
    parent = dynamic_cast<Directory*>(it);
  }

  // Find the file to delete
  if (parent != nullptr) {
    // Find the iterator to the file in the parent's children list
    // Use the new "Directory::getChildren" method to get the children of the parent directory
    auto it = parent->getEntry(components.back());
    if (it != nullptr) {
      // Delete the file and remove it from the parent's children list
      deleteEntry(it);
      parent->removeEntry(it->getName());
    }
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

  for (int i = 0; i < components.size() - 1; i++) {
    if (parent->getType() != Type::DIRECTORY) {
      // The target is not a directory
      std::cout << "Wrong path" << std::endl;
      return nullptr;
    }
    parent = (Directory*) parent->getEntry(components[i]);
  }
  return parent->getEntry(components.back());
}

Directory*& FileSystem::getRootDirectory() {
  return root_;
}

// void FileSystem::mount(const std::string& path, FileSystem& fs) {
//   // Split the path into components
//   std::vector<std::string> components = splitPath(path);

//   // Find the node to mount the file system at
//   FileNode* node = root_;
//   for (const std::string& component : components) {
//     // Check if the component is in the children vector
//     auto it = std::find_if(node->children.begin(), node->children.end(), [&](const FileNode& child) {
//       return child.file->getName() == component;
//     });
//     if (it != node->children.end()) {
//       // The component was found, move to the next node
//       node = *it;
//     } else {
//       // The component was not found, create a new node
//       node->children.emplace_back(FileNode{new Directory(component, 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0), {}});
//       node = node->children.back();
//     }
//   }

//   // Check if the node is a directory
//   Directory* dir = dynamic_cast<Directory*>(node->file);
//   if (dir == nullptr) {
//     // The node is not a directory
//     return;
//   }

//   // Clear the children of the directory and add the children of the root of the file system being mounted
//   node->children.clear();
//   for (const auto& child : fs.getRootDirectory()->children) {
//     node->children.emplace_back(FileNode{child->file, {}});
//   }
// }

// void FileSystem::umount(const std::string& path) {
//   // Split the path into components
//   std::vector<std::string> components = splitPath(path);

//   // Find the parent directory of the file system to unmount
//   FileNode* parent = cwd_;
//   for (size_t i = 0; i < components.size() - 1; i++) {
//     Directory* dir = dynamic_cast<Directory*>(parent->file);
//     if (dir == nullptr) {
//       // The parent is not a directory
//       return;
//     }
//     // Use the new "Directory::getChildren" method to get the children of the current directory
//     parent = &(*dir->getChildren()[components[i]]);
//   }

//   // Find the file system to unmount
//   Directory* dir = dynamic_cast<Directory*>(parent->file);
//   if (dir != nullptr) {
//     // Find the iterator to the file system in the parent's children list
//     // Use the new "Directory::getChildren" method to get the children of the parent directory
//     auto it = std::find_if(dir->getChildren().begin(), dir->getChildren().end(), [&](const FileNode& node) {
//       return node.file->getName() == components.back();
//     });
//     if (it != dir->getChildren().end()) {
//       // Delete the file and remove it from the parent's children list
//       delete it->second;
//       dir->getChildren().erase(it);
//     }
//   }
// }

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