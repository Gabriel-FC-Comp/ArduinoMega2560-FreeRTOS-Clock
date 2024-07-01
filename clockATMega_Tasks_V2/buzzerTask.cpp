#include "projdefs.h"    ///< Inclui definições de projeto necessárias
#include "buzzerTask.h"  ///< Inclui o cabeçalho da task do buzzer
#include "melodies.h"    ///< Inclui o cabeçalho com as melodias

/// Construtor da classe Buzzer
Buzzer::Buzzer(QueueHandle_t from_alarm_queue, QueueHandle_t from_menu_queue)
  : buzzIndex(0),                        ///< Inicializa o índice da melodia
    nextBuzz(true),                      ///< Inicializa o estado para tocar a próxima nota
    buzzSelect(0),                       ///< Inicializa o seletor da melodia
    lastBuzz(millis()),                  ///< Inicializa o último tempo em que o buzzer tocou
    buzzState(false),                    ///< Inicializa o estado do buzzer como inativo
    from_alarm_queue(from_alarm_queue),  ///< Inicializa a fila de comandos do alarme
    from_menu_queue(from_menu_queue) {   ///< Inicializa a fila de comandos do menu
  updateBuzzData();                      ///< Atualiza os dados do buzzer com a melodia inicial
}  // Buzzer

/// Atualiza o estado do buzzer
void Buzzer::updateBuzzState() {
  bool newBuzzState = false;  ///< Variável para o novo estado do buzzer
  if (xQueueReceive(from_alarm_queue, &newBuzzState, 0) == pdFALSE) {
    newBuzzState = buzzState;  ///< Mantém o estado atual se não houver novo comando
  }                            // if

  buzzState = newBuzzState;  ///< Atualiza o estado do buzzer
}  // updateBuzzState

/// Atualiza o seletor da melodia do buzzer
void Buzzer::updateBuzzSelect() {
  uint8_t newBuzzSelect = 0;  ///< Variável para o novo seletor da melodia
  if (xQueueReceive(from_menu_queue, &newBuzzSelect, 0) == pdTRUE) {
    buzzIndex = 0;  ///< Reinicia o índice da melodia se houver novo comando
  } else {
    newBuzzSelect = buzzSelect;  ///< Mantém o seletor atual se não houver novo comando
  }                              // if

  buzzSelect = newBuzzSelect;  ///< Atualiza o seletor da melodia
}  // updateBuzzSelect

/// Atualiza o índice da melodia do buzzer
void Buzzer::updateBuzzIndex() {
  buzzIndex++;  ///< Incrementa o índice da melodia

  switch (buzzSelect) {
    case 0:
      if (buzzIndex >= maxIndex_zelda) {
        buzzIndex = 0;  ///< Reinicia o índice se exceder o máximo para a melodia Zelda
      }                 // if
      break;
    case 1:
      if (buzzIndex >= maxIndex_mario) {
        buzzIndex = 0;  ///< Reinicia o índice se exceder o máximo para a melodia Mario
      }                 // if
      break;
    default:
      if (buzzIndex >= maxIndex_zelda) {
        buzzIndex = 0;  ///< Reinicia o índice se exceder o máximo para a melodia Zelda por padrão
      }                 // if
      break;
  }  // switch

  updateBuzzData();  ///< Atualiza os dados da melodia com o novo índice
}  // updateBuzzIndex

/// Atualiza os dados da melodia do buzzer
void Buzzer::updateBuzzData() {
  switch (buzzSelect) {
    case 0:
      tom = melodia_zelda[buzzIndex];             ///< Define o tom para a melodia Zelda
      duracao = duracoes_zelda[buzzIndex] * 200;  ///< Define a duração para a melodia Zelda
      break;
    case 1:
      tom = melodia_mario[buzzIndex];             ///< Define o tom para a melodia Mario
      duracao = duracoes_mario[buzzIndex] * 200;  ///< Define a duração para a melodia Mario
      break;
    default:
      tom = melodia_zelda[buzzIndex];             ///< Define o tom para a melodia Zelda por padrão
      duracao = duracoes_zelda[buzzIndex] * 200;  ///< Define a duração para a melodia Zelda por padrão
      break;
  }  // switch
}  // updateBuzzData

/// Toca a melodia no buzzer
void Buzzer::buzz() {
  if (!buzzState) {      ///< Verifica se o buzzer deve estar ativo
    noTone(BUZZER_PIN);  ///< Para o som se o buzzer não estiver ativo
    return;
  }

  unsigned long tempoAtual = millis();  ///< Obtém o tempo atual

  if (nextBuzz) {      ///< Verifica se deve tocar a próxima nota
    nextBuzz = false;  ///< Reseta o estado para tocar a próxima nota
    if (tom == 0) {
      noTone(BUZZER_PIN);  ///< Para o som se o tom for zero
    } else {
      tone(BUZZER_PIN, tom, duracao * 10);  ///< Toca a nota no buzzer
    }
  }  // if

  if (tempoAtual - lastBuzz >= duracao + 50) {  ///< Verifica se a duração da nota terminou
    updateBuzzIndex();                          ///< Atualiza o índice da melodia para a próxima nota
    lastBuzz = tempoAtual;                      ///< Atualiza o tempo da última nota tocada
    nextBuzz = true;                            ///< Define o estado para tocar a próxima nota
  }                                             // if
}  // buzz

/// Função da task do buzzer
void vBuzzerTaskCode(void* pvParameters) {
  Serial.println("\nInicializando BuzzTask...");
  BuzzerTaskParameters* buzzerTaskParameters = reinterpret_cast<BuzzerTaskParameters*>(pvParameters);   ///< Converte os parâmetros da task
  Buzzer buzzerInterno(buzzerTaskParameters->from_alarm_queue, buzzerTaskParameters->from_menu_queue);  ///< Cria uma instância da classe Buzzer
  Serial.println("BuzzerTask Inicializada!");

  while (true) {
    buzzerInterno.updateBuzzState();  ///< Atualiza o estado do buzzer
    // Serial.println("BuzzState Atualizado");
    buzzerInterno.buzz();  ///< Toca a melodia no buzzer
    // Serial.println("Buzz Acionado");
    buzzerInterno.updateBuzzSelect();  ///< Atualiza o seletor da melodia do buzzer
    // Serial.println("BuzzSelect Atualizado");
    vTaskDelay(pdMS_TO_TICKS(100));  ///< Atraso na execução da task para liberar CPU
  }                                  // while
}  // vBuzzerTaskCode