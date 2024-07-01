#ifndef communicationEnuns_H
#define communicationEnuns_H

/// Enumeração para comandos de tempo
enum TimeComms {
  Idle_Comm,         ///< Comando ocioso, sem ação
  Increment_Hour,    ///< Comando para incrementar a hora
  Decrement_Hour,    ///< Comando para decrementar a hora
  Increment_Minute,  ///< Comando para incrementar o minuto
  Decrement_Minute,  ///< Comando para decrementar o minuto
  Increment_Second,  ///< Comando para incrementar o segundo
  Decrement_Second   ///< Comando para decrementar o segundo
};                   // TimeComms

/// Enumeração para os estados do LCD
enum LCD_STATES {
  IDLE_LCD,         ///< Estado ocioso do LCD, sem interação com o usuário
  SELECT_HOUR,      ///< Estado de seleção de hora no menu do usuário
  CHANGE_HOUR,      ///< Estado de alteração de hora pelo usuário
  CHANGE_MINUTE,    ///< Estado de alteração de minuto pelo usuário
  CHANGE_SECOND,    ///< Estado de alteração de segundo pelo usuário
  SELECT_ALARM,     ///< Estado de seleção de alarme no menu do usuário
  CHANGE_A_HOUR,    ///< Estado de alteração de hora do alarme pelo usuário
  CHANGE_A_MINUTE,  ///< Estado de alteração de minuto do alarme pelo usuário
  SELECT_TONE,      ///< Estado de seleção de tom no menu do usuário
  TONE_LIST         ///< Estado de exibição da lista de tons para seleção pelo usuário
};                  // LCD_STATES

#endif  ///< communicationEnuns_H