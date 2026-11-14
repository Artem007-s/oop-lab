#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
template <typename T>
class Tree {
public:
    struct Node {
        T data;
        Node* parent;
        std::vector<std::unique_ptr<Node>> children;
        Node(T val, Node* p = nullptr) : data(std::move(val)), parent(p) {}
    };

    Tree() = default;
    ~Tree() = default;
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    bool isEmpty() const {
        return !root;
    }

    Node* getRoot() const {
        return root.get();
    }

    Node* createRoot(T data) {
        if (root)
            throw std::logic_error("Root already exists");
        root = std::make_unique<Node>(std::move(data));
        return root.get();
    }

    Node* addChild(Node* parent, T data) {
        if (!parent)
            throw std::invalid_argument("Parent is null");
        auto child = std::make_unique<Node>(std::move(data), parent);
        Node* n = child.get();
        parent->children.push_back(std::move(child));
        return n;
    }

    void removeSubtree(Node* target) {
        if (!target)
            return;
        if (target == root.get()) {
            root.reset();
            return;
        }
        auto& сhild = target->parent->children;
        for (auto it = сhild.begin(); it != сhild.end(); ++it) {
            if (it->get() == target) {
                сhild.erase(it);
                break;
            }
        }
    }

    Node* findById(const std::string& targetId, Node* curr = nullptr) const {
        if (curr == nullptr) {
            curr = root.get();
        }
        if (!curr) return nullptr;

        if (curr->data.getId() == targetId) return curr;
        for (const auto& child : curr->children) {
            if (auto found = findById(targetId, child.get())) {
                return found;
            }
        }
        return nullptr;
    }
private:
    std::unique_ptr<Node> root;
};