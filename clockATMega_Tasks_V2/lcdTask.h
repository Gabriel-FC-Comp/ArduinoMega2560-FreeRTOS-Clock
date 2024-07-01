#ifndef lcdConfig_H
#define lcdConfig_H

#include "Arduino.h"             ///< Inclui a biblioteca Arduino para funcionalidades básicas
#include "Arduino_FreeRTOS.h"    ///< Inclui a biblioteca Arduino FreeRTOS para uso de tarefas
#include "queue.h"               ///< Inclui a biblioteca Queue para gerenciamento de filas
#include "communicationEnuns.h"  ///< Inclui definições de enumeração para comunicação
#include "dataStructs.h"         ///< Inclui definições de estruturas de dados

#include <LiquidCrystal.h>  ///< Inclui a biblioteca LiquidCrystal para controle do display LCD

// Definição dos pinos para o display LCD
#define RS 12  ///< Pino RS do LCD
#define EN 11  ///< Pino EN do LCD
#define D4 5   ///< Pino D4 do LCD
#define D5 4   ///< Pino D5 do LCD
#define D6 3   ///< Pino D6 do LCD
#define D7 2   ///< Pino D7 do LCD

struct LCDTaskParameters {
  QueueHandle_t from_menu_queue;   ///< Fila para dados do menu
  QueueHandle_t from_alarm_queue;  ///< Fila para dados de alarme
  QueueHandle_t from_time_queue;   ///< Fila para dados de tempo
};

class LCD {
public:
  LCD(int rs_, int en_, int d4_, int d5_, int d6_, int d7_, QueueHandle_t from_menu_queue, QueueHandle_t from_alarm_queue, QueueHandle_t from_time_queue);
  void updateLCD();           ///< Atualiza o conteúdo do display LCD
  void updateInternalData();  ///< Atualiza dados internos do LCD

private:
  int alarmHour;      ///< Hora do alarme
  int alarmMinute;    ///< Minuto do alarme
  int currentHour;    ///< Hora atual
  int currentMinute;  ///< Minuto atual
  int currentSecond;  ///< Segundo atual
  int buzzSelect;     ///< Seleção do buzzer

  LCD_STATES internalState;  ///< Estado interno do LCD
  LiquidCrystal lcd;         ///< Objeto LiquidCrystal para controle do LCD
  LcdUpdateData newData;     ///< Dados de atualização do LCD

  QueueHandle_t from_menu_queue;   ///< Fila para dados do menu
  QueueHandle_t from_alarm_queue;  ///< Fila para dados de alarme
  QueueHandle_t from_time_queue;   ///< Fila para dados de tempo

  void getTimeData();   ///< Obtém dados de tempo da fila
  void getAlarmData();  ///< Obtém dados de alarme da fila

  void printHour(int lineIndex);    ///< Exibe a hora no display LCD
  void printAlarm(int lineIndex);   ///< Exibe o alarme no display LCD
  void printS_Tone(int lineIndex);  ///< Exibe o tom de música no display LCD
  void printT_List();               ///< Exibe a lista de tons no display LCD
};                                  // class

void vLCDTaskCode(void* pvParameters);  ///< Código da tarefa LCD

#endif