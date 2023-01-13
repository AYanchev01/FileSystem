#pragma once

enum class Type {
  REGULAR_FILE,
  DIRECTORY,
  SYMLINK,
  HARDLINK,
  NAMED_PIPE,
};