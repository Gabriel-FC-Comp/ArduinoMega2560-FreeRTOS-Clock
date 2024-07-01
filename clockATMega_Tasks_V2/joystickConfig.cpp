#include "projdefs.h"        ///< Inclui definições de projeto necessárias
#include "joystickConfig.h"  ///< Inclui definições relacionadas ao joystick

/// Construtor da classe Button
Button::Button(int pin)
  : pin(pin), state(Released), lastDebounceTime(0) {}

/// Método para atualizar o estado do botão
void Button::update() {
  int buttonState = !digitalRead(pin);                          // Lê o estado do botão (inverte porque é pull-up)
  unsigned long tempoAtual = millis();                          // Obtém o tempo atual em milissegundos
  unsigned long currBdebounce = tempoAtual - lastDebounceTime;  // Tempo desde o último debounce

  switch (state) {
    case Released:
      if (buttonState == HIGH && currBdebounce > JOYSTICK_BUTTON_DEBOUNCE_INTERVAL) {
        state = Pressed;                // Altera o estado para pressionado
        lastDebounceTime = tempoAtual;  // Registra o tempo do último debounce
      }
      break;

    case Pressed:
      if (buttonState == LOW) {
        state = Released;               // Altera o estado para solto
        lastDebounceTime = tempoAtual;  // Reseta o debounce ao liberar o botão
      }
      break;
  }
}

/// Método para obter o estado do botão
ButtonState Button::getState() {
  return state;  // Retorna o estado atual do botão
}

/// Construtor da classe Joystick
Joystick::Joystick(int xPin, int yPin, int buttonPin)
  : xPin(xPin), yPin(yPin), buttonPin(buttonPin), joyButton(buttonPin),
    lastBdebounce(millis()), lastXdebounce(millis()), lastYdebounce(millis()) {}

/// Método para obter o estado do joystick
JoystickState Joystick::getJoystickState() {
  int xValue = analogRead(xPin);  // Lê o valor do joystick no eixo X
  int yValue = analogRead(yPin);  // Lê o valor do joystick no eixo Y

  unsigned long tempoAtual = millis();                       // Obtém o tempo atual em milissegundos
  unsigned long currXdebounce = tempoAtual - lastXdebounce;  // Tempo desde o último debounce no eixo X
  unsigned long currYdebounce = tempoAtual - lastYdebounce;  // Tempo desde o último debounce no eixo Y

  if (xValue < JOY_LOW_SENSIBILITY && currXdebounce > JOYSTICK_DEBOUNCE_INTERVAL) {
    lastXdebounce = tempoAtual;  // Registra o tempo do último debounce no eixo X
    return Right;                // Retorna a direção direita
  } else if (xValue > JOY_HIGH_SENSIBILITY && currXdebounce > JOYSTICK_DEBOUNCE_INTERVAL) {
    lastXdebounce = tempoAtual;  // Registra o tempo do último debounce no eixo X
    return Left;                 // Retorna a direção esquerda
  } else if (yValue < JOY_LOW_SENSIBILITY && currYdebounce > JOYSTICK_DEBOUNCE_INTERVAL) {
    lastYdebounce = tempoAtual;                                                             // Registra o tempo do último debounce no eixo Y
    return Up;                                                                              // Retorna a direção para cima
  } else if (yValue > JOY_HIGH_SENSIBILITY && currYdebounce > JOYSTICK_DEBOUNCE_INTERVAL) {  // Usando JOY_HIGH_SENSIBILITY
    lastYdebounce = tempoAtual;                                                             // Registra o tempo do último debounce no eixo Y
    return Down;                                                                            // Retorna a direção para baixo
  } else {
    return Neutral;  // Retorna estado neutro se não houver movimento significativo
  }
}

/// Método para atualizar o estado do botão do joystick
void Joystick::updateButtonState() {
  joyButton.update();  // Chama o método de atualização do botão
}

/// Método para obter o estado do botão do joystick
ButtonState Joystick::getButtonState() {
  return joyButton.getState();  // Retorna o estado atual do botão do joystick
}