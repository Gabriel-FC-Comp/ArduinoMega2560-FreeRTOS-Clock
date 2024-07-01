#ifndef timeTask_H
#define timeTask_H

#include "Arduino.h"           ///< Inclui a biblioteca Arduino para funcionalidades básicas
#include "Arduino_FreeRTOS.h"  ///< Inclui a biblioteca Arduino FreeRTOS para uso de tarefas
#include "queue.h"             ///< Inclui a biblioteca Queue para gerenciamento de filas
#include "dataStructs.h"       ///< Inclui definições de estruturas de dados

struct TimeTaskParameters {
  QueueHandle_t to_alarm_queue;   ///< Fila para dados do alarme
  QueueHandle_t to_lcd_queue;     ///< Fila para dados do display LCD
  QueueHandle_t from_menu_queue;  ///< Fila de onde o menu lê dados
};

class Relogio {
public:
  Relogio(QueueHandle_t to_alarm_queue, QueueHandle_t to_lcd_queue, QueueHandle_t from_menu_queue);
  void incrementHour();    ///< Incrementa a hora
  void incrementMinute();  ///< Incrementa o minuto
  void incrementSecond();  ///< Incrementa o segundo
  void decrementHour();    ///< Decrementa a hora
  void decrementMinute();  ///< Decrementa o minuto
  void decrementSecond();  ///< Decrementa o segundo
  void getUserUpdates();   ///< Obtém atualizações do usuário
private:
  int second = 0;                 ///< Segundos atuais
  int hour = 0;                   ///< Hora atual
  int minute = 0;                 ///< Minuto atual
  TimeData currTime_toSend;       ///< Dados de tempo para enviar
  QueueHandle_t to_alarm_queue;   ///< Fila para dados do alarme
  QueueHandle_t to_lcd_queue;     ///< Fila para dados do display LCD
  QueueHandle_t from_menu_queue;  ///< Fila de onde o menu lê dados

  void adjustTime();  ///< Ajusta o tempo
};                    // class Relogio

void vTimeTaskCode(void* pvParameters);  ///< Código da tarefa de tempo

#endif