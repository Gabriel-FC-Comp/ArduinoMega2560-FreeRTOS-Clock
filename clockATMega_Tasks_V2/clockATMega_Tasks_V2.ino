///< Inclui as bibliotecas necessárias
#include "Arduino_FreeRTOS.h"  ///< Biblioteca do FreeRTOS para Arduino
#include "queue.h"             ///< Biblioteca para manipulação de filas do FreeRTOS

/* Definindo o Tamanho das Stacks das Tasks*/
#define STACK_SIZE 256  ///< Define o tamanho da pilha para cada task como 256 bytes

/* Definindo a Prioridade das Tasks*/
#define TIME_TASK_PRIORITY 10   ///< Define a prioridade da task de tempo como 10, alta prioridade
#define LCD_TASK_PRIORITY 7     ///< Define a prioridade da task de LCD como 7
#define MENU_TASK_PRIORITY 5    ///< Define a prioridade da task de menu como 5
#define ALARM_TASK_PRIORITY 6   ///< Define a prioridade da task de alarme como 6
#define BUZZER_TASK_PRIORITY 6  ///< Define a prioridade da task de buzzer como 6

const unsigned uxQueueLen = 2;  ///< Define o comprimento máximo das filas como 2

///< Configura os handlers e dados das filas
QueueHandle_t time_to_alarm_queue;  ///< Handler para a fila entre a task de tempo e alarme
QueueHandle_t time_to_lcd_queue;    ///< Handler para a fila entre a task de tempo e LCD
QueueHandle_t alarm_to_buzz_queue;  ///< Handler para a fila entre a task de alarme e buzzer
QueueHandle_t alarm_to_lcd_queue;   ///< Handler para a fila entre a task de alarme e LCD
QueueHandle_t menu_to_time_queue;   ///< Handler para a fila entre a task de menu e tempo
QueueHandle_t menu_to_alarm_queue;  ///< Handler para a fila entre a task de menu e alarme
QueueHandle_t menu_to_buzz_queue;   ///< Handler para a fila entre a task de menu e buzzer
QueueHandle_t menu_to_lcd_queue;    ///< Handler para a fila entre a task de menu e LCD

///< Configura os handlers das tasks
TaskHandle_t xTimeTaskHandle;    ///< Handler para a task de tempo
TaskHandle_t xAlarmTaskHandle;   ///< Handler para a task de alarme
TaskHandle_t xBuzzerTaskHandle;  ///< Handler para a task de buzzer
TaskHandle_t xLCDTaskHandle;     ///< Handler para a task de LCD
TaskHandle_t xMenuTaskHandle;    ///< Handler para a task de menu

///< Inclui as bibliotecas personalizadas
#include "dataStructs.h"       ///< Biblioteca customizada com definições de estruturas de dados
#include "timeTask.h"     ///< Biblioteca customizada com a implementação da task de tempo
#include "alarmTask.h"    ///< Biblioteca customizada com a implementação da task de alarme
#include "buzzerTask.h"   ///< Biblioteca customizada com a implementação da task de buzzer
#include "menuTask.h"     ///< Biblioteca customizada com a implementação da task de menu
#include "lcdTask.h"      ///< Biblioteca customizada com a implementação da task de LCD
#include "dataStructs.h"  ///< Biblioteca customizada com definições de estruturas de dados (já incluída anteriormente, pode ser redundante)

///< Configura os parâmetros que serão passados as tasks
TimeTaskParameters timeTaskParameters;      ///< Parâmetros para a task de tempo
AlarmTaskParameters alarmTaskParameters;    ///< Parâmetros para a task de alarme
BuzzerTaskParameters buzzerTaskParameters;  ///< Parâmetros para a task de buzzer
MenuTaskParameters menuTaskParameters;      ///< Parâmetros para a task de menu
LCDTaskParameters lcdTaskParameters;        ///< Parâmetros para a task de LCD

/// Função para inicializar as filas
void initializeQueues() {
  // Inicializa a fila time_to_alarm_queue
  time_to_alarm_queue = xQueueCreate(uxQueueLen, sizeof(TimeData));
  if (time_to_alarm_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar time_to_alarm_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                          ///< Entra em loop infinito em caso de erro
  }

  // Inicializa a fila time_to_lcd_queue
  time_to_lcd_queue = xQueueCreate(uxQueueLen, sizeof(TimeData));
  if (time_to_lcd_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar time_to_lcd_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                        ///< Entra em loop infinito em caso de erro
  }

  // Inicializa a fila alarm_to_buzz_queue
  alarm_to_buzz_queue = xQueueCreate(uxQueueLen, sizeof(bool));
  if (alarm_to_buzz_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar alarm_to_buzz_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                          ///< Entra em loop infinito em caso de erro
  }

  // Inicializa a fila alarm_to_lcd_queue
  alarm_to_lcd_queue = xQueueCreate(uxQueueLen, sizeof(TimeData));
  if (alarm_to_lcd_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar alarm_to_lcd_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                         ///< Entra em loop infinito em caso de erro
  }

  // Inicializa a fila menu_to_time_queue
  menu_to_time_queue = xQueueCreate(uxQueueLen, sizeof(TimeComms));
  if (menu_to_time_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar menu_to_time_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                         ///< Entra em loop infinito em caso de erro
  }

  // Serial.println((uintptr_t)menu_to_time_queue, HEX);  ///< (Comentado) Código para debug

  // Inicializa a fila menu_to_alarm_queue
  menu_to_alarm_queue = xQueueCreate(uxQueueLen, sizeof(TimeComms));
  if (menu_to_alarm_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar menu_to_alarm_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                          ///< Entra em loop infinito em caso de erro
  }

  // Inicializa a fila menu_to_buzz_queue
  menu_to_buzz_queue = xQueueCreate(uxQueueLen, sizeof(int));
  if (menu_to_buzz_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar menu_to_buzz_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                         ///< Entra em loop infinito em caso de erro
  }

  // Inicializa a fila menu_to_lcd_queue
  menu_to_lcd_queue = xQueueCreate(uxQueueLen, sizeof(LcdUpdateData));
  if (menu_to_lcd_queue == NULL) {                       ///< Verifica se a fila foi criada com sucesso
    Serial.println("Erro ao criar menu_to_lcd_queue!");  ///< Imprime mensagem de erro se a fila não foi criada
    while (true) { delay(1000); }                        ///< Entra em loop infinito em caso de erro
  }

  // Serial.println((uintptr_t)menu_to_lcd_queue, HEX);  ///< (Comentado) Código para debug
}

