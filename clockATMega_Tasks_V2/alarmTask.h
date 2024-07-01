#ifndef alarmTask_H  ///< Se não definido, define o identificador alarmTask_H
#define alarmTask_H  ///< Define o identificador alarmTask_H

#include "Arduino.h"           ///< Inclui a biblioteca Arduino
#include "Arduino_FreeRTOS.h"  ///< Inclui a biblioteca FreeRTOS para Arduino
#include "queue.h"             ///< Inclui a biblioteca de filas do FreeRTOS

/// Estrutura de parâmetros para a task de alarme
struct AlarmTaskParameters {
  QueueHandle_t from_time_queue;  ///< Fila de onde recebe dados de tempo
  QueueHandle_t to_buzzer_queue;  ///< Fila para onde envia comandos para o buzzer
  QueueHandle_t from_menu_queue;  ///< Fila de onde recebe comandos do menu
  QueueHandle_t to_lcd_queue;     ///< Fila para onde envia dados para o LCD
};

// Classe para o alarme
class Alarm {
public:
  /// Construtor da classe Alarm
  Alarm(QueueHandle_t from_time_queue, QueueHandle_t to_buzzer_queue, QueueHandle_t from_menu_queue, QueueHandle_t to_lcd_queue);

  void incrementHour();     ///< Incrementa a hora do alarme
  void incrementMinute();   ///< Incrementa o minuto do alarme
  void decrementHour();     ///< Decrementa a hora do alarme
  void decrementMinute();   ///< Decrementa o minuto do alarme
  void updateAlarmState();  ///< Atualiza o estado do alarme
  void updateLcdInfo();     ///< Atualiza as informações do LCD
  void getUserUpdates();    ///< Obtém atualizações do usuário

private:
  int hour;         ///< Hora do alarme
  int minute;       ///< Minuto do alarme
  bool alarmState;  ///< Variável para controlar a ativação do alarme

  QueueHandle_t from_time_queue;  ///< Fila de onde recebe dados de tempo
  QueueHandle_t to_buzzer_queue;  ///< Fila para onde envia comandos para o buzzer
  QueueHandle_t from_menu_queue;  ///< Fila de onde recebe comandos do menu
  QueueHandle_t to_lcd_queue;     ///< Fila para onde envia dados para o LCD

  void updateTime();                               ///< Atualiza o tempo do alarme
  bool isAlarmTime(int currHour, int currMinute);  ///< Verifica se é a hora do alarme tocar
  void activateBuzz();                             ///< Ativa o buzzer
  void deactivateBuzz();                           ///< Desativa o buzzer
};                                                 // class Alarm

/// Função da task de alarme
void vAlarmTaskCode(void* pvParameters);

#endif  ///< Fim do bloco de definição de alarmTask_H