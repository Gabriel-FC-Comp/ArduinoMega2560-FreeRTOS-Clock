# Sistema de Alarme com Arduino e FreeRTOS
Este projeto apresenta o desenvolvimento de um sistema de alarme utilizando a plataforma Arduino, integrando a biblioteca FreeRTOS para gestão de tarefas em tempo real. O objetivo principal foi migrar um sistema de alarme pré-existente, baseado em programação sequencial, para um modelo mais eficiente e modular baseado em RTOS (Real-Time Operating System).

## Introdução
Neste projeto, exploramos a implementação de um sistema de alarme utilizando Arduino e FreeRTOS, um kernel de tempo real amplamente utilizado em sistemas embarcados. A transição para FreeRTOS permitiu a execução concorrente de múltiplas tarefas, melhorando a responsividade e eficiência do sistema.

## Funcionalidades Implementadas
### Estrutura do Sistema
O sistema foi estruturado em torno de cinco tarefas principais, gerenciadas por filas unidirecionais para comunicação entre elas. A estrutura incluiu tarefas para o menu, relógio, alarme, buzzer e LCD, proporcionando um controle integrado e modular das funcionalidades. A relação entre as mesmas é melhor representada no diagrama abaixo:

[Diagrama Relacional entre as Tarefas](Figuras/LógicaRelógio.png)

O relógio foi desenvolvido como uma máquina de estados, representada no diagrama de transição de estados a seguir:

[Diagrama de Transição de Estados](Figuras/DiagramaEstados.png)

### Configuração do Circuito
O circuito foi projetado para integrar todos os componentes eletrônicos necessários, garantindo a correta interação com a placa Arduino Mega 2560. Foram utilizados pinos específicos para cada componente, assegurando a funcionalidade adequada conforme definido no código.

### Arquivo Principal e Configuração Inicial
O arquivo principal do projeto na Arduino IDE inclui a importação das bibliotecas necessárias para o FreeRTOS e a definição de parâmetros essenciais, como tamanhos de pilha, prioridades de tarefa e handlers de filas. A inicialização das filas e criação das tarefas são realizadas no setup inicial, garantindo que o sistema esteja pronto para operar.

### Configuração do Joystick
Para a interação do usuário, foi utilizado o módulo de joystick analógico KY-023. Foram implementadas lógicas para lidar com o debounce do botão e a sensibilidade do joystick, garantindo uma interação suave e sem ruídos.

### Definições de Enumerações para Comunicação
Enumerações foram utilizadas para definir estados claros e comandos dentro do sistema, facilitando o desenvolvimento e manutenção do código. Essas enumerações foram organizadas na biblioteca communicationEnums.h, proporcionando uma estrutura clara para as diferentes etapas de interação do usuário.

### Configuração das Estruturas de Comunicação Entre Tarefas
Para uma gestão eficiente dos dados temporais e atualizações do LCD, foram definidas estruturas específicas (TimeData e LcdUpdateData) que permitiram a troca efetiva de informações entre as tarefas do sistema.

### Configuração das Melodias para o Buzzer
As melodias para o buzzer foram configuradas utilizando frequências de notas musicais pré-definidas, organizadas na biblioteca melodies.h. Isso permitiu uma modularização clara das melodias e facilitou a integração com a lógica principal do sistema.

### Lógica das Tarefas Implementadas
Cada tarefa do sistema (menu, relógio, alarme, buzzer e LCD) foi desenvolvida como uma classe separada, encapsulando a lógica específica e facilitando a manutenção do código. A interação entre as tarefas foi gerenciada por meio das filas, garantindo uma comunicação ordenada e eficiente.

## Resultados e Testes
O circuito foi montado conforme ilustrado no relatório, e todos os componentes foram conectados corretamente. Os testes realizados demonstraram o funcionamento adequado das funcionalidades, incluindo o incremento correto dos valores do timer e a interação precisa do joystick com o sistema.

O circuito após montado pode ser observado na Figura abaixo, verificando a correta exibição das funcionalidaes no display LCD.

[Visão Geral do Circuito](Figuras/VisãoGeralCircuito.jpg)

O menu de seleção funcionou corretamente, indicando a opção a ser selecionada com o * e realizando a rolagem das opções, bem como a lista de melodias disponíveis pode ser trocada normalmente.

[Exemplo Menu de Seleção Geral](Figuras/SeleçãoMenu.png)

[Exemplo Menu de Seleção de Melodias](Figuras/ListaMelodias.png)

## Conclusão
O uso do FreeRTOS neste projeto proporcionou várias vantagens significativas, como a execução concorrente de tarefas, modularidade no desenvolvimento e sincronização eficiente entre processos. Os desafios enfrentados e as soluções implementadas foram detalhados, destacando a robustez e flexibilidade do sistema desenvolvido.

## Autores
[Gabriel Finger Conte](https://github.com/Gabriel-FC-Comp)
[Maria Eduarda Pedroso](https://github.com/mariaeduardapedroso)

## Licença

Este projeto é licenciado sob a [Licença MIT](https://opensource.org/licenses/MIT) - veja o arquivo [LICENSE](./LICENSE) para mais detalhes.
