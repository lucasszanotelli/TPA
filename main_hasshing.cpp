#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h> // Para SetConsoleOutputCP
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

Students a; //variável global que define o hashing

struct Hashing{
    Students a[100];
    int size_hashing;
}

void inicilization(){
    for (int i = 0; i < 100; i++){
        a[i].head = nullptr;
        a[i].end = nullptr;
        a[i].size = 0;
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
