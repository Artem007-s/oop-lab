#include <iostream>
#include <cassert>
#include <stdexcept>
#include "Tree.h"
#include "DocumentVersion.h"
#include "VersionHistory.h"

void runAutomaticTests() {
    std::cout << "=== Запуск автоматических тестов ==\n";

    VersionHistory history;

    //Тест создания начальной версии
    history.create("v1.0", "Начальное состояние документа", "Init commit");
    assert(history.getCurrent().getId() == "v1.0");
    assert(history.getCurrent().getContent() == "Начальное состояние документа");
    std::cout << "[PASS] Создание начальной версии успешно.\n";

    //Тест создания веток
    history.commit("v1.1", "Добавлен раздел 1", "Feature A");
    assert(history.getCurrent().getId() == "v1.1");

    // Возврат к v1.0 и создание другой ветки
    history.switc("v1.0");
    history.commit("v1.2", "Добавлен раздел 2", "Feature B");
    assert(history.getCurrent().getId() == "v1.2");
    std::cout << "[PASS] Ветвление истории успешно.\n";

    // Тест обработки ошибок (поиск несуществующей версии)
    bool exceptionThrown = false;
    try {
        history.switc("non_existent_id");
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    std::cout << "[PASS] Обработка исключений для несуществующих версий работает корректно.\n";

    // Тест удаления узла и корректного смещения current, если удаляется активная ветка
    history.switc("v1.1");
    assert(history.getCurrent().getId() == "v1.1");

    // Удаляем узел v1.1, current должен автоматически переключиться на родителя (v1.0)
    history.remove("v1.1");
    assert(history.getCurrent().getId() == "v1.0");
    std::cout << "[PASS] Корректное удаление узла и смещение указателя текущей версии прошли успешно.\n";

    std::cout << "=== Все автоматические тесты успешно пройдены! ===\n\n";
}

int main() {
    runAutomaticTests();

    VersionHistory docHistory;

    docHistory.create("v1", "Главный документ. Старт.", "Первый коммит");

    docHistory.commit("v2-alpha", "Экспериментальная фича 1", "Ветка alpha");
    docHistory.commit("v3-alpha", "Доработка экспериментов", "Продолжение alpha");

    docHistory.switc("v1");
    docHistory.commit("v2-release", "Стабильная версия ядра", "Релизный коммит");
    docHistory.commit("v3-release", "Исправление багов продакшена", "Хотфикс");

    std::cout << "\nТекущее дерево истории версий документа:\n";
    docHistory.print();

    std::cout << "\nТекущая активная версия: [" << docHistory.getCurrent().getId()
              << "] — \"" << docHistory.getCurrent().getContent()
              << "\" (Сообщение: " << docHistory.getCurrent().getMessage() << ")\n";

    return 0;
}