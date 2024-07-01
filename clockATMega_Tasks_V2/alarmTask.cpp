
#include "projdefs.h"            ///< Inclui definições de projeto necessárias
#include "alarmTask.h"           ///< Inclui a implementação da tarefa de alarme
#include "dataStructs.h"         ///< Inclui definições de estruturas de dados utilizadas
#include "communicationEnuns.h"  ///< Inclui definições de enumerações para comunicação

/// Construtor da classe Alarm
Alarm::Alarm(QueueHandle_t from_time_queue, QueueHandle_t to_buzzer_queue, QueueHandle_t from_menu_queue, QueueHandle_t to_lcd_queue)
  : hour(0), minute(0), alarmState(false),  ///< Inicializa hora, minuto e estado do alarme
    from_time_queue(from_time_queue),       ///< Inicializa fila de tempo
    to_buzzer_queue(to_buzzer_queue),       ///< Inicializa fila para buzzer
    from_menu_queue(from_menu_queue),       ///< Inicializa fila de menu
    to_lcd_queue(to_lcd_queue) {}           ///< Inicializa fila para LCD

/// Verifica se é hora do alarme tocar
bool Alarm::isAlarmTime(int currHour, int currMinute) {
  return ((hour == currHour) && (minute == currMinute));  ///< Retorna verdadeiro se a hora e o minuto atuais coincidem com os do alarme
}  // Alarm::isAlarmTime

/// Incrementa a hora do alarme
void Alarm::incrementHour() {
  hour++;        ///< Incrementa a hora
  updateTime();  ///< Atualiza o tempo após incrementar
}  // Alarm::incrementHour

/// Incrementa o minuto do alarme
void Alarm::incrementMinute() {
  minute++;      ///< Incrementa o minuto
  updateTime();  ///< Atualiza o tempo após incrementar
}  // Alarm::incrementMinute

/// Decrementa a hora do alarme
void Alarm::decrementHour() {
  hour--;        ///< Decrementa a hora
  updateTime();  ///< Atualiza o tempo após decrementar
}  // Alarm::decrementHour

/// Decrementa o minuto do alarme
void Alarm::decrementMinute() {
  minute--;      ///< Decrementa o minuto
  updateTime();  ///< Atualiza o tempo após decrementar
}  // Alarm::decrementMinute

/// Atualiza a hora e minuto do alarme conforme os limites
void Alarm::updateTime() {
  if (minute >= 60) {       ///< Se os minutos forem 60 ou mais
    minute = 0;             ///< Reseta minutos para 0
    hour++;                 ///< Incrementa a hora
  } else if (minute < 0) {  ///< Se os minutos forem negativos
    minute = 59;            ///< Define minutos para 59
  }                         // if minute

  if (hour >= 24) {       ///< Se as horas forem 24 ou mais
    hour = 0;             ///< Reseta horas para 0
  } else if (hour < 0) {  ///< Se as horas forem negativas
    hour = 23;            ///< Define horas para 23
  }                       // if hour

  updateLcdInfo();  ///< Atualiza as informações do LCD após ajustar a hora e minuto
}  // Alarm::updateTime

/// Ativa o buzzer
void Alarm::activateBuzz() {
  alarmState = true;                ///< Define o estado do alarme como ativo
  Serial.println("Activate Buzz");  ///< Imprime mensagem de ativação do buzzer
  xQueueSend(
    to_buzzer_queue,  ///< Envia para a fila do buzzer
    &alarmState,      ///< Dados a serem enviados (estado do alarme)
    0);               ///< Timeout de envio
}  // Alarm::activateBuzz

/// Desativa o buzzer
void Alarm::deactivateBuzz() {
  Serial.println("Deactivate Buzz");  ///< Imprime mensagem de desativação do buzzer
  alarmState = false;                 ///< Define o estado do alarme como inativo
  xQueueSend(
    to_buzzer_queue,  ///< Envia para a fila do buzzer
    &alarmState,      ///< Dados a serem enviados (estado do alarme)
    0);               ///< Timeout de envio
}  // Alarm::deactivateBuzz

