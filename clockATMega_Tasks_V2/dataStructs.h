#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include "communicationEnuns.h"  ///< Inclui definições de enums para comunicação

/// Estrutura para representar os dados de tempo
struct TimeData {
  int hour;    ///< Hora atual
  int minute;  ///< Minuto atual
  int second;  ///< Segundo atual
};

/// Estrutura para representar os dados de atualização do LCD
struct LcdUpdateData {
  LCD_STATES menuCommand;  ///< Comando atual do menu para o LCD
  int buzzSelect;          ///< Seleção de melodia/buzzer
};

#endif  // DATASTRUCTS_H