/// Função de setup do Arduino
void setup() {
  ///< Inicializa a Serial para debug
  Serial.begin(1000000);
  while (!Serial) {}  ///< Aguarda a inicialização da Serial

  ///< Configura o pino do buzzer
  pinMode(BUZZER_PIN, OUTPUT);                    ///< Configura o pino do buzzer como saída
  Serial.println("Pino do Buzzer Configurado!");  ///< Imprime mensagem de confirmação

  ///< Inicialização do joystick e botão
  pinMode(JOY_X_PIN, INPUT);  ///< Configura o pino do eixo X do joystick como entrada com pull-up
  pinMode(JOY_Y_PIN, INPUT);  ///< Configura o pino do eixo Y do joystick como entrada com pull-up
  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);  ///< Configura o pino do botão do joystick como entrada com pull-up

  ///< Inicializa as filas
  initializeQueues();
  Serial.println("Filas Inicializadas com Sucesso!");  ///< Imprime mensagem de confirmação

  ///< Configura os parâmetros a serem passados para as tasks
  timeTaskParameters = { time_to_alarm_queue, time_to_lcd_queue, menu_to_time_queue };                          ///< Atribui as filas aos parâmetros da task de tempo
  alarmTaskParameters = { time_to_alarm_queue, alarm_to_buzz_queue, menu_to_alarm_queue, alarm_to_lcd_queue };  ///< Atribui as filas aos parâmetros da task de alarme
  buzzerTaskParameters = { alarm_to_buzz_queue, menu_to_buzz_queue };                                           ///< Atribui as filas aos parâmetros da task de buzzer
  menuTaskParameters = { menu_to_alarm_queue, menu_to_time_queue, menu_to_buzz_queue, menu_to_lcd_queue };      ///< Atribui as filas aos parâmetros da task de menu
  lcdTaskParameters = { menu_to_lcd_queue, alarm_to_lcd_queue, time_to_lcd_queue };                             ///< Atribui as filas aos parâmetros da task de LCD
  Serial.println("Parâmetros das Tasks Criados");                                                               ///< Imprime mensagem de confirmação

  ///< Inicializa as tasks
  xTaskCreate(
    vTimeTaskCode,        ///< Função da task de tempo
    "TimeTask",           ///< Nome da task
    STACK_SIZE,           ///< Tamanho da pilha
    &timeTaskParameters,  ///< Parâmetros da task
    TIME_TASK_PRIORITY,   ///< Prioridade da task
    &xTimeTaskHandle      ///< Handler da task
  );
  Serial.println("TimeTask Escalonada!");  ///< Imprime mensagem de confirmação

  xTaskCreate(
    vAlarmTaskCode,        ///< Função da task de alarme
    "AlarmTask",           ///< Nome da task
    STACK_SIZE,            ///< Tamanho da pilha
    &alarmTaskParameters,  ///< Parâmetros da task
    ALARM_TASK_PRIORITY,   ///< Prioridade da task
    &xAlarmTaskHandle      ///< Handler da task
  );
  Serial.println("AlarmTask Escalonada!");  ///< Imprime mensagem de confirmação

  xTaskCreate(
    vBuzzerTaskCode,        ///< Função da task de buzzer
    "BuzzerTask",           ///< Nome da task
    STACK_SIZE,             ///< Tamanho da pilha
    &buzzerTaskParameters,  ///< Parâmetros da task
    BUZZER_TASK_PRIORITY,   ///< Prioridade da task
    &xBuzzerTaskHandle      ///< Handler da task
  );
  Serial.println("BuzzerTask Escalonada!");  ///< Imprime mensagem de confirmação

  xTaskCreate(
    vMenuTaskCode,        ///< Função da task de menu
    "MenuTask",           ///< Nome da task
    STACK_SIZE,           ///< Tamanho da pilha
    &menuTaskParameters,  ///< Parâmetros da task
    MENU_TASK_PRIORITY,   ///< Prioridade da task
    &xMenuTaskHandle      ///< Handler da task
  );
  Serial.println("MenuTask Escalonada!");  ///< Imprime mensagem de confirmação

  xTaskCreate(
    vLCDTaskCode,        ///< Função da task de LCD
    "LCDTask",           ///< Nome da task
    STACK_SIZE,          ///< Tamanho da pilha
    &lcdTaskParameters,  ///< Parâmetros da task
    LCD_TASK_PRIORITY,   ///< Prioridade da task
    &xLCDTaskHandle      ///< Handler da task
  );
  Serial.println("LCDTask Escalonada!");  ///< Imprime mensagem de confirmação
}

/// Loop principal do Arduino (vazio porque o FreeRTOS cuida das tasks)
void loop() {}