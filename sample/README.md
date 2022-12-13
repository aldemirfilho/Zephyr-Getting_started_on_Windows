
# Sample - UART Threads

O exemplo disponível para teste de *building* e *flashing* faz uso de comunicação serial para coletar uma entrada do usuário e em seguida devolver a mesma no formato *upercase*.

## Definição das Tarefas

no arquivo [main.c](https://github.com/Tayco110/Zephyr--Getting_started_on_Windows/blob/main/sample/src/main.c) é definida uma tarefa responsável por aguardar e receber as entradas do usuário e uma outra tarefa responsável por devolver essa estrada no formato esperado.

### [input_task](https://github.com/Tayco110/Zephyr--Getting_started_on_Windows/blob/c822fd284c56d8c774d1867a9ec216d93eee33cb/sample/src/main.c#L35)

```c
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
```

O código acima aguarda por uma entrada do usuário terminada em `\n`. Quando essa entrada é recebida, a mesma é colocada em uma [*message queue*](https://docs.zephyrproject.org/3.0.0/reference/kernel/data_passing/message_queues.html), um tipo de estrutura de dados presente no [Zephyr](https://www.zephyrproject.org/) é similar à um *ring buffer*.

### [output_task](https://github.com/Tayco110/Zephyr--Getting_started_on_Windows/blob/c822fd284c56d8c774d1867a9ec216d93eee33cb/sample/src/main.c#L54)

```c
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
```

O código acima aguarda por uma entrada do usuário ser colocada dentro da [*message queue*](https://docs.zephyrproject.org/3.0.0/reference/kernel/data_passing/message_queues.html) e em seguida devolve a mesma com o tratamento esperado.

### Message Queue 

A estrutura de dados descrita acima foi definida utilizado a uma macro presente no [Zephyr](https://www.zephyrproject.org/)

```c
K_MSGQ_DEFINE(my_msg_q, sizeof(struct string), 1, 1);
```
Mais detalhes podem ser encontrados na [documentação oficial](https://docs.zephyrproject.org/3.0.0/reference/kernel/data_passing/message_queues.html).

### Threads

Por fim, definimos as *threads*, que são responsáveis por determinar qual tarefa estará fazendo uso do processador nos momentos adequados.

```c
K_THREAD_DEFINE(input_task_id , STACKSIZE, input_task, NULL, NULL, NULL, PRIORITY + 1, 0, 0);

K_THREAD_DEFINE(output_task_id, STACKSIZE, output_task, NULL, NULL, NULL, PRIORITY, 0, 0);
```
Como a *thread* de input possui uma prioridade maior, ela sempre vai ocupar o processador até que uma entrada terminada em `\n` seja fornecida.

Mais detalhes podem ser encontrados na [documentação oficial](https://docs.zephyrproject.org/latest/kernel/services/threads/).
