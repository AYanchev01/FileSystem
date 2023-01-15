#pragma once
#include "CLI.h"
#include "FileSystem.h"
#include <iostream>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

TEST_CASE("Test 1: Add a file to the file system tree") {
    FileSystem fs;
    fs.addFile(new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10), "/file1");
    CHECK(fs.getFile("/file1") != nullptr);
    CHECK(fs.getFile("/file1")->getName() == "file1");
    CHECK(fs.getFile("/file1")->getSerialNum() == 1);
    CHECK(fs.getFile("/file1")->getSize() == 10);
    CHECK(fs.getFile("/file1")->getType() == Type::REGULAR_FILE);
}

TEST_CASE("Test 2: Add a file to the file system tree") {
    FileSystem fs;
    fs.addFile(new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10), "/file1");
    fs.addFile(new RegularFile("file2", 2, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20), "/file2");
    CHECK(fs.getFile("/file1") != nullptr);
    CHECK(fs.getFile("/file1")->getName() == "file1");
    CHECK(fs.getFile("/file1")->getSerialNum() == 1);
    CHECK(fs.getFile("/file1")->getSize() == 10);
    CHECK(fs.getFile("/file1")->getType() == Type::REGULAR_FILE);
    CHECK(fs.getFile("/file2") != nullptr);
    CHECK(fs.getFile("/file2")->getName() == "file2");
    CHECK(fs.getFile("/file2")->getSerialNum() == 2);
    CHECK(fs.getFile("/file2")->getSize() == 20);
    CHECK(fs.getFile("/file2")->getType() == Type::REGULAR_FILE);
}

TEST_CASE("Test 3: Add a file to the file system tree") {
    FileSystem fs;
    fs.addFile(new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10), "/file1");
    fs.addFile(new RegularFile("file2", 2, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20), "/file2");
    fs.addFile(new RegularFile("file3", 3, std::time(nullptr), std::time(nullptr), std::time(nullptr), 30), "/file3");
    CHECK(fs.getFile("/file1") != nullptr);
    CHECK(fs.getFile("/file1")->getName() == "file1");
    CHECK(fs.getFile("/file1")->getSerialNum() == 1);
    CHECK(fs.getFile("/file1")->getSize() == 10);
    CHECK(fs.getFile("/file1")->getType() == Type::REGULAR_FILE);
    CHECK(fs.getFile("/file2") != nullptr);
    CHECK(fs.getFile("/file2")->getName() == "file2");
    CHECK(fs.getFile("/file2")->getSerialNum() == 2);
    CHECK(fs.getFile("/file2")->getSize() == 20);
    CHECK(fs.getFile("/file2")->getType() == Type::REGULAR_FILE);
    CHECK(fs.getFile("/file3") != nullptr);
    CHECK(fs.getFile("/file3")->getName() == "file3");
    CHECK(fs.getFile("/file3")->getSerialNum() == 3);
    CHECK(fs.getFile("/file3")->getSize() == 30);
    CHECK(fs.getFile("/file3")->getType() == Type::REGULAR_FILE);
}

TEST_CASE("Test 4: Add a file to the file system tree") {
    FileSystem fs;
    fs.addFile(new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10), "/file1");
    fs.addFile(new RegularFile("file2", 2, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20), "/file2");
    fs.addFile(new RegularFile("file3", 3, std::time(nullptr), std::time(nullptr), std::time(nullptr), 30), "/file3");
    fs.addFile(new RegularFile("file4", 4, std::time(nullptr), std::time(nullptr), std::time(nullptr), 40), "/file4");
    CHECK(fs.getFile("/file1") != nullptr);
    CHECK(fs.getFile("/file1")->getName() == "file1");
    CHECK(fs.getFile("/file1")->getSerialNum() == 1);
    CHECK(fs.getFile("/file1")->getSize() == 10);
    CHECK(fs.getFile("/file1")->getType() == Type::REGULAR_FILE);
    CHECK(fs.getFile("/file2") != nullptr);
    CHECK(fs.getFile("/file2")->getName() == "file2");
    CHECK(fs.getFile("/file2")->getSerialNum() == 2);
    CHECK(fs.getFile("/file2")->getSize() == 20);
    CHECK(fs.getFile("/file2")->getType() == Type::REGULAR_FILE);
    CHECK(fs.getFile("/file3") != nullptr);
    CHECK(fs.getFile("/file3")->getName() == "file3");
    CHECK(fs.getFile("/file3")->getSerialNum() == 3);
    CHECK(fs.getFile("/file3")->getSize() == 30);
    CHECK(fs.getFile("/file3")->getType() == Type::REGULAR_FILE);
    CHECK(fs.getFile("/file4") != nullptr);
    CHECK(fs.getFile("/file4")->getName() == "file4");
    CHECK(fs.getFile("/file4")->getSerialNum() == 4);
    CHECK(fs.getFile("/file4")->getSize() == 40);
    CHECK(fs.getFile("/file4")->getType() == Type::REGULAR_FILE);
}

TEST_CASE("Test 5: Test changeDirectory and getCurrentDirectory")
{
    // Create a file system
    FileSystem fs;
    // Add a file to the file system tree
    fs.addFile(new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10), "/file1");
    // Add a directory to the file system tree
    Directory* dir1 = new Directory("dir1", 2, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, fs.getRootDirectory());
    fs.addFile(dir1, "/dir1");
    // Add a file to the file system tree
    fs.addFile(new RegularFile("file2", 3, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20), "/dir1/file2");
    // Add a directory to the file system tree
    fs.addFile(new Directory("dir2", 4, std::time(nullptr), std::time(nullptr), std::time(nullptr),0, dir1), "/dir1/dir2");
    // Add a file to the file system tree
    fs.addFile(new RegularFile("file3", 5, std::time(nullptr), std::time(nullptr), std::time(nullptr), 30), "/dir1/dir2/file3");

    fs.changeDirectory("/");

    CHECK(fs.getCurrentDirectory() == "/");

    fs.changeDirectory("dir1");

    CHECK(fs.getCurrentDirectory() == "/dir1/");

    fs.changeDirectory("dir2");

    CHECK(fs.getCurrentDirectory() == "/dir1/dir2/");

    fs.changeDirectory("..");

    CHECK(fs.getCurrentDirectory() == "/dir1/");

    fs.changeDirectory("..");
    CHECK(fs.getCurrentDirectory() == "/");
}

