
# Zephyr - Getting started on Windows

## Sistema Operacional

Garanta que seu Sistema Operacional esteja atualizado.

```
Iniciar -> Configurações -> Windows Update -> Verificar se há atualizações.
Instale qualquer atualização obrigatória que esteja disponível.
```

> **_NOTA:_**  Devido a problemas para encontrar executáveis, o Projeto Zephyr atualmente não oferece suporte a *flashing* de aplicativos usando o Windows Subsystem for Linux ([WSL](https://learn.microsoft.com/en-us/windows/wsl/about)). Portanto, não recomendamos o uso do [WSL](https://learn.microsoft.com/en-us/windows/wsl/about) ao começar.

## Instalando depêndencias

| Ferramenta | Versão mínima |
| :------: | :------: |
| [CMake](https://cmake.org/) | 3.20.0 |
| [Python](https://www.python.org/) | 3.8.10 |
| [Chocolatey](https://chocolatey.org/install) | --- |

As instruções apresentadas baixo devem ser executadas no `cmd.exe` do Windows em sua versão de administrador.

1. Instale o [CMake](https://cmake.org/).
2. Instale o [Python](https://www.python.org/).
3. Instale o [Chocolatey](https://chocolatey.org/install).

* Abra o `cmd.exe`.
* Desative a confirmação global para evitar ter que confirmar a instalação de programas individuais:
    
    ```bash
    choco feature enable -n allowGlobalConfirmation
    ```
* Use o comando `choco` para instalar as seguintes depêndencias:

    ```bash
    choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
    choco install ninja gperf python git dtc-msys2 wget unzip
    ```
4. Feche a janela atual do `cmd.exe` e reabra como administrador para continuar.

## Obtendo o Zephyr-rtos e depêndencias do Python

Para começar, é recomendado a criação de um diretório em seu local de preferência antes da execução dos próximos passos. Neste tutorial utilizaremos um diretório chamado `workspace`.

1. Em seu terminal execute o seguinte comando para instalar o [west](https://docs.zephyrproject.org/latest/develop/west/index.html):

    ```bash
    pip3 install -U west
    ```
2. Obtenha o código fonte do Zephyr:

    ```bash
    cd workspace
    west init zephyrproject
    ````

* Neste ponto, a seguinte estrutura de diretórios deve está montada: 

    ```
    workspace/
    └──zephyrproject/
    ```

3. Em seguida, vá para o diretório `zephyrproject` e utilize o seguinte comando:

    ```bash
    cd zephyrproject
    west update
    ```

4. Exporte o pacote Zephyr-CMake. Isso permite que o CMake carregue automaticamente o código necessário para criar projetos com o Zephyr:
    
    ```bash
    west zephyr-export
    ```

5. Instales as depêndencias adicionais do Python:

    ```bash
    pip3 install -r workspace\zephyrproject\zephyr\scripts\requirements.txt
    ```

## Instale o Zephyr SDK

Ainda na janela do `cmd.exe`.

1. Vá para o diretório raiz:

    ```bash
    cd workspace
    ```
2. Baixe o pacote [Zephyr SDK](https://github.com/zephyrproject-rtos/sdk-ng/releases) mais recente:

    ```bash
    wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.15.2/zephyr-sdk-0.15.2_windows-x86_64.zip
    ```

3. Exexute a extração dos dos arquivos:

    ```bash
    unzip zephyr-sdk-0.15.2_windows-x86_64.zip
    ```

4. Ao fim do passo **3** a seguinte estrutura de diretórios deve está montada:

    ```
    workspace/
    ├──zephyrproject/
    └──zephyr-sdk-0.15.2/
    ```

Caso a estrutura não esteja de acordo, garanta que ambas as pastas `zephyrproject` e `zephyr-sdk-0.15.2` estejam no mesmo espaço de trabalho.


# Build e Flash

O exemplo utilizado para ser *buildado* e em seguida *flashado* na placa de desenvolvimento [Nordic - nrf52832](https://docs.zephyrproject.org/2.7.0/boards/arm/nrf52dk_nrf52832/doc/index.html) se baseia em utilizar uma UART em conjunto de Threads para interagir com o usuário do programa. Mais detalhes podem ser encontrados na [documentação do projeto](https://github.com/Tayco110/Zephyr--Getting_started_on_Windows/blob/main/sample/README.md).

## Requisitos

| Ferramenta | Motivo |
| :------: | :------: |
| [nRF Command Line Tools](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download)| Garantir a comunicação entre o computador e a placa |
| [Tera Term](https://ttssh2.osdn.jp/index.html.en) | Garantir a interação entre o Usuário e o Programa |

Aqui cabem algumas observações. A ferramenta Tera Term é apenas uma sujestão, ficando a critério do desenvolvedor escolher qual ferramente utilizar. A instalação da `nRF Command Line Tools` se faz nescessário caso não exista nenhum drive de desenvolvimento da [Nordic](https://www.nordicsemi.com/) instalado em seu computador.

## Build

É o processo de "construção" do projeto que será gravado na placa. 

1. Clone o exemplo [sample](https://github.com/Tayco110/Zephyr--Getting_started_on_Windows/tree/main/sample) presente na raiz deste repositório.

2. Aloque a pasta do projeto dentro do diretório `zephyrproject` presente em sua pasta de trabalho, no caso deste turoarial: `workspace`. Ao fim da execução do ultimo passo, a seguinte estrutura de diretórios deve está montada:

    ```
    workspace/
        ├──zephyrproject/
        |   ├──sample/
        |   |   ├──src/
        |   |   |  └───main.c
        |   |   ├──CMakeLists.txt
        |   |   ├──prj.conf
        |   └───...
        └──zephyr-sdk-0.15.2/
    ```

3. Abra o `cmd.exe` e vá para o diretório do projeto

    ```bash
    cd ..\workspace\zephyrproject\sample
    ```
4. Execute o seguinte comando para *buildar* o projeto utilizando a ferramenta [west](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html#west-building)

    ```bash
    west build -b <BOARD>
    ```

Como citado anteriormente, estamos utilizando a placa de desenvolvimento [Nordic - nrf52832](https://docs.zephyrproject.org/2.7.0/boards/arm/nrf52dk_nrf52832/doc/index.html). Dessa forma, o comando em questão seria:

```bash
west build -b nrf52dk_nrf52832
```
Para identificar o comando adequado à sua placa é recomendado consultar a documentação oficial do Zephyr: [*Supported Boards*](https://docs.zephyrproject.org/latest/boards/index.html#boards).

* Caso todos os passos tenha sido seguidos corretamente até este ponto, deve aparecer em seu terminal após a execução do comando uma mensagem similar à mostrada abaixo:

    ```bash
    [162/162] Linking C executable zephyr\zephyr.elf
    Memory region         Used Size  Region Size  %age Used
               FLASH:       23496 B       512 KB      4.48%
                 RAM:        8388 B        64 KB     12.80%
            IDT_LIST:          0 GB         2 KB      0.00%
    ``` 

Essa mensagem indica que o processo de construção do projeto foi executado sem falhas.

* Caso contrário:

    ```bash
    required program nrfjprog not found; install it or add its location to PATH
    ```

Essa mensagem acusa a falta do [nRF Command Line Tools](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download) em seu computador.

## Flash

É o processo de "gravação" do projeto na placa.

1. Realize a conexão da placa de desenvolvimento em seu computador. Normalmente a conexão é realizada via USB.

2. Uma vez que o processo de *build* foi realizado, podemos agora utilizar o seguinte comando da ferramenta [west](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html#west-building) para gravar o programa:

    ```bash
    west flash
    ```
Para mais detalhes sobre o comando é recomendado a leitura da [documentação](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html#west-flashing).

* Caso o processo de *flash* tenha ocorrido sem erros, deve aparecer em seu terminal após a execução do comando uma mensagem similar a mostrada abaixo:

    ```bash
    -- runners.nrfjprog: Board with serial number 682720322 flashed successfully.
    ```

Essa mensagem indica que o processo de gravação do projeto foi executado sem falhas.

* Caso contrário:

    ```bash
    FATAL ERROR: "nrfjprog --ids" did not find a board; is the board connected?
    ```

Essa mensagem indica que a placa não foi encontrada. Verifique se a mesma se encontra conectada e alimentada.

# Resultado

![resultado](https://user-images.githubusercontent.com/44499744/206791017-33345823-e278-4d0e-8503-4f76db7390d9.gif)

<p align="center"> 
<b>Figura 1:</b> .Gif com o resultado.
</p>



