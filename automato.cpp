#include "automato.h"
#include "aux.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <set>
#include <vector>
using namespace std;

//lê o arquivo do autômato a partir do seu nome e preenche as informações do autômato já corretamente (tirando vírgula, transformando em inteiro, etc).
void ler_arquivo(string nome, Automato &automato){
    ifstream arquivo(nome);
    if(!arquivo.is_open()){
        cout << "Arquivo nao encontrado!\n";
        return;
    }
    
    string linha;
    while(getline(arquivo, linha)){
        if(linha.empty()) continue;
        
        if(linha[0] == 'Q'){
            automato.Q = stoi(linha.substr(2));
        }else if(linha[0] == 'F'){
            automato.estadosFinais = tirar_virgula(tirar_chaves(linha));
        }else if(linha.find("alfabeto_pilha") != string::npos){
            automato.alfabetoPilha = tirar_virgula(tirar_chaves(linha));
        }else if(linha.find("alfabeto_fita") != string::npos){
            automato.alfabetoFita = tirar_virgula(tirar_chaves(linha));
        }else if(linha[0] == '('){
            ler_transicoes(arquivo, automato, linha);
            break;
        }
    }
}

// lê as linhas das transições e guarda cada uma na lista de transições do automâto.
void ler_transicoes(ifstream &arquivo, Automato &automato, string linha){
    do {
        if(linha.empty() || linha[0] != '(') continue;
        
        size_t ini1 = linha.find('('), fim1 = linha.find(')');
        size_t ini2 = linha.find('(', fim1), fim2 = linha.find(')', ini2);
        
        if(ini1 == string::npos || fim1 == string::npos || ini2 == string::npos || fim2 == string::npos) continue;
        
        stringstream ssEsquerda(linha.substr(ini1 + 1, fim1 - ini1 - 1));
        stringstream ssDireita(linha.substr(ini2 + 1, fim2 - ini2 - 1));
        string estadoAtual, entrada, topo, proximoEstado, operacaoPilha;
        
        getline(ssEsquerda, estadoAtual, ',');
        getline(ssEsquerda, entrada, ',');
        getline(ssEsquerda, topo, ',');
        getline(ssDireita, proximoEstado, ',');
        getline(ssDireita, operacaoPilha, ',');
        
        if(entrada.empty() || topo.empty()) continue;
        
        Transicao transicao;
        transicao.estadoAtual = estadoAtual;
        transicao.entrada = entrada[0];
        transicao.topo = topo[0];
        transicao.proximoEstado = proximoEstado;
        transicao.operacaoPilha = operacaoPilha;
        automato.transicoes.push_back(transicao);
    } 
    while(getline(arquivo, linha));
}

//procura a transição que se aplica ao estado, ao símbolo da fita e ao topo da pilha atuais.
Transicao buscar_transicao(Automato automato, string estado, char entrada, char topo){
    for(auto &transicao : automato.transicoes){
        if(transicao.estadoAtual == estado && transicao.entrada == entrada &&
           (transicao.topo == topo || transicao.topo == '@')){
            return transicao;
        }
    }
    return Transicao{};
}

//empilha na pilha a cadeia indicada pela transição.
void op_pilha(stack<char>& pilha, string operacao){
    if(operacao != "@"){
        for(int j = (int)operacao.size() - 1; j >= 0; j--){
            pilha.push(operacao[j]);
        }
    }
}

//imprime um passo do processamento (fita com o estado entre colchetes e o conteúdo da pilha).
void imprimir_configuracao(string cadeia, int posicao, string estado, stack<char> pilha){
    cout << "Fita: ";
    for(int j = 0; j <= (int)cadeia.size(); j++){
        if(j == posicao) cout << "[" << estado << "]";
        if(j < (int)cadeia.size()) cout << cadeia[j];
    }
    cout << " Pilha: ";
    while(!pilha.empty()){
        cout << pilha.top();
        pilha.pop();
    }
    cout << endl;
}

//executa o autômato sobre uma cadeia e informa se ela é aceita ou rejeitada.
bool simular(Automato automato, string cadeia){
    string estado = "q0";
    stack<char> pilha;
    int i = 0;
    set<string> visitados;

    while(true){
        char topo = pilha.empty() ? '@' : pilha.top();
        imprimir_configuracao(cadeia, i, estado, pilha);

        stack<char> copia = pilha;
        string conteudoPilha;
        while(!copia.empty()){
            conteudoPilha += copia.top();
            copia.pop();
        }
        string configuracao = estado + "|" + to_string(i) + "|" + conteudoPilha;
        if(visitados.count(configuracao)) return false;
        visitados.insert(configuracao);

        Transicao transicao{};
        bool transicaoVazia = false;
        if(i < (int)cadeia.size()){
            transicao = buscar_transicao(automato, estado, cadeia[i], topo);
        }
        if(transicao.estadoAtual.empty()){
            transicao = buscar_transicao(automato, estado, '@', topo);
            transicaoVazia = true;
        }
        if(transicao.estadoAtual.empty()){
            if(i == (int)cadeia.size()){
                for(const string &estadoFinal : automato.estadosFinais){
                    if(estado == estadoFinal) return true;
                }
            }
            return false;
        }
        if(transicao.topo != '@' && !pilha.empty()) pilha.pop();
        op_pilha(pilha, transicao.operacaoPilha);
        estado = transicao.proximoEstado;
        if(!transicaoVazia) i++;
    }
}
