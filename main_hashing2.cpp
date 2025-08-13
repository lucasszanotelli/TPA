#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h> // Para SetConsoleOutputCP
#include <time.h>
using namespace std;

struct Student {
    Student *previous;
    Student *next;

    string registration;
    string cpf;
    string name;
    float pontuation;
    int age;
    string course;
    string city;
};

struct Students {
    Student *head;
    Student *end;
    int size;
};

struct Hashing {
    Students students[100]; // 100 buckets
    int total_size;         // Total de alunos
};
Hashing table_hashing;

void initialization() {
    for (int i = 0; i < 100; i++) {
        table_hashing.students[i].head = nullptr;
        table_hashing.students[i].end = nullptr;
        table_hashing.students[i].size = 0;
    }
    table_hashing.total_size = 0;
}

bool insert_ordenado(Students *bucket, Student *new_student) {
    if (!new_student) return false;

    // Evita duplicados (CPF ou matrícula iguais)
    Student *check = bucket->head;
    while (check) {
        if (check->cpf == new_student->cpf) {
            return false;
        }
        check = check->next;
    }

    // Inserção no início (lista vazia)
    if (bucket->head == nullptr) {
        bucket->head = bucket->end = new_student;
        new_student->previous = new_student->next = nullptr;
    }
    // Inserção no início (nome menor)
    else if (new_student->name < bucket->head->name) {
        new_student->next = bucket->head;
        new_student->previous = nullptr;
        bucket->head->previous = new_student;
        bucket->head = new_student;
    }
    // Inserção no final (nome maior)
    else if (new_student->name >= bucket->end->name) {
        new_student->previous = bucket->end;
        new_student->next = nullptr;
        bucket->end->next = new_student;
        bucket->end = new_student;
    }
    // Inserção no meio
    else {
        Student *current = bucket->head;
        while (current && new_student->name >= current->name) {
            current = current->next;
        }
        new_student->next = current;
        new_student->previous = current->previous;
        current->previous->next = new_student;
        current->previous = new_student;
    }

    bucket->size++;
    table_hashing.total_size++;

    // Log a cada 1000 alunos
    if (table_hashing.total_size % 1000 == 0) {
        cout << "Lidos " << table_hashing.total_size << " alunos." << endl;
    }
    return true;
}

void read_student() {
    ifstream file("../alunos_teste.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;
    getline(file, line); // Ignora cabeçalho

    while (getline(file, line)) {
        vector<string> columns;
        size_t start = 0, end;
        while ((end = line.find(',', start)) != string::npos) {
            columns.emplace_back(line.substr(start, end - start));
            start = end + 1;
        }
        columns.emplace_back(line.substr(start));

        if (columns.size() < 7) continue;

        Student *new_student = new Student{
            nullptr, nullptr,
            columns[0],
            columns[1],
            columns[2],
            stof(columns[3]),
            stoi(columns[4]),
            columns[5],
            columns[6]
        };

        // Hash pelos 2 últimos dígitos do CPF
        int index = stoi(new_student->cpf.substr(12, 2));
        insert_ordenado(&table_hashing.students[index], new_student);
    }

    file.close();
}

void remove_student(Students *bucket, Student *student) {
    if (!student) return;

    if (student == bucket->head && student == bucket->end) {
        bucket->head = bucket->end = nullptr;
    }
    else if (student == bucket->head) {
        bucket->head = bucket->head->next;
        if (bucket->head) bucket->head->previous = nullptr;
    }
    else if (student == bucket->end) {
        bucket->end = bucket->end->previous;
        if (bucket->end) bucket->end->next = nullptr;
    }
    else {
        student->previous->next = student->next;
        if (student->next) student->next->previous = student->previous;
    }

    delete student;
    bucket->size--;
    table_hashing.total_size--;
}

void print_student(Student *student) {
    cout << student->registration << " - "
         << student->cpf << " - "
         << student->name << " - "
         << student->pontuation << " - "
         << student->age << " - "
         << student->course << " - "
         << student->city << endl;
}

void print_all_students() {
    for (int i = 0; i < 100; i++) {
        Student *current = table_hashing.students[i].head;
        while (current) {
            print_student(current);
            current = current->next;
        }
    }
}

void search() {
    string term;
    int option;
    bool found = false;

    cout << "\nDigite o termo de busca (CPF ou Nome): ";
    cin.ignore();
    getline(cin, term);

    for (int i = 0; i < 100; i++) {
        Student *current = table_hashing.students[i].head;
        while (current) {
            if (current->name.find(term) != string::npos || current->cpf.find(term) != string::npos) {
                found = true;
                print_student(current);
                cout << "\nDigite 1 para remover esse aluno, qualquer outra tecla para continuar: ";
                cin >> option;
                if (option == 1) {
                    Student *to_remove = current;
                    current = current->next;
                    remove_student(&table_hashing.students[i], to_remove);
                    continue;
                }
            }
            current = current->next;
        }
    }

    if (!found) {
        cout << "\nNenhum aluno encontrado com o termo fornecido.\n\n";
    }
}

int menu() {
    cout << " ==== Menu: ==== \n" << endl;
    cout << "1 - Buscar aluno" << endl
         << "2 - Listar todos" << endl
         << "0 - Sair" << endl
         << "\nOpção: ";
    int opcao;
    cin >> opcao;
    return opcao;
}

int main() {
    SetConsoleOutputCP(65001);
    int time_start = clock();

    initialization();
    read_student();

    int time_end = clock();
    double time_taken = double(time_end - time_start) / CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << time_taken << " segundos." << endl;

    int option;
    do {
        option = menu();
        switch (option) {
            case 1:
                search();
                break;
            case 2:
                print_all_students();
                break;
            case 0:
                cout << "\nSaindo..." << endl;
                break;
            default:
                cout << "\nOpção inválida." << endl;
        }
    } while (option != 0);

    return 0;
}
