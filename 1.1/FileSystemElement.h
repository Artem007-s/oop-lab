#pragma once
#include <string>
#include <stdexcept>

class FileSystemElement {
protected:
    std::string name;
public:
    FileSystemElement(const std::string& name) {
        if (name.empty())
            throw std::invalid_argument("Name is empty");
        this->name = name;
    }

    virtual ~FileSystemElement() = default;

    const std::string& getName() const {
        return name;
    }

    virtual size_t getSize() const = 0;

    virtual std::string toString() const = 0;

    virtual void printTree(int level = 0) const = 0;
};