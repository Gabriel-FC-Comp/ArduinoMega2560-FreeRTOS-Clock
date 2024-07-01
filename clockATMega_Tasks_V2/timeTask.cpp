
#include "projdefs.h"  ///< Inclui definições de projeto necessárias
#include "timeTask.h"
#include "communicationEnuns.h"

Relogio::Relogio(QueueHandle_t to_alarm_queue, QueueHandle_t to_lcd_queue, QueueHandle_t from_menu_queue)
  : hour(0), minute(0), second(0),
    to_alarm_queue(to_alarm_queue), to_lcd_queue(to_lcd_queue), from_menu_queue(from_menu_queue) {}

void Relogio::incrementHour() {
  hour++;        ///< Incrementa a hora
  adjustTime();  ///< Chama a função para ajustar o tempo
}  // Relogio::incrementHour

void Relogio::incrementMinute() {
  minute++;      ///< Incrementa o minuto
  adjustTime();  ///< Chama a função para ajustar o tempo
}  // Relogio::incrementMinute

void Relogio::incrementSecond() {
  second++;      ///< Incrementa o segundo
  adjustTime();  ///< Chama a função para ajustar o tempo
}  // Relogio::incrementSecond

void Relogio::decrementHour() {
  hour--;        ///< Decrementa a hora
  adjustTime();  ///< Chama a função para ajustar o tempo
}  // Relogio::decrementHour

void Relogio::decrementMinute() {
  minute--;      ///< Decrementa o minuto
  adjustTime();  ///< Chama a função para ajustar o tempo
}  // Relogio::decrementMinute

void Relogio::decrementSecond() {
  second--;      ///< Decrementa o segundo
  adjustTime();  ///< Chama a função para ajustar o tempo
}  // Relogio::decrementSecond

void Relogio::adjustTime() {
  ///< Verifica a atualização do tempo
  if (second >= 60) {       ///< Verifica se os segundos ultrapassaram 60
    second = 0;             ///< Reseta os segundos para zero
    minute++;               ///< Incrementa os minutos
  } else if (second < 0) {  ///< Verifica se os segundos são menores que zero
    second = 59;            ///< Define os segundos para 59 (último segundo)
  }                         // if second

  if (minute >= 60) {       ///< Verifica se os minutos ultrapassaram 60
    minute = 0;             ///< Reseta os minutos para zero
    hour++;                 ///< Incrementa as horas
  } else if (minute < 0) {  ///< Verifica se os minutos são menores que zero
    minute = 59;            ///< Define os minutos para 59 (último minuto)
  }                         // if minute

  if (hour >= 24) {       ///< Verifica se as horas ultrapassaram 24
    hour = 0;             ///< Reseta as horas para zero (novo dia)
  } else if (hour < 0) {  ///< Verifica se as horas são menores que zero
    hour = 23;            ///< Define as horas para 23 (última hora do dia)
  }                       // if hour

  ///< Manda os dados do tempo atual para a fila do alarme e do lcd
  currTime_toSend = { hour, minute, second };       ///< Atualiza os dados do tempo a serem enviados
  xQueueSend(to_alarm_queue, &currTime_toSend, 0);  ///< Envia dados do tempo para a fila do alarme
  xQueueSend(to_lcd_queue, &currTime_toSend, 0);    ///< Envia dados do tempo para a fila do display LCD
}  // Relogio::adjustTime

void Relogio::getUserUpdates() {
  TimeComms menuCommand = Idle_Comm;  ///< Inicializa o comando de menu como ocioso

  if (xQueueReceive(from_menu_queue, &menuCommand, 0) == pdTRUE) {  ///< Verifica se há novos comandos do menu
    switch (menuCommand) {
      case Increment_Hour:
        incrementHour();  ///< Incrementa a hora
        break;
      case Decrement_Hour:
        decrementHour();  ///< Decrementa a hora
        break;
      case Increment_Minute:
        incrementMinute();  ///< Incrementa o minuto
        break;
      case Decrement_Minute:
        decrementMinute();  ///< Decrementa o minuto
        break;
      case Increment_Second:
        incrementSecond();  ///< Incrementa o segundo
        break;
      case Decrement_Second:
        decrementSecond();  ///< Decrementa o segundo
        break;
      case Idle_Comm:  ///< Caso ocioso, não faz nada
        break;
      default:
        Serial.println("Warning - Comando para alterar o Tempo Corrompido!");  ///< Aviso de comando inválido
        break;
    }  // switch
  }    // if
}  // Menu::getUserUpdates

void vTimeTaskCode(void* pvParameters) {

  Serial.println("\nInicializando TimeTask...");
  TimeTaskParameters* timeTaskParameters = reinterpret_cast<TimeTaskParameters*>(pvParameters);
  Relogio relogioInterno(timeTaskParameters->to_alarm_queue, timeTaskParameters->to_lcd_queue, timeTaskParameters->from_menu_queue);
  TickType_t xLastWakeTime;                           ///< Último instante que a task foi executada
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);  ///< Frequência de execução da task em 1 segundo
  xLastWakeTime = xTaskGetTickCount();                ///< Inicializa a referência temporal da última execução
  Serial.println("TimeTask Inicializada!");

  /* Loop Eterno */
  while (true) {
    /* Espera o novo ciclo, atualizando o xLastWakeTime */
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
    /* Incrementa os segundos atuais */
    relogioInterno.incrementSecond();
    /* Verifica atualizações do usuário */
    relogioInterno.getUserUpdates();
  }  // while
}  // vTimeTaskCode
