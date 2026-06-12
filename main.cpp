#include "h.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    CircularList students;
    Game game(students);
    bool gamePlayed = false;
    int choice;

    do {
        showMenu();
        choice = getIntInput("", 1, 5);

        try {
            switch (choice) {
                case 1: {
                    std::string filename;
                    std::cout << "Введите имя файла: ";
                    std::cin >> filename;
                    students.clear();
                    loadFromFile(students, filename);
                    gamePlayed = false;
                    break;
                }
                case 2:
                    students.printList();
                    break;
                case 3: {
                    if (students.isEmpty()) {
                        std::cout << "Сначала загрузите студентов из файла!\n";
                        break;
                    }
                    int rounds = getIntInput("Введите количество раундов (1-1000): ", 1, 1000);
                    game.play(rounds);
                    gamePlayed = true;
                    break;
                }
                case 4:
                    if (!gamePlayed) {
                        std::cout << "Сначала сыграйте в игру, чтобы увидеть результаты!\n";
                    } else {
                        game.showResults();
                    }
                    break;
                case 5:
                    std::cout << "До свидания!\n";
                    break;
            }
        }
        catch (const std::exception& e) {
            //выводим тип ошибки
            std::cerr << "Ошибка: " << e.what() << std::endl;
            Sleep(100);
            //очищаем cout 
            std::cin.ignore();
        }
    } while (choice != 5);

    return 0;
}
