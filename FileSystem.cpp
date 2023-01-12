#include "FileSystem.h"

FileSystem::FileSystem() {
  // Create the root directory and set it as the current working directory
  root_ = new FileNode{nullptr, {}};
  root_->file = new Directory("/", 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, nullptr);
  cwd_ = root_;
}

FileSystem::~FileSystem() {
  // Recursively delete all the nodes in the file system tree
  deleteNode(root_);
}

void FileSystem::addFile(File* file, const std::string& path) {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);
  // Find the node to add the file to
  FileNode* node = root_;
  for (const std::string& component : components) {
    // Check if the component is in the children vector
    auto it = std::find_if(node->children.begin(), node->children.end(), [&](auto* child) {
      return child->file->getName() == component;
    });
    if (it != node->children.end()) {
      // The component was found, move to the next node
      node = *it;
    } else {
      // The component was not found, create a new node
      // Use the new constructor of the Directory class, passing the parent directory as an argument
      Directory* parent = dynamic_cast<Directory*>(node->file);
      node->children.emplace_back(new FileNode{new Directory(component, 0, std::time(nullptr), std::time(nullptr), std::time(nullptr), 1, 0, parent), {}});
      node = node->children.back();
    }
  }
  // Add the file to the current node
  node->file = file;
}

void FileSystem::deleteNode(FileNode*& node) {
// Recursively delete all the children of this node
for (auto& child : node->children) {
deleteNode(child);
}
// Delete the file for this node
delete node->file;
}

void FileSystem::deleteFile(const std::string& path) {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);

  // Find the parent directory of the file
  FileNode* parent = cwd_;
  for (size_t i = 0; i < components.size() - 1; i++) {
    Directory* dir = dynamic_cast<Directory*>(parent->file);
    if (dir == nullptr) {
      // The parent is not a directory
      return;
    }
    // Use the new "Directory::getChildren" method to get the children of the current directory
    auto it = dir->getChildren().find(components[i]);
    if (it == dir->getChildren().end()) {
      // The component was not found
      return;
    }
    parent = it->second;
  }

  // Find the file to delete
  Directory* dir = dynamic_cast<Directory*>(parent->file);
  if (dir != nullptr) {
    // Find the iterator to the file in the parent's children list
    // Use the new "Directory::getChildren" method to get the children of the parent directory
    auto it = dir->getChildren().find(components.back());
    if (it != dir->getChildren().end()) {
      // Delete the file and remove it from the parent's children list
      deleteNode(it->second);
      dir->getChildren().erase(it);
    }
  }
}


void FileSystem::changeDirectory(const std::string& path) {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);

  // Find the target directory
  FileNode* target = cwd_;
  for (const std::string& component : components) {
    Directory* dir = dynamic_cast<Directory*>(target->file);
    if (dir == nullptr) {
      // The target is not a directory
      return;
    }
    target->file = &(*dir->getChildren()[component]->file);
  }

  // Set the target as the current working directory
  cwd_ = target;
}

std::string FileSystem::getCurrentDirectory() const {
  // Recursively build the path to the current working directory
  std::string path;
  FileNode* node = cwd_;
  while (node != root_) {
    Directory* dir = dynamic_cast<Directory*>(node->file);
    if (dir == nullptr) {
      // The node is not a directory
      return "";
    }
    path = '/' + node->file->getName() + path;
    node->file = dir->getParent();
  }
  return path;
}

File* FileSystem::getFile(const std::string& path) const {
  // Split the path into components
  std::vector<std::string> components = splitPath(path);

  // Find the target file
  FileNode* target = cwd_;
  for (const std::string& component : components) {
    Directory* dir = dynamic_cast<Directory*>(target->file);
    if (dir == nullptr) {
      // The target is not a directory
      return nullptr;
    }
    target->file = &(*dir->getChildren()[component]->file);
  }
  return target->file;
}

FileNode*& FileSystem::getRootDirectory() {
  return root_;
}

FileNode* FileSystem::getFileNode(const std::string& path) {
    // Split the path into components
    std::vector<std::string> components = splitPath(path);

    // Start at the root of the file system
    FileNode* node = root_;

    // Iterate through the path components
    for (const std::string& component : components) {
        // Check if the component is in the children map
        auto it = std::find_if(node->children.begin(), node->children.end(), [&](auto* child) {
            return child->file->getName() == component;
        });
        if (it != node->children.end()) {
            // The component was found, move to the next node
            node = *it;
        } else {
            // The component was not found, return nullptr
            return nullptr;
        }
    }
    return node;
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