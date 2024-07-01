
#include "projdefs.h"
#include "menuTask.h"
#include "melodies.h"

Menu::Menu(int joystick_x_pin, int joystick_y_pin, int joystick_button_pin, QueueHandle_t to_alarm_queue,
           QueueHandle_t to_time_queue, QueueHandle_t to_buzzer_queue, QueueHandle_t to_lcd_queue)
  : to_alarm_queue(to_alarm_queue), to_time_queue(to_time_queue), to_buzzer_queue(to_buzzer_queue), to_lcd_queue(to_lcd_queue),
    estadoInterno(Idle_Machine), lastButtonState(1), hour(0), minute(0), second(0), alarmHour(0), alarmMinute(0),
    buzzSelect(0), joystick(joystick_x_pin, joystick_y_pin, joystick_button_pin) {}

void Menu::updateJoyButtonState() {
  joystick.updateButtonState();  ///< Atualiza o estado do botão do joystick
}  // Menu::updateJoyButtonState

void Menu::verifyUserInput() {
  updateJoyButtonState();                                    ///< Atualiza o estado do botão do joystick
  JoystickState currJoyState = joystick.getJoystickState();  ///< Obtém o estado atual do joystick
  ButtonState currButtonState = joystick.getButtonState();   ///< Obtém o estado atual do botão do joystick
  updateState(currJoyState, currButtonState);                ///< Atualiza o estado interno com base na entrada do usuário
  lastButtonState = joystick.getButtonState();               ///< Atualiza o estado anterior do botão do joystick
}  // Menu::verifyUserInput

void Menu::updateBuzzSelect() {
  xQueueSend(
    to_buzzer_queue,
    &buzzSelect,
    0);  ///< Envia o índice do som selecionado para a fila de seleção de buzzer
}  // Menu::updateBuzzSelect

void Menu::sendTimeCommand(QueueHandle_t queue) {
  xQueueSend(
    queue,
    &timeCommand,
    0);  ///< Envia um comando de tempo para a fila especificada
}  // Menu::sendTimeCommand

