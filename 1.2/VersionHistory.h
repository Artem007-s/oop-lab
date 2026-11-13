#pragma once
#include "Tree.h"
#include "DocumentVersion.h"
#include <iostream>
#include <stdexcept>
#include <string>

class VersionHistory {
public:
    VersionHistory() = default;
    ~VersionHistory() = default;

    void create(const std::string& id, const std::string& content, const std::string& msg = "Init") {
        current = tree.createRoot(DocumentVersion(id, content, msg));
    }

    void commit(const std::string& id, const std::string& content, const std::string& msg = "") {
        if (!current)
            throw std::logic_error("No active version");
        current = tree.addChild(current, DocumentVersion(id, content, msg));
    }

    void switc(const std::string& id) {
        Tree<DocumentVersion>::Node* found = findNode(id);
        if (!found)
            throw std::invalid_argument("Version with this ID not found");
        current = found;
    }

    void remove(const std::string& id) {
        Tree<DocumentVersion>::Node* target = findNode(id);
        if (!target)
            throw std::invalid_argument("Version with this ID not found");
        if (isCurrentInSubtree(target))
            current = target->parent;
        tree.removeSubtree(target);
    }

    const DocumentVersion& getCurrent() const {
        if (!current)
            throw std::logic_error("History is empty");
        return current->data;
    }

    void print() const {
        printRecursive(tree.getRoot(), 0);
    }

private:
    Tree<DocumentVersion> tree;
    Tree<DocumentVersion>::Node* current = nullptr;

    Tree<DocumentVersion>::Node* findNode(const std::string& id) {
        return tree.findById(id);
    }

    bool isCurrentInSubtree(Tree<DocumentVersion>::Node* rootNode) const {
        for (const Tree<DocumentVersion>::Node* curr = current; curr; curr = curr->parent) {
            if (curr == rootNode)
                return true;
        }
        return false;
    }

    void printRecursive(const Tree<DocumentVersion>::Node* curr, size_t depth) const {
        if (!curr)
            return;

        for (size_t i = 0; i < depth; ++i) {
            std::cout << "    ";
        }
        std::cout << "└── [" << curr->data.getId() << "] "
                  << "\"" << curr->data.getContent() << "\" "
                  << "(Msg: " << curr->data.getMessage() << ")\n";

        for (const auto& child : curr->children) {
            printRecursive(child.get(), depth + 1);
        }
    }
};