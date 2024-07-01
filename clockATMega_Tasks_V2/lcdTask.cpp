
#include "projdefs.h"  ///< Inclui definições de projeto necessárias
#include "lcdTask.h"

LCD::LCD(int rs_, int en_, int d4_, int d5_, int d6_, int d7_, QueueHandle_t from_menu_queue, QueueHandle_t from_alarm_queue, QueueHandle_t from_time_queue)
  : internalState(IDLE_LCD), lcd(rs_, en_, d4_, d5_, d6_, d7_), from_menu_queue(from_menu_queue),
    from_alarm_queue(from_alarm_queue), from_time_queue(from_time_queue), alarmHour(0), alarmMinute(0),
    currentHour(0), currentMinute(0), currentSecond(0),
    buzzSelect(0) {
  // Inicialização do display LCD
  lcd.begin(16, 2);  ///< Inicializa o display LCD com 16 colunas e 2 linhas
}

void LCD::printHour(int lineIndex) {

  lcd.setCursor(0, lineIndex);         ///< Posiciona o cursor no início linha
  if (internalState == SELECT_HOUR) {  ///< Verifica se o estado atual é a seleção da hora
    lcd.print("*");                    ///< Exibe um asterisco para indicar a seleção da hora
  }                                    //if

  lcd.print("Hora: ");
  if (currentHour < 10) {  ///< Verifica se a hora é menor que 10 para exibir um zero à esquerda
    lcd.print("0");
  }  //if

  lcd.print(currentHour);  ///< Exibe a hora
  lcd.print(":");
  if (currentMinute < 10) {  ///< Verifica se os minutos são menores que 10 para exibir um zero à esquerda
    lcd.print("0");
  }  //if

  lcd.print(currentMinute);  ///< Exibe os minutos
  lcd.print(":");
  if (currentSecond < 10) {  ///< Verifica se os segundos são menores que 10 para exibir um zero à esquerda
    lcd.print("0");
  }  // if

  lcd.print(currentSecond);                     ///< Exibe os segundos
  if (internalState == CHANGE_HOUR) {           ///< Verifica se o estado atual é a seleção das horas do alarme
    lcd.setCursor(15, lineIndex);               ///< Posiciona o cursor no final da primeira linha
    lcd.print("h");                             ///< Exibe "h" para indicar as horas do alarme
  } else if (internalState == CHANGE_MINUTE) {  ///< Verifica se o estado atual é a seleção dos minutos do alarme
    lcd.setCursor(15, lineIndex);               ///< Posiciona o cursor no final da primeira linha
    lcd.print("m");                             ///< Exibe "m" para indicar os minutos do alarme
  } else if (internalState == CHANGE_SECOND) {  ///< Verifica se o estado atual é a seleção dos segundos do alarme
    lcd.setCursor(15, lineIndex);               ///< Posiciona o cursor no final da primeira linha
    lcd.print("s");                             ///< Exibe "s" para indicar os segundos do alarme
  }                                             // if
}  // LCD::printHour

void LCD::printAlarm(int lineIndex) {

  lcd.setCursor(0, lineIndex);          ///< Posiciona o cursor no início linha
  if (internalState == SELECT_ALARM) {  ///< Verifica se o estado atual é a seleção do alarme
    lcd.print("*");                     ///< Exibe um asterisco para indicar a seleção do alarme
  }                                     // if

  lcd.print("Alarme: ");
  if (alarmHour < 10) {  ///< Verifica se a hora do alarme é menor que 10 para exibir um zero à esquerda
    lcd.print("0");
  }  // if

  lcd.print(alarmHour);  ///< Exibe a hora do alarme
  lcd.print(":");
  if (alarmMinute < 10) {  ///< Verifica se os minutos do alarme são menores que 10 para exibir um zero à esquerda
    lcd.print("0");
  }  // if

  lcd.print(alarmMinute);                         ///< Exibe os minutos do alarme
  if (internalState == CHANGE_A_HOUR) {           ///< Verifica se o estado atual é a seleção das horas do alarme
    lcd.setCursor(15, lineIndex);                 ///< Posiciona o cursor no final da segunda linha
    lcd.print("h");                               ///< Exibe "h" para indicar as horas do alarme
  } else if (internalState == CHANGE_A_MINUTE) {  ///< Verifica se o estado atual é a seleção dos minutos do alarme
    lcd.setCursor(15, lineIndex);                 ///< Posiciona o cursor no final da segunda linha
    lcd.print("m");                               ///< Exibe "m" para indicar os minutos do alarme
  }                                               // if
}  // LCD::printAlarm

