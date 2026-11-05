#include "validacao.h"
#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

//converte "q3" pra 3. retorna -1 se o nome do estado for invalido.
int numero_estado(const string &estado){
    if(estado.size() < 2 || estado[0] != 'q') return -1;
    for(size_t i = 1; i < estado.size(); i++){
        if(!isdigit((unsigned char)estado[i])) return -1;
    }
    return stoi(estado.substr(1));
}

//indica se o simbolo pertence ao alfabeto.
bool pertence(const vector<string> &alfabeto, char simbolo){
    for(const string &s:alfabeto){
        if(s.size() == 1 && s[0] == simbolo) return true;
    }
    return false;
}

//monta o texto de uma transição para as mensagens de erro.
string texto_transicao(const Transicao &t){
    return "(" + t.estadoAtual + "," + string(1, t.entrada) + "," + string(1, t.topo) +
           ") = (" + t.proximoEstado + "," + t.operacaoPilha + ")";
}

//verifica se o autômato é determinístico, apontando transições em conflito.
bool verificar_determinismo(const Automato &automato){
    bool ok = true;
    const vector<Transicao> &T = automato.transicoes;

    for(size_t i = 0; i < T.size(); i++){
        for(size_t j = i + 1; j < T.size(); j++){
            if(T[i].estadoAtual != T[j].estadoAtual) continue;

            // topos se sobrepoem: iguais ou algum deles e '@' (qualquer topo)
            bool topoSobrepoe = T[i].topo == T[j].topo || T[i].topo == '@' || T[j].topo == '@';

            // entradas conflitam: iguais, ou uma e vazia (@) e a outra le simbolo
            bool entradaConflita = T[i].entrada == T[j].entrada ||
                                   T[i].entrada == '@' || T[j].entrada == '@';

            if(topoSobrepoe && entradaConflita){
                cout << "Nao-determinismo: " << texto_transicao(T[i])
                     << "  x  " << texto_transicao(T[j]) << endl;
                ok = false;
            }
        }
    }
    return ok;
}

//valida estados, símbolos e operações de pilha da especificação e chama a verificação de determinístico.
bool validar_automato(const Automato &automato){
    bool ok = true;

    if(automato.Q <= 0){
        cout<<"Erro: numero de estados (Q) invalido ou arquivo do automato vazio.\n";
        return false;
    }
    if(automato.transicoes.empty()){
        cout<<"Erro: nenhuma transicao foi lida.\n";
        return false;
    }

    for(const string &f:automato.estadosFinais){
        int n = numero_estado(f);
        if(n < 0 || n >= automato.Q){
            cout<<"Erro: estado final invalido: " << f << endl;
            ok = false;
        }
    }

    for(const Transicao &t : automato.transicoes){
        int a = numero_estado(t.estadoAtual), p = numero_estado(t.proximoEstado);
        if(a < 0 || a >= automato.Q || p < 0 || p >= automato.Q){
            cout<<"Erro: estado inexistente em " << texto_transicao(t) << endl;
            ok = false;
        }
        if(t.entrada != '@' && !pertence(automato.alfabetoFita, t.entrada)){
            cout<<"Erro: simbolo de fita '" << t.entrada << "' fora do alfabeto em "<< texto_transicao(t) << endl;
            ok = false;
        }
        if(t.topo != '@' && t.topo != '$' && !pertence(automato.alfabetoPilha, t.topo)){
            cout<<"Erro: simbolo de pilha '"<< t.topo <<"' fora do alfabeto em "<< texto_transicao(t) << endl;
            ok = false;
        }
        if(t.operacaoPilha.empty()){
            cout<<"Erro: operacao de pilha vazia em "<< texto_transicao(t) << endl;
            ok = false;
        }else if(t.operacaoPilha != "@"){
            for(char c : t.operacaoPilha){
                if(c != '$' && !pertence(automato.alfabetoPilha, c)){
                    cout<<"Erro: simbolo de pilha '"<< c << "' fora do alfabeto em "<< texto_transicao(t) << endl;
                    ok = false;
                }
            }
        }
    }

    if(!verificar_determinismo(automato)) ok = false;
    if(!ok) cout<<"Automato rejeitado: corrija o arquivo e tente novamente.\n";
    return ok;
}
