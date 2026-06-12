#ifndef H_H
#define H_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <windows.h>

//исключения
class FileException : public std::runtime_error {
public:
    explicit FileException(const std::string& msg) : std::runtime_error(msg) {}
};

class ListException : public std::runtime_error {
public:
    explicit ListException(const std::string& msg) : std::runtime_error(msg) {}
};

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg) : std::runtime_error(msg) {}
};

//данные
struct Student {
    std::string surname;
    int rating;
    Student() : surname(""), rating(0) {}
    Student(const std::string& s) : surname(s), rating(0) {}
};

struct Node {
    Student data;
    Node* next;
    Node* prev;
    Node(const Student& s) : data(s), next(nullptr), prev(nullptr) {}
};

//классы
class CircularList {
private:
    Node* head;
    int size;
public:
    CircularList();
    ~CircularList();

    void add(const std::string& surname);
    bool isEmpty() const;
    int getSize() const;
    Node* getHead() const;

    //перемещение по списку
    Node* move(Node* current, int steps, bool clockwise) const;

    //сортировка и вывод
    void sortByRatingDesc();
    void printList() const;
    void printProtocol(int round, const std::string& surname, int num, int rating) const;

    void clear();
};

class Game {
private:
    CircularList& list;
    Node* currentPos;
public:
    Game(CircularList& lst);
    void play(int rounds);
    void showResults();
};

//утилиты
int getIntInput(const std::string& prompt, int min, int max);
void loadFromFile(CircularList& list, const std::string& filename);
void showMenu();

#endif // H_H
