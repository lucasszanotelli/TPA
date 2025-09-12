#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Student {
    string registration;
    string cpf;
    string name;
    float pontuation;
    int age;
    string course;
    string city;
};

struct Node {
    Student* student;
    Node* left;
    Node* right;
    int height;
};

// Função para criar um novo nó
Node* new_node(Student* new_student) {
    Node* node = new Node();
    node->student = new_student;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    return node;
}

// Função para inserir na árvore (ordenada por nome)
Node* insert_tree(Node* root, Student* new_student) {
    if (root == nullptr) {
        return new_node(new_student);
    }

    if (new_student->name < root->student->name) {
        root->left = insert_tree(root->left, new_student);
    } else if (new_student->name > root->student->name) {
        root->right = insert_tree(root->right, new_student);
    } else {
        // nomes iguais não inserimos
        return root;
    }

    return root;
}

// Impressão em ordem (alfabética)
void print_in_order(Node* root) {
    if (root == nullptr) return;

    print_in_order(root->left);
    cout << root->student->name << " - " 
         << root->student->course << " - "
         << root->student->city << endl;
    print_in_order(root->right);
}

// Ler alunos do CSV e montar a árvore
Node* ler_alunos(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return nullptr;
    }

    string line;
    getline(file, line); // ignora cabeçalho

    Node* root = nullptr;

    while (getline(file, line)) {
        vector<string> colunas;
        size_t start = 0, end;

        while ((end = line.find(',', start)) != string::npos) {
            colunas.emplace_back(line.substr(start, end - start));
            start = end + 1;
        }
        colunas.emplace_back(line.substr(start));

        if (colunas.size() < 7) continue;

        Student* new_student = new Student{
            colunas[0], // registration
            colunas[1], // cpf
            colunas[2], // name
            stof(colunas[3]), // pontuation
            stoi(colunas[4]), // age
            colunas[5], // course
            colunas[6]  // city
        };

        root = insert_tree(root, new_student);
    }

    file.close();
    return root;
}

int main() {
    Node* root = ler_alunos("../alunos_teste.csv");

    cout << "Alunos em ordem alfabetica:" << endl;
    print_in_order(root);

    return 0;
}
