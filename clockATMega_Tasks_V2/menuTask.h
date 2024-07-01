#ifndef menuTask_H
#define menuTask_H

#include "Arduino.h"             ///< Inclui a biblioteca Arduino para funcionalidades básicas
#include "Arduino_FreeRTOS.h"    ///< Inclui a biblioteca Arduino FreeRTOS para uso de tarefas
#include "queue.h"               ///< Inclui a biblioteca Queue para gerenciamento de filas
#include "joystickConfig.h"      ///< Inclui a configuração do joystick
#include "communicationEnuns.h"  ///< Inclui definições de enumeração para comunicação
#include "dataStructs.h"         ///< Inclui definições de estruturas de dados

struct MenuTaskParameters {
  QueueHandle_t to_alarm_queue;   ///< Fila para dados do alarme
  QueueHandle_t to_time_queue;    ///< Fila para dados do tempo
  QueueHandle_t to_buzzer_queue;  ///< Fila para dados do buzzer
  QueueHandle_t to_lcd_queue;     ///< Fila para dados do display LCD
};

///< Estados para o relógio
enum MachineStates {
  Idle_Machine,  ///< Estado ocioso
  Select_Hour,   ///< Seleção da hora
  H_Hours,       ///< Horas
  H_Minutes,     ///< Minutos
  H_Seconds,     ///< Segundos
  Select_Alarm,  ///< Seleção do alarme
  A_Hours,       ///< Horas do alarme
  A_Minutes,     ///< Minutos do alarme
  Select_Tone,   ///< Seleção do tom
  Tone_List      ///< Lista de tons
};

class Menu {
public:
  Menu(int joystick_x_pin, int joystick_y_pin, int joystick_button_pin, QueueHandle_t to_alarm_queue,
       QueueHandle_t to_time_queue, QueueHandle_t to_buzzer_queue, QueueHandle_t to_lcd_queue);
  void updateInternalData();  ///< Atualiza dados internos
  void verifyUserInput();     ///< Verifica entrada do usuário
  void updateLCD();           ///< Atualiza o display LCD
private:
  MachineStates estadoInterno;  ///< Estado interno da máquina
  bool lastButtonState;         ///< Estado do último botão pressionado

  Joystick joystick;  ///< Objeto Joystick para leitura dos valores do joystick

  QueueHandle_t to_alarm_queue;     ///< Fila para dados do alarme
  QueueHandle_t from_alarm_queue;   ///< Fila de onde o alarme é lido
  QueueHandle_t to_time_queue;      ///< Fila para dados do tempo
  QueueHandle_t from_time_queue;    ///< Fila de onde o tempo é lido
  QueueHandle_t to_buzzer_queue;    ///< Fila para dados do buzzer
  QueueHandle_t from_buzzer_queue;  ///< Fila de onde o buzzer é lido
  QueueHandle_t to_lcd_queue;       ///< Fila para dados do display LCD

  TimeComms timeCommand;     ///< Comando de tempo
  LcdUpdateData newLcdData;  ///< Dados de atualização do LCD

  int alarmHour;       ///< Hora do alarme
  int alarmMinute;     ///< Minuto do alarme
  int hour;            ///< Hora
  int minute;          ///< Minuto
  int second;          ///< Segundo
  uint8_t buzzSelect;  ///< Seleção do buzzer

  void getAlarmData();                                                        ///< Obtém dados do alarme
  void getTimeData();                                                         ///< Obtém dados do tempo
  void updateBuzzSelect();                                                    ///< Atualiza seleção do buzzer
  void sendTimeCommand(QueueHandle_t queue);                                  ///< Envia comando de tempo para a fila especificada
  void updateState(JoystickState currJoyState, ButtonState currButtonState);  ///< Atualiza estado baseado nos valores do joystick e botão
  void updateJoyButtonState();                                                ///< Atualiza estado do botão do joystick
};

void vMenuTaskCode(void* pvParameters);  ///< Código da tarefa do menu

#endif