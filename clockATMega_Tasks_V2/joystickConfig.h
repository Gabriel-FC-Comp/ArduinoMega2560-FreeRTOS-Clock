#ifndef joystickConfig_H
#define joystickConfig_H

#include "Arduino.h"  ///< Inclui a biblioteca do Arduino

// Definição dos pinos para o joystick e botão
#define JOY_X_PIN A0                          ///< Pino para a leitura do eixo X do joystick
#define JOY_Y_PIN A1                          ///< Pino para a leitura do eixo Y do joystick
#define JOY_BUTTON_PIN 7                      ///< Pino para o botão do joystick
#define JOY_LOW_SENSIBILITY 150               ///< Sensibilidade baixa do joystick
#define JOY_HIGH_SENSIBILITY 850              ///< Sensibilidade alta do joystick
#define JOYSTICK_BUTTON_DEBOUNCE_INTERVAL 50  ///< Intervalo de debounce para o botão do joystick
#define JOYSTICK_DEBOUNCE_INTERVAL 200        ///< Intervalo de debounce para o joystick
#define JOYSTICK_DEBOUNCE_INTERVAL_MENU 300   ///< Intervalo de debounce para o joystick no menu

// Estados para o botão
enum ButtonState {
  Released,  ///< Estado liberado do botão
  Pressed    ///< Estado pressionado do botão
};

// Estados para o joystick
enum JoystickState {
  Neutral,  ///< Estado neutro do joystick
  Up,       ///< Estado para cima do joystick
  Down,     ///< Estado para baixo do joystick
  Left,     ///< Estado para a esquerda do joystick
  Right     ///< Estado para a direita do joystick
};

// Classe do botão
class Button {
private:
  int pin;                         ///< Pino de leitura do botão
  ButtonState state;               ///< Estado atual do botão
  unsigned long lastDebounceTime;  ///< Último momento de debounce do botão
public:
  Button(int pin);         ///< Construtor da classe Button
  void update();           ///< Método para atualizar o estado do botão
  ButtonState getState();  ///< Método para obter o estado do botão
};

// Classe do joystick
class Joystick {
private:
  int xPin;                     ///< Pino de leitura do eixo X do joystick
  int yPin;                     ///< Pino de leitura do eixo Y do joystick
  int buttonPin;                ///< Pino de leitura do botão
  unsigned long lastBdebounce;  ///< Último momento de debounce do botão
  unsigned long lastXdebounce;  ///< Variável auxiliar para debounce do eixo X
  unsigned long lastYdebounce;  ///< Variável auxiliar para debounce do eixo Y
  Button joyButton;             ///< Objeto do botão do joystick
public:
  Joystick(int xPin, int yPin, int buttonPin);  ///< Construtor da classe Joystick
  JoystickState getJoystickState();             ///< Método para obter o estado do joystick
  ButtonState getButtonState();                 ///< Método para obter o estado do botão do joystick
  void updateButtonState();                     ///< Método para atualizar o estado do botão do joystick
};

#endif  // joystickConfig_H