void LCD::printS_Tone(int lineIndex) {
  lcd.setCursor(0, lineIndex);  ///< Posiciona o cursor no início linha
  if (internalState == SELECT_TONE) {
    lcd.print("*");
  }  //if

  lcd.print("Musicas");
}  // LCD::printS_Tone

void LCD::printT_List() {
  if (buzzSelect == 1) {  ///<
    lcd.print("*");       ///< Exibe um asterisco para indicar a seleção do tom
  }
  lcd.print("Mario");
  lcd.setCursor(0, 1);    ///< Posiciona o cursor no início da segunda linha
  if (buzzSelect == 0) {  ///<
    lcd.print("*");       ///< Exibe um asterisco para indicar a seleção do tom
  }
  lcd.print("Zelda");
}  // LCD::printT_List

void LCD::updateLCD() {
  lcd.clear();
  switch (internalState) {
    case SELECT_TONE:
      printS_Tone(0);
      printHour(1);
      break;

    case TONE_LIST:
      printT_List();
      break;

    case IDLE_LCD:
    case SELECT_HOUR:
    case CHANGE_HOUR:
    case CHANGE_MINUTE:
    case CHANGE_SECOND:
      printHour(0);
      printAlarm(1);
      break;

    case SELECT_ALARM:
    case CHANGE_A_HOUR:
    case CHANGE_A_MINUTE:
      printAlarm(0);
      printS_Tone(1);
      break;

    default:
      Serial.println("Warning - Estado do LCD corrompido!");
      internalState = IDLE_LCD;
      break;
  }  //switch
}  // LCD::updateLCD

void LCD::getTimeData() {
  TimeData currTime;

  if (xQueueReceive(from_time_queue, &currTime, 0) == pdTRUE) {
    ///< Verifica se recebeu os dados corretamente para substituir
    if (currTime.hour != -1) {
      currentHour = currTime.hour;
      currentMinute = currTime.minute;
      currentSecond = currTime.second;
    }  // if
  }    // if queue

}  // LCD::getTimeData

void LCD::getAlarmData() {
  TimeData currAlarm;

  if (xQueueReceive(from_alarm_queue, &currAlarm, 0) == pdTRUE) {
    ///< Verifica se recebeu os dados corretamente para substituir
    if (currAlarm.hour != -1) {
      alarmHour = currAlarm.hour;
      alarmMinute = currAlarm.minute;
    }  // if
  }    // if queue

}  // LCD::getAlarmData

void LCD::updateInternalData() {
  getTimeData();
  getAlarmData();

  // Tenta receber os dados
  if (xQueueReceive(from_menu_queue, &newData, 0) == pdTRUE) {
    // Se há novos dados, atualiza tudo
    internalState = newData.menuCommand;
    buzzSelect = newData.buzzSelect;

  }  // if

}  // LCD::updateInternalData

void vLCDTaskCode(void* pvParameters) {
  Serial.println("\nInicializando LCDTask...");
  LCDTaskParameters* lcdTaskParameters = reinterpret_cast<LCDTaskParameters*>(pvParameters);
  xQueueReset(lcdTaskParameters->from_menu_queue);
  // Serial.println((uintptr_t)from_menu_queue, HEX);
  LCD lcdInterno(RS, EN, D4, D5, D6, D7, lcdTaskParameters->from_menu_queue, lcdTaskParameters->from_alarm_queue, lcdTaskParameters->from_time_queue);
  Serial.println("LCDTask Inicializada!");
  while (true) {
    lcdInterno.updateInternalData();
    // Serial.println("LCDInternalData Atualizada");
    lcdInterno.updateLCD();
    // Serial.println("LCD Atualizado");
    vTaskDelay(pdMS_TO_TICKS(50));
  }  // while
}  // vLCDTaskCode
