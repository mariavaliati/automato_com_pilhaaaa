#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <set>
#include <vector>
using namespace std;

typedef struct{
    string estadoAtual;
    char entrada;
    char topo;
    string proximoEstado;
    string operacaoPilha;
} Transicao;

typedef struct{
    int Q;
    vector<string>estadosFinais;
    vector<string>alfabetoFita; //{a,b,c,d}
    vector<string>alfabetoPilha; //{X,Y}
    vector<Transicao>transicoes;
}Automato;

void ler_arquivo(string nome, Automato &automato);
void ler_transicoes(ifstream &arquivo, Automato &automato, string linha);
Transicao buscar_transicao(Automato automato, string estado, char entrada, char topo);
void op_pilha(stack<char>&pilha, string operacao);
bool simular(Automato automato, string cadeia);

#endif