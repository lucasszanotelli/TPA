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
#define SIZE 10000

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

//é um vetor que armazena uma estrutura de lista em cada ponta
struct Hashing{
    Students students[SIZE];
    int size_hashing;
};
Hashing table_hashing;

void initialization(){//inicializa a hashing 
    for (int i = 0; i < SIZE; i++){
        table_hashing.students[i].head = nullptr;
        table_hashing.students[i].end = nullptr;
        table_hashing.students[i].size = 0;
    }
}

bool insert_ordenado(Students *students, Student *new_student) {
    if (new_student == nullptr) return false;


    Student *current = students->head;
    while (current != nullptr) {
        if (current->registration == new_student->registration || current->cpf == new_student->cpf) {
            return false;
        }
        current = current->next;
    }

    // Lista vazia
    if (students->head == nullptr) {
        students->head = students->end = new_student;
        new_student->previous = new_student->next = nullptr;
    }
    // Inserir no início
    else if (new_student->name < students->head->name) {
        new_student->next = students->head;
        new_student->previous = nullptr;
        students->head->previous = new_student;
        students->head = new_student;
    }
    // Inserir no final
    else if (new_student->name >= students->end->name) {
        new_student->previous = students->end;
        new_student->next = nullptr;
        students->end->next = new_student;
        students->end = new_student;
    }
    // Inserir no meio
    else {
        current = students->head;
        while (current != nullptr && new_student->name >= current->name) {
            current = current->next;
        }
        new_student->next = current;
        new_student->previous = current->previous;
        current->previous->next = new_student;
        current->previous = new_student;
    }

    students->size++;
    table_hashing.size_hashing++;

    if (table_hashing.size_hashing % 1000 == 0) {
        cout << "Lidos " << table_hashing.size_hashing << " alunos." << endl;
    }
    return true;
}

void read_student() {
    ifstream file("../../arquivos/alunos_completosV2.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;
    getline(file, line); // Ignora cabeçalho

    while (getline(file, line)) {
        vector<string> columns;

        size_t start = 0;
        size_t end;
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
        int lasts2 = stoi(new_student->cpf.substr(12, 2)); //separa os dois últimos dígitos do CPF

        insert_ordenado(&table_hashing.students[lasts2], new_student);
    }
    file.close();
}


int menu() {
    cout << " ==== Menu: ==== \n" << endl;
    cout << "1 - Buscar aluno" << endl
         << "0 - Sair" << endl
         << "\nOpção: ";
    int opcao;
    cin >> opcao;
    return opcao;
}

int main(){
    SetConsoleOutputCP(65001); // Define a codificação UTF-8 para o console
    
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
            case 1: {
                cout << "==== 1 - Buscar aluno ==== " << endl;''
                break;
            }
            case 2: {
                cout << "==== 2 - Listar todos os alunos ==== " << endl;
                break;
            }
            case 0:
                cout << "\nSaindo..." << endl;
                break;
            default:
                cout << "\nOpção inválida." << endl;
        }
    } while (option != 0);


    return 0;
}
