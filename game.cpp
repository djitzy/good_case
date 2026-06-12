#include "h.h"

CircularList::CircularList() : head(nullptr), size(0) {}

CircularList::~CircularList() {
    clear();
}

void CircularList::clear() {
    if (head == nullptr) return;
    Node* curr = head->next;
    while (curr != head) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
    delete head;
    head = nullptr;
    size = 0;
}

void CircularList::add(const std::string& surname) {
    Node* newNode = new Node(Student(surname));
    if (head == nullptr) {
        head = newNode;
        head->next = head;
        head->prev = head;
    }
    else {
        Node* tail = head->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = head;
        head->prev = newNode;
    }
    size++;
}

bool CircularList::isEmpty() const { return size == 0; }
int CircularList::getSize() const { return size; }
Node* CircularList::getHead() const { return head; }

Node* CircularList::move(Node* current, int steps, bool clockwise) const {
    if (current == nullptr || size == 0) {
        throw ListException("Ошибка перемещения: список пуст или неверный указатель");
    }
    steps = steps % size;
    if (steps < 0) steps += size;
    
    for (int i = 0; i < steps; ++i) {
        current = clockwise ? current->next : current->prev;
    }
    return current;
}

void CircularList::sortByRatingDesc() {
    if (size <= 1) return;
    //пузырьковая сортировка (устойчивая, сохраняет порядок при равных рейтингах)
    for (int i = 0; i < size - 1; ++i) {
        Node* curr = head;
        for (int j = 0; j < size - i - 1; ++j) {
            if (curr->data.rating < curr->next->data.rating) {
                // Меняем данные местами
                std::string tmpS = curr->data.surname;
                int tmpR = curr->data.rating;
                curr->data.surname = curr->next->data.surname;
                curr->data.rating = curr->next->data.rating;
                curr->next->data.surname = tmpS;
                curr->next->data.rating = tmpR;
            }
            curr = curr->next;
        }
    }
}

void CircularList::printList() const {
    if (head == nullptr) {
        std::cout << "Список пуст.\n";
        return;
    }
    Node* curr = head;
    int pos = 1;
    do {
        std::cout << pos << ". " << curr->data.surname 
                  << " - Рейтинг: " << curr->data.rating << "\n";
        curr = curr->next;
        pos++;
    } while (curr != head);
}

void CircularList::printProtocol(int round, const std::string& surname, int num, int rating) const {
    std::cout << "\n--- Раунд " << round << " ---\n";
    std::cout << "Выпало число: " << num << "\n";
    std::cout << "Направление: " << (num >= 0 ? "по часовой стрелке" : "против часовой стрелки") << "\n";
    std::cout << "Студент: " << surname << "\n";
    std::cout << "Новый рейтинг: " << rating << "\n";
    std::cout << "------------------\n";
}

//Game
Game::Game(CircularList& lst) : list(lst), currentPos(nullptr) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::play(int rounds) {
    if (list.isEmpty()) {
        throw GameException("Сначала загрузите студентов из файла!");
    }
    if (currentPos == nullptr) {
        currentPos = list.getHead();
    }

    for (int i = 1; i <= rounds; ++i) {
        int num = std::rand() % 21 - 10; // Диапазон [-10; 10]
        int steps = std::abs(num);
        bool clockwise = (num >= 0);

        //отсчет начинается со следующего ученика (соседа предыдущего выбранного)
        currentPos = clockwise ? currentPos->next : currentPos->prev;
        currentPos = list.move(currentPos, steps, clockwise);
        
        currentPos->data.rating++;
        list.printProtocol(i, currentPos->data.surname, num, currentPos->data.rating);
        Sleep(100);
    }
    std::cout << "\nИгра завершена!\n";
}

void Game::showResults() {
    std::cout << "\n--- Итоговые результаты (по убыванию рейтинга) ---\n";
    list.sortByRatingDesc();
    list.printList();
}

//утилиты
int getIntInput(const std::string& prompt, int min, int max) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= min && val <= max) return val;
            std::cout << "Ошибка: введите число от " << min << " до " << max << "\n";
        }
        else {
            std::cout << "Ошибка: некорректный ввод.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void loadFromFile(CircularList& list, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException("Не удалось открыть файл: " + filename);
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        //убираем пробелы по краям
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        if (start != std::string::npos) {
            list.add(line.substr(start, end - start + 1));
            count++;
        }
    }
    file.close();
    if (count == 0) {
        throw FileException("Файл пуст или не содержит корректных фамилий.");
    }
    std::cout << "Успешно загружено " << count << " студентов.\n";
}

void showMenu() {
    std::cout << "\n--------------------------------------\n";
    std::cout << "       ИГРА 'ДОБРОЕ ДЕЛО'\n";
    std::cout << "--------------------------------------\n";
    std::cout << "1. Загрузить студентов из файла\n";
    std::cout << "2. Показать текущий список\n";
    std::cout << "3. Начать игру\n";
    std::cout << "4. Показать результаты\n";
    std::cout << "5. Выход\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Выберите пункт меню: ";
}
