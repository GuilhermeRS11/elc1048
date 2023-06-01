/**
 * \file
 *
 * \brief Exemplos diversos de tarefas e funcionalidades de um sistema operacional multitarefas.
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Este arquivo contem exemplos diversos de tarefas e
 * funcionalidades de um sistema operacional multitarefas.
 *
 *
 * \par Conteudo
 *
 * -# Inclui funcoes do sistema multitarefas (atraves de multitarefas.h)
 * -# Inicizalizao do processador e do sistema multitarefas
 * -# Criacao de tarefas de demonstracao
 *
 */

/*
 * Inclusao de arquivos de cabecalhos
 */
#include <asf.h>
#include "stdint.h"
#include "multitarefas.h"

/*
 * Prototipos das tarefas
 */
void tarefa_1(void);
void tarefa_2(void);
void tarefa_3(void);

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_1   		 (TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_2   		 (TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_3   		 (TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_OCIOSA    (TAM_MINIMO_PILHA + 24)

/*
 * Declaracao das pilhas das tarefas
 */
uint32_t PILHA_TAREFA_1[TAM_PILHA_1];
uint32_t PILHA_TAREFA_2[TAM_PILHA_2];
uint32_t PILHA_TAREFA_3[TAM_PILHA_3];
uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
    system_init();
    
    /* Criacao das tarefas */
    /* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
    
    CriaTarefa(tarefa_1, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 2);
    
    CriaTarefa(tarefa_2, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 1);
    
    CriaTarefa(tarefa_3, "Tarefa 3", PILHA_TAREFA_3, TAM_PILHA_3, 3);
    TCB[2].tempo_espera = 100; // A cada 100 ns essa tarefa deve ser executada
    
    /* Cria tarefa ociosa do sistema */
    CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 0);
    
    /* Configura marca de tempo */
    ConfiguraMarcaTempo();   
    
    /* Inicia sistema multitarefas */
    IniciaMultitarefas();
    
    /* Nunca chega aqui */
    while (1)
    {
    }
}

/* Tarefas de exemplo que usam funcoes para suspender/continuar as tarefas */
void tarefa_1(void)
{
    volatile uint16_t a = 0;
    for(;;)
    {
   	 a++;
   	 port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE); /* Liga LED. */
   	 
   	 ExecutaMarcaDeTempo();   			 // Verifica se tem alguma tarefa esperando um tempo pra executar. Se sim, bota ela na fila de prontos. Tbm decrementa o tempo de espera, caso ele seja > 0
   	 if (TCB[2].tempo_espera == 0){
   		 TCB[2].tempo_espera = 100;   	 // Reseta o tempo
   		 TarefaSuspende(1);   			 // Suspende a tarefa atual
   		 TarefaContinua(3);   			 // Continua a tarefa que executa a cada 100ns
   	 }
   	 TarefaContinua(2);
    }
}

void tarefa_2(void)
{
    volatile uint16_t b = 0;
    for(;;)
    {
   	 b++;
   	 port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);     /* Turn LED off. */
   	 
   	 ExecutaMarcaDeTempo();   			  // Verifica se tem alguma tarefa esperando um tempo pra executar. Se sim, bota ela na fila de prontos. Tbm decrementa o tempo de espera, caso ele seja > 0
   	 if (TCB[2].tempo_espera == 0){
   		 TCB[2].tempo_espera = 100;   	  // Reseta o tempo
   		 TarefaSuspende(2);   			  // Suspende a tarefa atual
   		 TarefaContinua(3);   			  // Continua a tarefa que executa a cada 100ns
   	 }
   	 
    }
}

/* TAREFA ESPECIAL QUE EXECUTA A CADA 100 ns */
void tarefa_3(void)
{
    volatile uint16_t c = 0;
    for(;;)
    {
   	 c++;   		 
   	 /* Liga LED. */
   	 port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
   	 TarefaEspera(1000);     /* tarefa 1 se coloca em espera por 3 marcas de tempo (ticks) */
   	 
   	 /* Desliga LED. */
   	 port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
   	 TarefaEspera(1000);     /* tarefa 1 se coloca em espera por 3 marcas de tempo (ticks) */
   	 TarefaSuspende(3);
   	 
    }
}
