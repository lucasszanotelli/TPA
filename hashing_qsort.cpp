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
    Students students[1000000];
    int size_hashing;
};
Hashing table_hashing;

void initialization(){

    for (int i = 0; i < 1000000; i++){
        table_hashing.students[i].head = NULL;
        table_hashing.students[i].end = NULL;
        table_hashing.students[i].size = 0;
    }
}

bool insert_ordenado(Students *students, Student *new_student) {
    if (new_student == NULL) return false;

    // Verificar duplicados no bucket
    Student *current = students->head;
    while (current != NULL) {
        if (current->registration == new_student->registration || current->cpf == new_student->cpf) {
            return false;
        }
        current = current->next;
    }

    // Lista vazia
    if (students->head == NULL) {
        students->head = students->end = new_student;
        new_student->previous = new_student->next = NULL;
    }
    // Inserir no início
    else if (new_student->name < students->head->name) {
        new_student->next = students->head;
        new_student->previous = NULL;
        students->head->previous = new_student;
        students->head = new_student;
    }
    // Inserir no final
    else if (new_student->name >= students->end->name) {
        new_student->previous = students->end;
        new_student->next = NULL;
        students->end->next = new_student;
        students->end = new_student;
    }
    // Inserir no meio
    else {
        current = students->head;
        while (current != NULL && new_student->name >= current->name) {
            current = current->next;
        }
        new_student->next = current;
        new_student->previous = current->previous;
        current->previous->next = new_student;
        current->previous = new_student;
    }

    students->size++;
    table_hashing.size_hashing++;

    if (table_hashing.size_hashing % 100000 == 0) {
        cout << "Lidos " << table_hashing.size_hashing << " alunos." << endl;
    }
    return true;
}

bool insert(Students *students, Student *new_student) {
    if (new_student == NULL) return false;

    // Verificar duplicados no bucket
    Student *current = students->head;
    while (current != NULL) {
        if (current->registration == new_student->registration || current->cpf == new_student->cpf) {
            return false;
        }
        current = current->next;
    }

    if (students->head == NULL) {
        students->head = students->end = new_student;
        new_student->previous = new_student->next = NULL;
    } else {
        students->end->next = new_student;
        new_student->previous = students->end;
        new_student->next = NULL;
        students->end = new_student;
    }

    students->size++;
    table_hashing.size_hashing++;

    if (table_hashing.size_hashing % 100000 == 0) {
        cout << "Lidos " << table_hashing.size_hashing << " alunos." << endl;
    }

    return true;
}

int compareByname(const void *a, const void *b) {
    Student *s1 = *(Student **)a;
    Student *s2 = *(Student **)b;
    return s1->name.compare(s2->name);
}

void sort_students_in_hashing(){
    for(int i = 0; i < table_hashing.size_hashing; i++){
        Students *students = &table_hashing.students[i];
        if (students->size <= 1) continue; // Não há nada para ordenar

        Student **array = new Student*[students->size];
        Student *current = students->head;
        for (int j = 0; j < students->size; j++) {
            array[j] = current;
            current = current->next;
        }

        qsort(array, students->size, sizeof(Student *), compareByname);

        // Recria a lista encadeada ordenada
        students->head = array[0];
        students->end = array[students->size - 1];
        students->head->previous = NULL;
        for (int j = 0; j < students->size; j++) {
            array[j]->next = (j + 1 < students->size) ? array[j + 1] : NULL;
            if (j > 0) {
                array[j]->previous = array[j - 1];
            }
        }
        delete[] array;
    }
}

int lasts_3_number_for_cpf(Student *student){
    string numbers;//variável que receberá a string inteira do cpf com apenas os números

    // Remove pontos e traços
    for (char c : student->cpf) {
        if (isdigit(static_cast<unsigned char>(c))) {
            numbers.push_back(c);
        }
    }
    if (numbers.size() < 3) {
        cerr << "CPF inválido: " << student->cpf << endl;
        return -1;
    }

    string last = numbers.substr(numbers.size() - 3);

    return stoi(last.c_str());

}

int lasts_4_number_for_cpf(Student *student){
    string numbers;//variável que receberá a string inteira do cpf com apenas os números

    // Remove pontos e traços
    for (char c : student->cpf) {
        if (isdigit(static_cast<unsigned char>(c))) {
            numbers.push_back(c);
        }
    }
    if (numbers.size() < 4) {
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
    if (numbers.size() < 5) {
        cerr << "CPF inválido: " << student->cpf << endl;
        return -1;
    }

    string last = numbers.substr(numbers.size() - 5);

    return atoi(last.c_str());

}

int lasts_6_number_for_cpf(Student *student){
    string numbers;//variável que receberá a string inteira do cpf com apenas os números

    // Remove pontos e traços
    for (char c : student->cpf) {
        if (isdigit(static_cast<unsigned char>(c))) {
            numbers.push_back(c);
        }
    }
    if (numbers.size() < 6) {
        cerr << "CPF inválido: " << student->cpf << endl;
        return -1;
    }

    string last = numbers.substr(numbers.size() - 6);

    return atoi(last.c_str());

}

void read_student() {
    ifstream file("../alunos_completosV2.csv");
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
            NULL, NULL,
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
        // int lasts = lasts_6_number_for_cpf(new_student);

        insert(&table_hashing.students[lasts], new_student);
    }


    file.close();
}

