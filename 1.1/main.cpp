#include <iostream>
#include <stdexcept>
#include "FileSystem.h"

int main() {
    FileSystem fs("root");

    //Path: оператор / не меняет исходный путь
    Path home("home");
    Path documentsPath = home / "documents";
    if (home.getComponents().size() != 1 || documentsPath.getComponents().size() != 2)
        throw std::runtime_error("Test failed: Path /");
    std::cout << "Тест 1 пройден: Path / возвращает новый путь\n";
    for (auto i : home.getComponents()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl;
    for (auto i : documentsPath.getComponents()) {
        std::cout << i << " ";
    }
    std::cout<<std::endl;

    //Path: == и !=
    if (!(Path("a") == Path("a")) || !(Path("a") != Path("b")))
        throw std::runtime_error("Test failed: Path == !=");
    std::cout << "Тест 2 пройден: Path == и !=\n";

    //Создание папки
    fs.createDirectory(Path("documents"));
    if (fs.find(Path("documents")) == nullptr)
        throw std::runtime_error("Test failed: create directory");
    std::cout << "Тест 3 пройден: создание папки\n";
    fs.printTree();
    std::cout << "\n";

    //Создание файла и поиск
    fs.createFile(Path("documents") / "report.txt", "Hello");
    fs.createFile(Path("documents") / "r.txt", "Buy");
    if (fs.find(Path("documents") / "report.txt") == nullptr)
        throw std::runtime_error("Test failed: create file / find");
    std::cout << "Тест 4 пройден: создание файла и поиск\n";
    fs.printTree();
    std::cout << "\n";

    //Размер файла
    if (fs.getSize(Path("documents") / "report.txt") != 5)
        throw std::runtime_error("Test failed: file size");
    std::cout << "Тест 5 пройден: размер файла\n";
    std::cout<<fs.getSize(Path("documents") / "report.txt")<<std::endl;

    //Размер папки
    if (fs.getSize(Path("documents" )) != 8)
        throw std::runtime_error("Test failed: directory size");
    std::cout << "Тест 6 пройден: размер папки\n";
    std::cout<<fs.getSize(Path("documents"))<<std::endl;

    //Поиск несуществующего элемента
    if (fs.find(Path("nope")) != nullptr)
        throw std::runtime_error("Test failed: find missing");
    std::cout << "Тест 7 пройден: несуществующий элемент не находится\n";

    //Создание с занятым именем
    try {
        fs.createFile(Path("documents") / "report.txt", "1");
        throw std::runtime_error("Test failed: duplicate name");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 8 пройден: занятое имя запрещено\n";
    }

    //Создание без родительской папки
    try {
        fs.createFile(Path("nope") / "x.txt", "1");
        throw std::runtime_error("Test failed: no parent");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 9 пройден: без родительской папки создать нельзя\n";
    }

    //Перемещение
    fs.createDirectory(Path("images"));
    fs.moveElement(Path("documents") / "report.txt", Path("images"));
    if (fs.find(Path("images") / "report.txt") == nullptr ||
        fs.find(Path("documents") / "report.txt") != nullptr)
        throw std::runtime_error("Test failed: move");
    std::cout << "Тест 10 пройден: перемещение файла\n";
    fs.printTree();
    std::cout << "\n";

    //Перемещение корня
    try {
        fs.moveElement(Path(), Path("images"));
        throw std::runtime_error("Test failed: move root");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 11 пройден: корень переместить нельзя\n";
    }

    //Папку в саму себя
    try {
        fs.moveElement(Path("images"), Path("images"));
        throw std::runtime_error("Test failed: move into itself");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 12 пройден: папку в саму себя переместить нельзя\n";
    }

    //Папку в её потомка
    fs.createDirectory(Path("images") / "sub");
    try {
        fs.moveElement(Path("images"), Path("images") / "sub");
        throw std::runtime_error("Test failed: move into descendant");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 13 пройден: папку в её потомка переместить нельзя\n";
    }

    //Конфликт имён при перемещении
    fs.createFile(Path("documents") / "report.txt", "x");
    try {
        fs.moveElement(Path("documents") / "report.txt", Path("images"));
        throw std::runtime_error("Test failed: move name conflict");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 14 пройден: конфликт имён при перемещении\n";
    }

    //Удаление
    fs.deleteElement(Path("images") / "report.txt");
    if (fs.find(Path("images") / "report.txt") != nullptr)
        throw std::runtime_error("Test failed: delete");
    std::cout << "Тест 15 пройден: удаление файла\n";
    fs.printTree();
    std::cout << "\n";

    //Удаление корня
    try {
        fs.deleteElement(Path());
        throw std::runtime_error("Test failed: delete root");
    } catch (const std::invalid_argument&) {
        std::cout << "Тест 16 пройден: корень удалить нельзя\n";
    }

    return 0;
}