#pragma once
#include "Directory.h"
#include "File.h"
#include "Path.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <iostream>

class FileSystem {
private:
    std::unique_ptr<Directory> root;
    Directory* findDirectoryOnPath(const Path& path) const {
        FileSystemElement* element = find(path);
        return dynamic_cast<Directory*>(element);
    }
    Path getParentPath(const Path& path) const {
        const auto& parts = path.getComponents();
        if (parts.empty())
            throw std::invalid_argument("Root has no parent");
        std::vector<std::string> parentParts(parts.begin(), parts.end()-1);
        return Path(parentParts);
    }
public:
    FileSystem(const std::string& rootName) : root(std::make_unique<Directory>(rootName)) {}

    FileSystemElement* find(const Path& path) const {
        FileSystemElement* current = root.get();
        for (const auto& part : path.getComponents()) {
            Directory* dir = dynamic_cast<Directory*>(current);
            if (dir == nullptr)
                return nullptr;
            current = dir->findElement(part);
            if (current == nullptr)
                return nullptr;
        }
        return current;
    }

    void createFile(const Path& path, const std::string& content) {
        const auto& parts = path.getComponents();
        if (parts.empty())
            throw std::invalid_argument("Invalid file path");
        Directory* parent = findDirectoryOnPath(getParentPath(path));
        if (parent == nullptr)
            throw std::invalid_argument("Parent directory not found");
        parent->addElement(std::make_unique<File>(parts.back(), content));
    }

    void createDirectory(const Path& path) {
        const auto& parts = path.getComponents();
        if (parts.empty())
            throw std::invalid_argument("Invalid directory path");
        Directory* parent = findDirectoryOnPath(getParentPath(path));
        if (parent == nullptr)
            throw std::invalid_argument("Parent directory not found");
        parent->addElement(std::make_unique<Directory>(parts.back()));
    }

    void deleteElement(const Path& path) {
        const auto& parts = path.getComponents();
        if (parts.empty())
            throw std::invalid_argument("Cannot delete root");
        Directory* parent = findDirectoryOnPath(getParentPath(path));
        if (parent == nullptr)
            throw std::invalid_argument("Parent directory not found");
        parent->deleteElement(parts.back());
    }

    void moveElement(const Path& path1, const Path& path2) {
        const auto& src = path1.getComponents();
        const auto& dst = path2.getComponents();

        if (src.empty())
            throw std::invalid_argument("Cannot move root");
        FileSystemElement* element = find(path1);
        if (element == nullptr)
            throw std::invalid_argument("Element not found");
        Directory* targetDir = findDirectoryOnPath(path2);
        if (targetDir == nullptr)
            throw std::invalid_argument("Destination directory not found");

        if (dynamic_cast<Directory*>(element) != nullptr) {
            if (dst.size() >= src.size()) {
                bool isDescendant = true;
                for (size_t i = 0; i < src.size(); ++i) {
                    if (src[i] != dst[i]) {
                        isDescendant = false;
                        break;
                    }
                }
                if (isDescendant)
                    throw std::invalid_argument("Cannot move directory into itself");
            }
        }

        Directory* sourceparent = findDirectoryOnPath(getParentPath(path1));
        if (sourceparent == nullptr)
            throw std::invalid_argument("Source parent not found");
        if (targetDir->findElement(element->getName()) != nullptr)
            throw std::invalid_argument("Element with this name already exists in destination");

        auto moving = sourceparent->takeElement(element->getName());
        targetDir->addElement(std::move(moving));
    }

    size_t getSize(const Path& path) const {
        FileSystemElement* element = find(path);
        if (element == nullptr)
            throw std::invalid_argument("Element not found");
        return element->getSize();
    }

    void printTree() const {
        root->printTree(0);
    }
};