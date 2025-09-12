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
    vector<Student*> student;  // vetor de ponteiros (dinâmico)
    vector<bool> used;         // marca se a posição já foi usada (tombstone)
    int size_hashing;
    int capacity;
};
Hashing table_hashing;

unsigned int calc_name(const string& name) {
    unsigned int soma = 0;
    for (char c : name) soma += static_cast<unsigned int>(c);
    soma = soma * soma; // elevando ao quadrado
    return soma;
}

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int next_prime(int n) {
    if (n <= 2) return 2;
    while (!is_prime(n)) n++;
    return n;
}

void initialization(int tamanho) {
    table_hashing.capacity = tamanho;
    table_hashing.size_hashing = 0;
    table_hashing.student.assign(tamanho, nullptr);
    table_hashing.used.assign(tamanho, false);
}

void fazer_rehashing() {
    int old_size = table_hashing.capacity;
    int new_size = next_prime(old_size * 2);

    vector<Student*> new_table(new_size, nullptr);
    vector<bool> new_used(new_size, false);

    // Reinsere todos os elementos na nova tabela
    for (int i = 0; i < old_size; i++) {
        if (table_hashing.student[i] != nullptr) {
            Student* student = table_hashing.student[i];
            unsigned int hash = calc_name(student->name) % new_size;

            for (int j = 0; j < new_size; j++) {
                int pos = (hash + j) % new_size;
                if (new_table[pos] == nullptr) {
                    new_table[pos] = student;   // guarda ponteiro
                    new_used[pos] = true;
                    break;
                }
            }
        }
    }

    // Substitui os vetores antigos pelos novos (move para eficiência)
    table_hashing.student = std::move(new_table);
    table_hashing.used = std::move(new_used);
    table_hashing.capacity = new_size;

    cout << "Tabela rehashing concluída! Novo tamanho: " << new_size << endl;
}

bool insert_hashing(Student *new_student) {
    double fator = (double)table_hashing.size_hashing / (double)table_hashing.capacity;
    if (fator >= 0.66) {
        cout << "Fator de carga atingido (" << fator << "). Fazendo rehashing..." << endl;
        fazer_rehashing(); //depois daqui, continua a inserir normalmente
    }

    unsigned int hash = calc_name(new_student->name) % table_hashing.capacity;

    for (int i = 0; i < table_hashing.capacity; i++) {
        int pos = (hash + i) % table_hashing.capacity;
        if (table_hashing.student[pos] == nullptr) {
            table_hashing.student[pos] = new_student;
            table_hashing.used[pos] = true;
            table_hashing.size_hashing++;
            return true;
        }
    }

    return false; 
}

Student* search_hashing(const string& nome) {
    unsigned int hash = calc_name(nome) % table_hashing.capacity;

    for (int i = 0; i < table_hashing.capacity; i++) {
        int pos = (hash + i) % table_hashing.capacity;
        if (table_hashing.student[pos] == nullptr) {
            if (!table_hashing.used[pos]) {
                return NULL; // nunca houve nada aqui
            }
            // se used[pos] == true, continui (slot era tombstone)
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

void print_list() {
    for (int i = 0; i < table_hashing.capacity; i++) {
        if (table_hashing.student[i] != nullptr) {
            print_student(table_hashing.student[i]);
        }
    }
}

void calc_re_hashing(Student *new_student, int dado) {
    if (dado == INSERE) {
        if (!insert_hashing(new_student)) {
            cerr << "Erro ao inserir aluno: " << new_student->name << endl;
        }
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
        cerr << "Erro ao abrir o arquivo: " << arquivo << endl;
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

int menu() {
    int choice;
    cout << "Menu:\n";
    cout << "1. Inserir alunos do arquivo\n";
    cout << "2. Listar todos os alunos\n";
    cout << "0. Sair\n";
    cout << "Escolha uma opção: ";
    cin >> choice;
    return choice;
}

int main() {
    SetConsoleOutputCP(65001); // Define UTF-8 no console
    initialization(SIZE); // inicia com SIZE (1021) mas pode crescer
    int option;
    do {
        option = menu();
        switch (option) {
            case 1:
                cout << "==== Inserindo alunos do arquivo ==== " << endl;
                read_student("../alunos_completosV2.csv", INSERE);
                cout << "Alunos inseridos com sucesso! Qtd Alunos: " << table_hashing.size_hashing << endl;
                break;
            case 2:
                cout << "==== Listando todos os alunos ==== " << endl;
                print_list();
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
