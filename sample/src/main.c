#include <zephyr/kernel.h>
#include <zephyr/console/console.h>
#include <zephyr/sys/printk.h>

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

/**
 * @brief Definições das variáveis utilizadas no código
 * 
 */
#define SLEEP_TIME_MS 1000 /**< Tempo de espera em ms*/
#define STACKSIZE 1024 /**< Tamanho da pilha em bytes */
#define PRIORITY  1 /**< Prioridade da thread*/

/**
 * @brief Estrutura que contem os dados recebidos na entrada
 * 
 */
struct string{
    char* typed_line; /**< Referência para a entrada do tipo char*/
};

/**
 * @brief Macro definida no Zephyr-rtos para criação de uma message queue
 * 
 */
K_MSGQ_DEFINE(my_msg_q, sizeof(struct string), 1, 1);

/**
 * @brief Task responsável por aguardar uma entrada do usuário e guardar na fila
 * 
 */
void input_task(void){

    struct string s;
    console_getline_init();

    while (true){

        printk("\n[TYPE A LOWERCASE STRING]: ");

        s.typed_line = console_getline();
        k_msgq_put(&my_msg_q, &s.typed_line, K_NO_WAIT);
        k_msleep(SLEEP_TIME_MS);
    }
}

/**
 * @brief Task responsável por aguardar uma entrada do usuário ser colocada na fila para então retirá-la
 * 
 */
void output_task(void){

    struct string s;

    while (true){

        k_msgq_get(&my_msg_q, &s.typed_line, K_FOREVER);
        k_msleep(SLEEP_TIME_MS);
        
        for (int i = 0; i < strlen(s.typed_line); i++) {
            if ((s.typed_line[i] >= 'a') && ((s.typed_line[i] <= 'z'))) {
                s.typed_line[i] = 'A' + (s.typed_line[i] - 'a');
            }
        }

        printk("[TYPED LINE IN UPERCASE]: %s\n", s.typed_line);
    }
}

/**
 * @brief Definição da Thread responsável por aguardar a entrada com prioridade n + 1
 * 
 */
K_THREAD_DEFINE(input_task_id , STACKSIZE, input_task, NULL, NULL, NULL, PRIORITY + 1, 0, 0);
/**
 * @brief Definição da Thread responsável por aguardar a entrada com prioridade n
 * 
 */
K_THREAD_DEFINE(output_task_id, STACKSIZE, output_task, NULL, NULL, NULL, PRIORITY, 0, 0);