#ifndef buzzerTask_H  ///< Se não definido, define o identificador buzzerTask_H
#define buzzerTask_H  ///< Define o identificador buzzerTask_H

#include "Arduino.h"           ///< Inclui a biblioteca Arduino
#include "Arduino_FreeRTOS.h"  ///< Inclui a biblioteca FreeRTOS para Arduino
#include "queue.h"             ///< Inclui a biblioteca de filas do FreeRTOS

///< Define o pino de controle do buzzer
#define BUZZER_PIN 8

/// Estrutura de parâmetros para a task do buzzer
struct BuzzerTaskParameters {
  QueueHandle_t from_alarm_queue;  ///< Fila de onde recebe comandos do alarme
  QueueHandle_t from_menu_queue;   ///< Fila de onde recebe comandos do menu
};

// Classe para o buzzer
class Buzzer {
private:
  int buzzIndex;           ///< Índice para percorrer as melodias/durações
  uint8_t buzzSelect;      ///< Seletor da melodia a ser tocada no alarme
  int tom;                 ///< Tom a ser tocado no buzzer
  int duracao;             ///< Duração da nota a ser tocada no buzzer
  bool nextBuzz;           ///< Indica se deve tocar a próxima nota ou esperar passar o tempo
  bool buzzState;          ///< Estado atual do buzzer (ativo ou inativo)
  unsigned long lastBuzz;  ///< Último instante que o buzzer tocou

  QueueHandle_t from_alarm_queue;  ///< Fila de onde recebe comandos do alarme
  QueueHandle_t to_menu_queue;     ///< Fila para onde envia comandos para o menu
  QueueHandle_t from_menu_queue;   ///< Fila de onde recebe comandos do menu

  void updateBuzzIndex();  ///< Atualiza o índice do buzzer
  void updateBuzzData();   ///< Atualiza os dados do buzzer

public:
  /// Construtor da classe Buzzer
  Buzzer(QueueHandle_t from_alarm_queue, QueueHandle_t from_menu_queue);

  void updateBuzzState();   ///< Atualiza o estado do buzzer
  void updateBuzzSelect();  ///< Atualiza a seleção do buzzer
  void buzz();              ///< Executa o som do buzzer
};                          // class Buzzer

/// Função da task do buzzer
void vBuzzerTaskCode(void* pvParameters);

#endif  ///< Fim do bloco de definição de buzzerTask_H