void remove_student(Student *student){
    if (student == NULL) return;

    if (student == table_hashing.students->head && student == table_hashing.students->end) {
        // Removendo o único elemento
        table_hashing.students->head = table_hashing.students->end = NULL;
    }

    else if (student == table_hashing.students->head) {
        // Removendo o primeiro elemento
        table_hashing.students->head = table_hashing.students->head->next;
        if (table_hashing.students->head != NULL) {
            table_hashing.students->head->previous = NULL;
        } else {
            table_hashing.students->end = NULL;
        }
    }

    else if (student == table_hashing.students->end) {
        // Removendo o último elemento
        table_hashing.students->end = table_hashing.students->end->previous;
        if (table_hashing.students->end != NULL) {
            table_hashing.students->end->next = NULL;
        } else {
            table_hashing.students->head = NULL;
        }
    }
    else {
        // Removendo um elemento do meio
        student->previous->next = student->next;
        if (student->next != NULL) {
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
    for (int i = 0; i < table_hashing.size_hashing; i++) { // percorre todos os buckets
        Student *current = table_hashing.students[i].head;
        while (current != NULL) { // percorre a lista encadeada dentro do bucket
            print_student(current);
            current = current->next;
        }
    }
}
int menu() {
    cout << " ==== Menu: ==== \n" << endl;
    cout << "1 - Buscar aluno" << endl
         << "2 - Busca Automática" << endl
         << "3 - Listar todos os alunos" << endl
         << "0 - Sair" << endl
         << "\nOpção: ";

    int opcao;
    cin >> opcao;
    return opcao;
}

void search(string term) {
    bool found = false;

    for (int i = 0; i < table_hashing.size_hashing; i++){

        Student *current = table_hashing.students[i].head;
        while (current != NULL) {
            if (current->name.find(term) != string::npos || current->cpf.find(term) != string::npos) {
                found = true;
                print_student(current);
                
            }
            current = current->next;
        }
    }

    if (!found) {
        cout << "\nNenhum aluno encontrado com o termo fornecido.\n\n";
    }
}

void what_term(){
    string term;
    cout << "\nDigite o termo de busca (CPF ou Nome): ";
    cin.ignore();
    getline(cin, term);  // Permite nomes com espaços
    search(term);
    
}

void read_line() {
    ifstream file("../busca_10_alunos.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de busca." << endl;
        return; 
    }

    string line;
    getline(file, line); // descarta o cabeçalho

    while (getline(file, line)) {
        vector<string> columns;
        size_t start = 0, end;

        // divide a linha por vírgulas
        while ((end = line.find(',', start)) != string::npos) {
            columns.emplace_back(line.substr(start, end - start));
            start = end + 1;
        }
        columns.emplace_back(line.substr(start)); // último campo

        if (columns.size() < 3) continue; // garante que tem a coluna do nome

        string term = columns[2]; // terceira coluna = Nome
        cout << "\nBuscando: " << term << endl;
        search(term); // passa só o nome para a função de busca
    }

    file.close();
}

int main(){

    //================ DOCUMENTAÇÃO ===================
    /*
    ORDENAÇÃO COM QUICK SORT:
    Tempo de execução: 76.683 segundos.
    Tempo de execução da ordenação: 0.581 segundos.
    */

    //comparar tempos e documentar;
    SetConsoleOutputCP(65001); // Define a codificação UTF-8 para o console
    
    int time_start = clock();
    initialization();
    read_student();
    int time_start_ordenad = clock();
    sort_students_in_hashing();
    int time_end_ordenad = clock();
    int time_end = clock();
    
    double time_taken = double(time_end - time_start) / CLOCKS_PER_SEC;
    double time_taken_ordenad = double(time_end_ordenad - time_start_ordenad) / CLOCKS_PER_SEC;

    cout << "Tempo de execução: " << time_taken << " segundos." << endl;
    cout << "Tempo de execução da ordenação: " << time_taken_ordenad << " segundos." << endl;

    int option;
    do {
        option = menu();
        switch (option) {
            case 1: {
                cout << "==== 1 - Buscar aluno ==== " << endl;
                what_term();
                

                break;
            }
            case 2: {
                cout << "==== 2 - Busca Automática ==== " << endl;
                clock_t time_start1 = clock();
                read_line();//função de busca automática
                clock_t time_end1 = clock();
                double time_taken1 = double(time_end1 - time_start1) / CLOCKS_PER_SEC;
                cout << "Tempo de execução: " << time_taken1 << " segundos." << endl;
                break;
            }
            case 3: {
                cout << "==== 3 - Listar todos os alunos ==== " << endl;
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
