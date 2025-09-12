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

#define INSERE 0
#define BUSCA 1
#define SIZE 1021

struct Student {
    string registration;
    string cpf;
    string name;
    float pontuation;
    int age;
    string course;
    string city;
};

struct Hashing {
    Student* student[SIZE];  // vetor de ponteiros
    bool used[SIZE];         // marca se a posição já foi usada
    int size_hashing;
};
Hashing table_hashing;

unsigned int calc_name(string name) {
    unsigned int soma = 0;
    for (char c : name) {
        soma += (int)c; // soma todos os valores ASCII
    }
    soma = soma * soma; // eleva ao quadrado
    return soma;
}
void fazer_rehashing(){
    
}

bool insert_hashing(Student *new_student) {
    if (table_hashing.size_hashing >= (SIZE * 0.66)) {
        cout << "Fator de carga atingido, fazendo rehasing" << endl;
        return false;
    }

    unsigned int hash = calc_name(new_student->name) % SIZE;

    for (int i = 0; i < SIZE; i++) {
        int pos = (hash + i) % SIZE;
        if (table_hashing.student[pos] == nullptr) {
            table_hashing.student[pos] = new_student;
            table_hashing.used[pos] = true; // marca como usado
            table_hashing.size_hashing++;
            return true;
        }
    }
    if(table_hashing.size_hashing % 100 == 0){
        cout << table_hashing.size_hashing << "Pessoas inseridas" << endl;
    }
    return false; // tabela cheia
}

Student* search_hashing(string nome) {
    unsigned int hash = calc_name(nome) % SIZE;

    for (int i = 0; i < SIZE; i++) {
        int pos = (hash + i) % SIZE;
        if (table_hashing.student[pos] == nullptr) {
            if (!table_hashing.used[pos]) {
                return nullptr; // nunca houve nada aqui -> para
            }
        } else if (table_hashing.student[pos]->name == nome) {
            return table_hashing.student[pos];
        }
    }
    return nullptr;
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

void calc_re_hashing(Student *new_student, int dado) {
    if (dado == INSERE) {
        insert_hashing(new_student);
    } else if (dado == BUSCA) {
        Student *res = search_hashing(new_student->name);
        if (res) {
            cout << "Aluno encontrado: ";
            print_student(res);
        } else {
            cout << "Aluno não encontrado!\n";
        }
    }
}

void read_student(string arquivo, int dado) {
    ifstream file(arquivo);
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
            columns[0],
            columns[1],
            columns[2],
            stof(columns[3]),
            stoi(columns[4]),
            columns[5],
            columns[6]
        };

        calc_re_hashing(new_student, dado);
    }
    file.close();
}

void initialization() {
    for (int i = 0; i < SIZE; i++) {
        table_hashing.student[i] = nullptr;
        table_hashing.used[i] = false;
    }
    table_hashing.size_hashing = 0;

    read_student("../alunos_completosV2.csv", INSERE);
}

int main() {
    SetConsoleOutputCP(65001); // Define UTF-8 no console
    
    int time_start = clock();
    initialization();
    int time_end = clock();
    
    double time_taken = double(time_end - time_start) / CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << time_taken << " segundos." << endl;

    return 0;
}
