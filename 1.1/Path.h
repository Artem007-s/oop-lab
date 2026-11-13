#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

class Path {
private:
    std::vector<std::string> components;
    static void validName(const std::string& name) {
        if (name.empty() || name.find('/') != std::string::npos)
            throw std::invalid_argument("Invalid path component");
    }
public:
    Path() = default;
    Path(const std::vector<std::string>& parts) : components(parts) {
        for (const auto& part : components) {
            validName(part);
        }
    }
    Path(const std::string& name) {
        validName(name);
        components.push_back(name);
    }
    Path operator/(const std::string& name) const {
        validName(name);
        Path result = *this;
        result.components.push_back(name);
        return result;
    }
    Path operator/(const Path& other) const {
        Path result = *this;
        for (const auto& part : other.components) {
            result.components.push_back(part);
        }
        return result;
    }
    bool operator==(const Path& other) const {
        return components == other.components;
    }
    bool operator!=(const Path& other) const {
        return components != other.components;
    }
    const std::vector<std::string>& getComponents() const {
        return components;
    }
};