#pragma once
#include "FileSystemElement.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

class Directory : public FileSystemElement {
private:
    std::vector<std::unique_ptr<FileSystemElement>> elements;
public:
    Directory(const std::string& name) : FileSystemElement(name) {}

    void addElement(std::unique_ptr<FileSystemElement> element) {
        if (!element)
            throw std::invalid_argument("Element is null");
        if (element.get() == this)
            throw std::invalid_argument("Cannot add directory to itself");
        if (findElement(element->getName()) != nullptr)
            throw std::invalid_argument("Name already exists");
        elements.push_back(std::move(element));
    }

    FileSystemElement* findElement(const std::string& targetName) const {
        for (const auto& elem : elements) {
            if (elem->getName() == targetName)
                return elem.get();
        }
        return nullptr;
    }

    void deleteElement(const std::string& targetName) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->getName() == targetName) {
                elements.erase(it);
                return;
            }
        }
        throw std::invalid_argument("Element not found");
    }

    std::unique_ptr<FileSystemElement> takeElement(const std::string& targetName) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if ((*it)->getName() == targetName) {
                std::unique_ptr<FileSystemElement> t = std::move(*it);
                elements.erase(it);
                return t;
            }
        }
        return nullptr;
    }

    size_t getSize() const override {
        size_t total = 0;
        for (const auto& i : elements) {
            total += i->getSize();
        }
        return total;
    }

    std::string toString() const override {
        return name + " (" + std::to_string(getSize()) + " bytes)";
    }

    void printTree(int level = 0) const override {
        for (int i = 0; i < level; ++i) {
            std::cout << "   ";
        }
        std::cout << "|-- " << name << "\n";
        for (const auto& el : elements) {
            el->printTree(level + 1);
        }
    }
};