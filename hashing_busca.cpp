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

// Students students; //variável global que define o hashing

struct Hashing{
    Students students[100];
    int size_hashing;
};
Hashing table_hashing;

void initialization(){
    
    for (int i = 0; i < 100; i++){
        table_hashing.students[i].head = nullptr;
        table_hashing.students[i].end = nullptr;
        table_hashing.students[i].size = 0;
    }
}

bool insert_ordenado(Students *students, Student *new_student) {
    if (new_student == nullptr) return false;

    // Verificar duplicados no bucket
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

int lasts_3_number_for_cpf(Student *student){
    string numbers;//variável que receberá a string inteira do cpf com apenas os números

    // Remove pontos e traços
    for (char c : student->cpf) {
        if (isdigit(static_cast<unsigned char>(c))) {
            numbers.push_back(c);
        }
    }
    if (numbers.size() < 2) {
        cerr << "CPF inválido: " << student->cpf << endl;
        return -1;
    }

    string last = numbers.substr(numbers.size() - 3);

    return atoi(last.c_str());

}

int lasts_4_number_for_cpf(Student *student){
    string numbers;//variável que receberá a string inteira do cpf com apenas os números

    // Remove pontos e traços
    for (char c : student->cpf) {
        if (isdigit(static_cast<unsigned char>(c))) {
            numbers.push_back(c);
        }
    }
    if (numbers.size() < 2) {
        cerr << "CPF inválido: " << student->cpf << endl;
        return -1;
    }

    string last = numbers.substr(numbers.size() - 4);

    return atoi(last.c_str());

}

int lasts_5_number_for_cpf(Student *student){
    string numbers;//variável que receberá a string inteira do cpf com apenas os números

    // Remove pontos e traços
    for (char c : student->cpf) {
        if (isdigit(static_cast<unsigned char>(c))) {
            numbers.push_back(c);
        }
    }
    if (numbers.size() < 2) {
        cerr << "CPF inválido: " << student->cpf << endl;
        return -1;
    }

    string last = numbers.substr(numbers.size() - 5);

    return atoi(last.c_str());

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
        int lasts = lasts_3_number_for_cpf(new_student);
        // int lasts = lasts_4_number_for_cpf(new_student);
        // int lasts = lasts_5_number_for_cpf(new_student);

        
        insert_ordenado(&table_hashing.students[lasts], new_student);
    }


    file.close();
}

void remove_student(Student *student){
    if (student == nullptr) return;

    if (student == table_hashing.students->head && student == table_hashing.students->end) {
        // Removendo o único elemento
        table_hashing.students->head = table_hashing.students->end = nullptr;
    }

    else if (student == table_hashing.students->head) {
        // Removendo o primeiro elemento
        table_hashing.students->head = table_hashing.students->head->next;
        if (table_hashing.students->head != nullptr) {
            table_hashing.students->head->previous = nullptr;
        } else {
            table_hashing.students->end = nullptr;
        }
    }

    else if (student == table_hashing.students->end) {
        // Removendo o último elemento
        table_hashing.students->end = table_hashing.students->end->previous;
        if (table_hashing.students->end != nullptr) {
            table_hashing.students->end->next = nullptr;
        } else {
            table_hashing.students->head = nullptr;
        }
    }
    else {
        // Removendo um elemento do meio
        student->previous->next = student->next;
        if (student->next != nullptr) {
            student->next->previous = student->previous;
        }
    }
    delete student;
    table_hashing.students->size--;
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

void print_list() {
    Student *current = table_hashing.students->head;
    while (current != nullptr) {
        print_student(current);
        current = current->next;
    }
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
void search() {
    string term;
    int option;
    bool found = false;

    cout << "\nDigite o termo de busca (CPF ou Nome): ";
    cin.ignore();
    getline(cin, term);  // Permite nomes com espaços

    Student *current = table_hashing.students->head;
    while (current != nullptr) {
        if (current->name.find(term) != string::npos || current->cpf.find(term) != string::npos) {
            found = true;
            print_student(current);
            cout << "\nDigite 1 para remover esse aluno, qualquer outra tecla para continuar: ";
            cin >> option;
            if (option == 1) {
                Student *to_remove = current;
                current = current->next;  // Avança antes de remover
                remove_student(to_remove);
                continue;  // Pula o incremento normal
            }
        }
        current = current->next;
    }

    if (!found) {
        cout << "\nNenhum aluno encontrado com o termo fornecido.\n\n";
    }
}

int main(){

    //comparar tempos e documentar;
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
                cout << "==== 1 - Buscar aluno ==== " << endl;
                search();
                break;
            }
            case 2: {
                cout << "==== 2 - Listar todos os alunos ==== " << endl;
                print_list();
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
