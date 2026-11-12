#pragma once
#include "FileSystemElement.h"
#include <iostream>
#include <string>

class File : public FileSystemElement {
private:
    std::string content;

public:
    File(const std::string& name, const std::string& content): FileSystemElement(name), content(content) {}

    size_t getSize() const override {
        return content.size();
    }

    const std::string& getContent() const {
        return content;
    }

    std::string toString() const override {
        return name + " (" + std::to_string(getSize()) + " bytes)";
    }

    void printTree(int level) const override {
        for (int i = 0; i < level; ++i) {
            std::cout << "   ";
        }
        std::cout << "|-- " << name << "\n";
    }
};