void Menu::updateState(JoystickState currJoyState, ButtonState currButtonState) {

  switch (estadoInterno) {
    case Idle_Machine:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Select_Hour;
      }  // if currButtonState
      break;

    case Select_Hour:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = H_Hours;
      }  // if currButtonState

      if (currJoyState == Up) {
        estadoInterno = Select_Tone;
      } else if (currJoyState == Down) {
        estadoInterno = Select_Alarm;
      }  // if currJoyState

      break;

    case H_Hours:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Idle_Machine;
      }  // if currButtonState

      switch (currJoyState) {
        case Up:
          // Incrementa o tempo local
          hour++;
          // Seta o comando para incrementar a hora atual
          timeCommand = Increment_Hour;
          // Envia o comando para a fila
          sendTimeCommand(to_time_queue);
          break;
        case Down:
          // Decrementa o tempo local
          hour--;
          // Seta o comando para decrementar a hora atual
          timeCommand = Decrement_Hour;
          // Envia o comando para a fila
          sendTimeCommand(to_time_queue);
          break;
        case Left:
          break;
        case Right:
          estadoInterno = H_Minutes;
          break;
        case Neutral:
          break;
        default:
          Serial.println("Warning - Erro na leitura do Joystick");
      }  // switch

      break;

    case H_Minutes:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Idle_Machine;
      }  // if currButtonState

      switch (currJoyState) {
        case Up:
          // Incrementa o tempo local
          minute++;
          // Seta o comando para incrementar a minuto atual
          timeCommand = Increment_Minute;
          // Envia o comando para a fila
          sendTimeCommand(to_time_queue);
          break;
        case Down:
          // Decrementa o tempo local
          minute--;
          // Seta o comando para decrementar a minuto atual
          timeCommand = Decrement_Minute;
          // Envia o comando para a fila
          sendTimeCommand(to_time_queue);
          break;
        case Left:
          estadoInterno = H_Hours;
          break;
        case Right:
          estadoInterno = H_Seconds;
          break;
        case Neutral:
          break;
        default:
          Serial.println("Warning - Erro na leitura do Joystick");
      }  // switch
      break;

    case H_Seconds:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Idle_Machine;
      }  // if currButtonState

      switch (currJoyState) {
        case Up:
          // Incrementa o tempo local
          second++;
          // Seta o comando para incrementar o segundo atual
          timeCommand = Increment_Second;
          // Envia o comando para a fila
          sendTimeCommand(to_time_queue);
          break;
        case Down:
          // Decrementa o tempo local
          second--;
          // Seta o comando para decrementar a hora atual
          timeCommand = Decrement_Second;
          // Envia o comando para a fila
          sendTimeCommand(to_time_queue);
          break;
        case Left:
          estadoInterno = H_Minutes;
          break;
        case Right:
          break;
        case Neutral:
          break;
        default:
          Serial.println("Warning - Erro na leitura do Joystick");
      }  // switch

      break;

    case Select_Alarm:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = A_Hours;
      }  // if currButtonState

      if (currJoyState == Up) {
        estadoInterno = Select_Hour;
      } else if (currJoyState == Down) {
        estadoInterno = Select_Tone;
      }  // if currJoyState
      break;

    case A_Hours:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Idle_Machine;
      }  // if currButtonState

      switch (currJoyState) {
        case Up:
          // Incrementa o tempo do alarme local
          alarmHour++;
          // Seta o comando para incrementar a hora do alarme atual
          timeCommand = Increment_Hour;
          // Envia o comando para a fila
          sendTimeCommand(to_alarm_queue);
          break;
        case Down:
          // Decrementa o tempo do alarme local
          alarmHour--;
          // Seta o comando para decrementar a hora do alarme atual
          timeCommand = Decrement_Hour;
          // Envia o comando para a fila
          sendTimeCommand(to_alarm_queue);
          break;
        case Left:
          break;
        case Right:
          estadoInterno = A_Minutes;
          break;
        case Neutral:
          break;
        default:
          Serial.println("Warning - Erro na leitura do Joystick");
      }  // switch
      break;

    case A_Minutes:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Idle_Machine;
      }  // if currButtonState

      switch (currJoyState) {
        case Up:
          // Incrementa o tempo do alarme local
          alarmMinute++;
          // Seta o comando para incrementar o minuto do alarme atual
          timeCommand = Increment_Minute;
          // Envia o comando para a fila
          sendTimeCommand(to_alarm_queue);
          break;
        case Down:
          // Decrementa o tempo do alarme local
          alarmMinute--;
          // Seta o comando para decrementar o minuto do alarme atual
          timeCommand = Decrement_Minute;
          // Envia o comando para a fila
          sendTimeCommand(to_alarm_queue);
          break;
        case Left:
          estadoInterno = A_Hours;
          break;
        case Right:
          break;
        case Neutral:
          break;
        default:
          Serial.println("Warning - Erro na leitura do Joystick");
      }  // switch
      break;

    case Select_Tone:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Tone_List;
      }  // if currButtonState

      if (currJoyState == Up) {
        estadoInterno = Select_Alarm;
      } else if (currJoyState == Down) {
        estadoInterno = Select_Hour;
      }  // if currJoyState
      break;

    case Tone_List:
      if (currButtonState == Pressed && lastButtonState == Released) {
        estadoInterno = Idle_Machine;
      }  // if currButtonState

      if (currJoyState == Up) {
        // Incrementa o índice da lista circular
        buzzSelect = (buzzSelect + 1) % MELODIES_NUM;
        updateBuzzSelect();
      } else if (currJoyState == Down) {
        // Verifica se chegou em 0 para fazer a lista circular
        if (buzzSelect) {
          buzzSelect--;
        } else {
          buzzSelect = MELODIES_NUM - 1;
        }  // if
        updateBuzzSelect();
      }  // if currJoyState
      break;

    default:
      Serial.println("Warning - Estado Interno Corrompido!\nResetando estado interno...");
      estadoInterno = Idle_Machine;
      break;
  }  // switch

}  // Menu::updateState

void Menu::updateLCD() {
  newLcdData = { static_cast<LCD_STATES>(static_cast<int>(estadoInterno)), buzzSelect };  ///< Atualiza os dados para o LCD
  xQueueSend(
    to_lcd_queue,
    &newLcdData,
    0);  ///< Envia os dados atualizados para a fila do LCD
}  // Menu::updateLCD()

void vMenuTaskCode(void* pvParameters) {

  Serial.println("\nInicializando MenuTask...");
  MenuTaskParameters* parameters = reinterpret_cast<MenuTaskParameters*>(pvParameters);
  Menu menuInterno(JOY_X_PIN, JOY_Y_PIN, JOY_BUTTON_PIN,
                   parameters->to_alarm_queue,
                   parameters->to_time_queue,
                   parameters->to_buzzer_queue,
                   parameters->to_lcd_queue);
  Serial.println("MenuTask Inicializada!");

  while (true) {
    /* Verifica interação com o usuário */
    menuInterno.verifyUserInput();
    /* Comunica com o LCD para exibir o estado atualizado do Dispositivo */
    menuInterno.updateLCD();
    vTaskDelay(pdTICKS_TO_MS(1));
  }  // while
}  // vMenuTaskCode
