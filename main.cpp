#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h> // Para SetConsoleOutputCP
using namespace std;

struct Aluno {
    Aluno *ante;
    Aluno *prox;

    string matricula;
    string cpf;
    string nome;
    float nota;
    int idade;
    string curso;
    string cidade;
};

struct Alunos {
    Aluno *head;
    Aluno *end;
    int size;
};

Alunos students; // variável global

void inicializa() {
    students.size = 0;
    students.head = nullptr;
    students.end = nullptr;
}

bool insert_ordenado(Aluno *new_student) {
    if (new_student == nullptr) return false;

    
    if (students.head == nullptr) {
        students.head = students.end = new_student;
        new_student->ante = new_student->prox = nullptr;
        students.size++;
        return true;
    }
    Aluno *atual = students.head->prox;
    while (atual != nullptr && new_student->nome >= atual->nome) {
        if (atual->matricula == new_student->matricula || atual->cpf == new_student->cpf) {
            return false; // Já existe um aluno com a mesma matrícula ou CPF
        }
        atual = atual->prox;
    }

    
    if (new_student->nome <= students.head->nome) {
        new_student->prox = students.head;
        new_student->ante = nullptr;
        students.head->ante = new_student;
        students.head = new_student;
    }
   
    else if (new_student->nome >= students.end->nome) {
        new_student->ante = students.end;
        new_student->prox = nullptr;
        students.end->prox = new_student;
        students.end = new_student;
    }
    
    else {
        
        while (atual != nullptr && new_student->nome >= atual->nome) {
            atual = atual->prox;
        }
        new_student->prox = atual;
        new_student->ante = atual->ante;

        atual->ante->prox = new_student;
        atual->ante = new_student;
    }

    students.size++;
    return true;
}

void ler_aluno() {
    ifstream file("../alunos_teste.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;
    getline(file, line); // Ignora cabeçalho

    while (getline(file, line)) {
        vector<string> colunas;

        size_t start = 0;
        size_t end;
        while ((end = line.find(',', start)) != string::npos) {
            colunas.emplace_back(line.substr(start, end - start));
            start = end + 1;
        }
        colunas.emplace_back(line.substr(start));

        if (colunas.size() < 7) continue;

        Aluno *new_student = new Aluno{
            nullptr, nullptr,
            colunas[0],
            colunas[1],
            colunas[2],
            stof(colunas[3]),
            stoi(colunas[4]),
            colunas[5],
            colunas[6]
        };

        //insert_normal(new_student);
        insert_ordenado(new_student);
    }

    file.close();
}

void print_student(Aluno *aluno) {
    cout << aluno->matricula << " - "
         << aluno->cpf << " - "
         << aluno->nome << " - " 
         << aluno->nota << " - "
         << aluno->idade << " - "
         << aluno->curso << " - " 
         << aluno->cidade << endl;
}

void print_list() {
    Aluno *atual = students.head;
    while (atual != nullptr) {
        print_student(atual);
        atual = atual->prox;
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



void remove_student(Aluno *student){
    if (student == nullptr) return;

    if (student == students.head && student == students.end) {
        // Removendo o único elemento
        students.head = students.end = nullptr;    }

    else if (student == students.head) {
        // Removendo o primeiro elemento
        students.head = students.head->prox;
        if (students.head != nullptr) {
            students.head->ante = nullptr;
        } else {
            students.end = nullptr;
        }
    }

    else if (student == students.end) {
        // Removendo o último elemento
        students.end = students.end->ante;
        if (students.end != nullptr) {
            students.end->prox = nullptr;
        } else {
            students.head = nullptr;
        }
    }
    else {
        // Removendo um elemento do meio
        student->ante->prox = student->prox;
        if (student->prox != nullptr) {
            student->prox->ante = student->ante;
        }
    }
    delete student;
    students.size--;
}

void search() {
    string term;
    int option;
    bool found = false;

    cout << "\nDigite o termo de busca (CPF ou Nome): ";
    cin.ignore();
    getline(cin, term);  // Permite nomes com espaços

    Aluno *current = students.head;
    while (current != nullptr) {
        if (current->nome.find(term) != string::npos || current->cpf.find(term) != string::npos) {
            found = true;
            print_student(current);
            cout << "\nDigite 1 para remover esse aluno, qualquer outra tecla para continuar: ";
            cin >> option;
            if (option == 1) {
                Aluno *to_remove = current;
                current = current->prox;  // Avança antes de remover
                remove_student(to_remove);
                continue;  // Pula o incremento normal
            }
        }
        current = current->prox;
    }

    if (!found) {
        cout << "\nNenhum aluno encontrado com o termo fornecido.\n\n";
    }
}


int main(){
    SetConsoleOutputCP(65001); // Define a codificação UTF-8 para o console
    inicializa();
    ler_aluno();
    print_list();

    int option;  
    do{
        option = menu();
        switch (option) {
            case 1: {
                cout << "==== 1 - Buscar aluno ==== " << endl;
                search();
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