TEST_CASE("Test 6: Test ls")
{
    // Create a file system
    FileSystem fs;
    // Create a CLI
    CLI cli(fs);
    // Add a file to the file system tree
    fs.addFile(new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10), "/file1");
    std::vector<std::string> args;
    args.push_back("ls");
    CHECK(cli.ls(args) == "file1 ");
    // Add a directory to the file system tree
    Directory* dir1 = new Directory("dir1", 2, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, fs.getRootDirectory());
    fs.addFile(dir1, "/dir1");
    // Add a file to the file system tree
    fs.addFile(new RegularFile("file2", 3, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20), "/dir1/file2");
    CHECK(cli.ls(args) == "file1 dir1 ");
    args.push_back("dir1");
    CHECK(cli.ls(args) == "file2 ");
}

TEST_CASE("Test 7: Test cat")
{
    // Create a file system
    FileSystem fs;
    // Create a CLI
    CLI cli(fs);
    // Add a file to the file system tree
    RegularFile* file1 = new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10);
    file1->setContents("Contents of file1");
    fs.addFile(file1, "/file1");
    std::vector<std::string> args;
    args.push_back("cat");
    args.push_back("file1");
    CHECK(cli.cat(args) == "Contents of file1");
    // Add a directory to the file system tree
    Directory* dir1 = new Directory("dir1", 2, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, fs.getRootDirectory());
    fs.addFile(dir1, "/dir1");
    // Add a file to the file system tree
    RegularFile* file2 = new RegularFile("file2", 3, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20);
    file2->setContents("Contents of file2");
    fs.addFile(file2, "/dir1/file2");
    args.push_back("dir1/file2");
    CHECK(cli.cat(args) == "Contents of file1\nContents of file2");
}

TEST_CASE("Test 7: Test cp")
{
    // Create a file system
    FileSystem fs;
    // Create a CLI
    CLI cli(fs);
    // Add a file to the file system tree
    RegularFile* file1 = new RegularFile("file1", 1, std::time(nullptr), std::time(nullptr), std::time(nullptr), 10);
    file1->setContents("Contents of file1");
    fs.addFile(file1, "/file1");
    std::vector<std::string> args;
    args.push_back("cp");
    args.push_back("file1");
    args.push_back("/file2");
    cli.cp(args);
    CHECK(fs.getFile("/file2") != nullptr);
    CHECK(fs.getFile("/file2")->getName() == "file2");
    CHECK(fs.getFile("/file2")->getSize() == 10);
    CHECK(fs.getFile("/file2")->getType() == Type::REGULAR_FILE);
    CHECK(dynamic_cast<RegularFile*>(fs.getFile("/file2"))->getContents() == "Contents of file1");
    // Add a directory to the file system tree
    Directory* dir1 = new Directory("dir1", 3, std::time(nullptr), std::time(nullptr), std::time(nullptr), 0, fs.getRootDirectory());
    fs.addFile(dir1, "/dir1");
    // Add a file to the file system tree
    RegularFile* file3 = new RegularFile("file3", 4, std::time(nullptr), std::time(nullptr), std::time(nullptr), 20);
    file3->setContents("Contents of file3");
    fs.addFile(file3, "/dir1/file3");
    args.push_back("dir1/file4");
    cli.cp(args);
    CHECK(fs.getFile("/dir1/file4") != nullptr);
    CHECK(fs.getFile("/dir1/file4")->getName() == "file4");
    CHECK(fs.getFile("/dir1/file4")->getSize() == 10);
    CHECK(fs.getFile("/dir1/file4")->getType() == Type::REGULAR_FILE);
    CHECK(dynamic_cast<RegularFile*>(fs.getFile("/dir1/file4"))->getContents() == "Contents of file1");
}

TEST_CASE("Test 7: Test mkdir and rmdir")
{
    // Create a file system
    FileSystem fs;
    // Create a CLI
    CLI cli(fs);
    std::vector<std::string> args;
    args.push_back("mkdir");
    args.push_back("dir1");
    cli.mkdir(args);
    CHECK(fs.getFile("/dir1") != nullptr);
    CHECK(fs.getFile("/dir1")->getName() == "dir1");
    CHECK(fs.getFile("/dir1")->getSize() == 0);
    CHECK(fs.getFile("/dir1")->getType() == Type::DIRECTORY);
    args.clear();
    args.push_back("mkdir");
    args.push_back("/dir1/dir2");
    cli.mkdir(args);
    CHECK(fs.getFile("/dir1/dir2") != nullptr);
    CHECK(fs.getFile("/dir1/dir2")->getName() == "dir2");
    CHECK(fs.getFile("/dir1/dir2")->getSize() == 0);
    CHECK(fs.getFile("/dir1/dir2")->getType() == Type::DIRECTORY);
    args.pop_back();
    args.push_back("/dir1/dir2");
    cli.rmdir(args);
    CHECK(fs.getFile("/dir1/dir2") == nullptr);
    args.pop_back();
    args.push_back("dir1");
    cli.rmdir(args);
    CHECK(fs.getFile("/dir1") == nullptr);
}

void runTests() {
    doctest::Context().run();
}