#ifndef VALIDACAO_H
#define VALIDACAO_H

#include "automato.h"

// Verifica se o automato e deterministico (exigencia do trabalho).
// Retorna true se NAO houver conflito entre transicoes; caso contrario
// imprime os pares de transicoes conflitantes e retorna false.
bool verificar_determinismo(const Automato &automato);

// Valida a especificacao lida do arquivo (Q, F, alfabetos, estados e simbolos
// das transicoes) e depois chama verificar_determinismo.
// Retorna true se o automato esta pronto para ser simulado.
bool validar_automato(const Automato &automato);

#endif
