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
Node* node = nullptr;

Node* new_node(Student* new_student) {
    Node* novo = new Node;
    novo->student = new_student;
    novo->left = nullptr;
    novo->right = nullptr;
    novo->height = 1;
    return novo;
}

Node* insert_tree(Node* root, Student* new_student) {
    if (root == nullptr) {
        return new_node(new_student);
    }

    if (new_student->name <= root->student->name) {
        root->left = insert_tree(root->left, new_student);
    } else if (new_student->name > root->student->name) {
        root->right = insert_tree(root->right, new_student);
    }
    return root;
}

void print_in_order(Node* root) {
    if (root == nullptr) return;

    print_in_order(root->left);
    cout << root->student->name << " - " 
         << root->student->course << " - "
         << root->student->city << endl;
    print_in_order(root->right);
}

void ler_alunos(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;
    getline(file, line); // ignora cabeçalho

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

        node = insert_tree(node, new_student);
    }

    file.close();
}

int calcular_altura(Node* root) {
    if (root == nullptr) return 0;
    int altura_esq = calcular_altura(root->left);
    int altura_dir = calcular_altura(root->right);
    return 1 + max(altura_esq, altura_dir);
}

int main() {
    ler_alunos("../../arquivos/alunos_completosV2.csv");

    // cout << "Alunos em ordem alfabetica:" << endl;
    // print_in_order(node);
    cout << "Altura da arvore: " << calcular_altura(node) << endl;

    return 0;
}
