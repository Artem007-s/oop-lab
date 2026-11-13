#pragma once
#include <string>

class DocumentVersion {
private:
    std::string id;
    std::string content;
    std::string message;
public:
    DocumentVersion(std::string id, std::string content, std::string msg = "")
    : id(std::move(id)), content(std::move(content)), message(std::move(msg)) {}

    const std::string& getId() const {
        return id;
    }
    const std::string& getContent() const {
        return content;
    }
    const std::string& getMessage() const {
        return message;
    }
};