/// Atualiza o estado do alarme baseado no tempo atual
void Alarm::updateAlarmState() {
  TimeData currTime;  ///< Estrutura para armazenar o tempo atual

  if (xQueueReceive(from_time_queue, &currTime, 0) == pdTRUE) {  ///< Recebe o tempo atual da fila
    // Serial.print("Alarm HourUpdate: ");
    // Serial.println(currTime.hour);
    // Serial.print("Alarm MinuteUpdate: ");
    // Serial.println(currTime.minute);
    if (isAlarmTime(currTime.hour, currTime.minute)) {  ///< Verifica se é hora do alarme tocar
      if (!alarmState) {                                ///< Se o alarme não está ativo
        activateBuzz();                                 ///< Ativa o buzzer
      }                                                 // if
    } else if (alarmState) {                            ///< Se o alarme está ativo mas não é a hora do alarme
      deactivateBuzz();                                 ///< Desativa o buzzer
    }                                                   // if
  }                                                     // if

}  // Alarm::updateAlarmState

/// Atualiza as informações no LCD
void Alarm::updateLcdInfo() {
  ///< Manda os dados do tempo atual para a fila do alarme e do menu
  TimeData currAlarm_toSend = { hour, minute, -1 };  ///< Estrutura com hora, minuto e um campo não utilizado (-1)

  xQueueSend(
    to_lcd_queue,       ///< Envia para a fila do LCD
    &currAlarm_toSend,  ///< Dados a serem enviados (tempo do alarme)
    0);                 ///< Timeout de envio

}  // Alarm::updateLcdInfo

/// Obtém atualizações do usuário via menu
void Alarm::getUserUpdates() {
  TimeComms menuCommand = Idle_Comm;  ///< Inicializa comando do menu como Idle (inativo)

  if (xQueueReceive(
        from_menu_queue,      ///< Recebe da fila do menu
        &menuCommand,         ///< Armazena o comando recebido
        0)                    ///< Timeout de recepção
      == pdFALSE) {           ///< Se não recebeu nenhum comando
    menuCommand = Idle_Comm;  ///< Define comando como Idle
  };

  switch (menuCommand) {  ///< Verifica o comando recebido

    case Increment_Hour:  ///< Se o comando for incrementar hora
      incrementHour();    ///< Incrementa a hora
      break;
    case Decrement_Hour:  ///< Se o comando for decrementar hora
      decrementHour();    ///< Decrementa a hora
      break;
    case Increment_Minute:  ///< Se o comando for incrementar minuto
      incrementMinute();    ///< Incrementa o minuto
      break;
    case Decrement_Minute:  ///< Se o comando for decrementar minuto
      decrementMinute();    ///< Decrementa o minuto
      break;
    case Idle_Comm:  ///< Se o comando for inativo
      break;
    default:                                                                  ///< Se o comando for desconhecido
      Serial.println("Warning - Comando para alterar o Alarme Corrompido!");  ///< Imprime aviso de comando corrompido
      break;
  }  //switch
}  // Alarm::getUserUpdates

/// Função da task de alarme
void vAlarmTaskCode(void* pvParameters) {

  Serial.println("\nInicializando AlarmTask...");                                                                                        ///< Imprime mensagem de inicialização da task de alarme
  AlarmTaskParameters* parameters = reinterpret_cast<AlarmTaskParameters*>(pvParameters);                                                ///< Interpreta os parâmetros passados para a task
  Alarm alarmeInterno(parameters->from_time_queue, parameters->to_buzzer_queue, parameters->from_menu_queue, parameters->to_lcd_queue);  ///< Cria um objeto de alarme com os parâmetros recebidos
  uint8_t update_menu_info_tag = 0;                                                                                                      ///< Variável para controlar atualização do menu
  Serial.println("AlarmTask Inicializada!");                                                                                             ///< Imprime mensagem de task inicializada
  /* Loop Eterno */
  while (true) {
    alarmeInterno.updateAlarmState();  ///< Atualiza o estado do alarme
    // Serial.println("AlarmInternalState Atualizado");
    if (!update_menu_info_tag) {      ///< Se não for hora de atualizar o menu
      alarmeInterno.updateLcdInfo();  ///< Atualiza as informações do LCD
      // Serial.println("AlarmGetMenuInfo Atualizada");
    }
    update_menu_info_tag++;          ///< Incrementa o contador de atualização do menu
    alarmeInterno.getUserUpdates();  ///< Obtém atualizações do usuário
    // Serial.println("AlarmUserUpdate Realizado");
    vTaskDelay(pdMS_TO_TICKS(750));  ///< Aguarda 750ms antes da próxima iteração do loop
  }                                  // while
}  // vAlarmTaskCode
