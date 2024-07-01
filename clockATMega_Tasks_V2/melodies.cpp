#include "projdefs.h"  ///< Inclui definições de projeto necessárias
#include "melodies.h"

// Melodia da música Saria's Song de The Legend of Zelda
const int melodia_zelda[] = {
  0, NOTE_F4, NOTE_A4, NOTE_B4, 0,
  NOTE_F4, NOTE_A4, NOTE_B4, 0,
  NOTE_F4, NOTE_A4, NOTE_B4, NOTE_E5, NOTE_D5, 0,
  NOTE_B4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_E4, 0,
  NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, 0,

  NOTE_F4, NOTE_A4, NOTE_B4, 0,
  NOTE_F4, NOTE_A4, NOTE_B4, 0,
  NOTE_F4, NOTE_A4, NOTE_B4, NOTE_E5, NOTE_D5, 0,
  NOTE_B4, NOTE_C5, NOTE_E5, NOTE_B4, NOTE_G4, 0,
  NOTE_B4, NOTE_G4, NOTE_D4, NOTE_E4
};

// Durações das notas
const int duracoes_zelda[] = {
  1, 2, 2, 3, 1,    // Primeira frase
  2, 2, 3, 1,    // Segunda frase
  2, 2, 2, 2, 4, 1,    // Terceira frase
  2, 2, 2, 2, 4, 1,    // Quarta frase
  2, 2, 2, 4, 1,    // Quinta frase
  2, 2, 3, 1,    // Sexta frase
  2, 2, 3, 1,    // Sétima frase
  2, 2, 2, 2, 4, 1,    // Oitava frase
  2, 2, 2, 2, 4, 1,    // Nona frase
  2, 2, 2, 4    // Décima frase
};

///< Maior índice dentre as notas da melodia
const int maxIndex_zelda = sizeof(melodia_zelda) / sizeof(melodia_zelda[0]);

// Melodia do Theme de Mario
const int melodia_mario[] = {
  0,  NOTE_E5,  0,  NOTE_E5,  0,  NOTE_E5,  NOTE_C5,  NOTE_E5,  NOTE_G5,  NOTE_G4,
  0,  NOTE_C5,  NOTE_G4,  NOTE_E4,  NOTE_A4,  NOTE_B4,  NOTE_AS4,  NOTE_A4,  NOTE_G4,
  0,  NOTE_E5,  NOTE_G5,  NOTE_A5,  NOTE_F5,  NOTE_G5,  NOTE_E5,  NOTE_C5,  NOTE_D5,  NOTE_B4,
  0,  NOTE_G5,  NOTE_FS5,  NOTE_F5,  NOTE_DS5,  NOTE_E5,  0,  NOTE_GS4,  NOTE_A4,  NOTE_C5,  NOTE_A4,
  NOTE_C5,  NOTE_D5,  0,  NOTE_G5,  NOTE_FS5,  NOTE_F5,  NOTE_DS5,  NOTE_E5,  
  0,  NOTE_C6,  0,  NOTE_C6,  0,  NOTE_C6,
};

// Durações das notas
const int duracoes_mario[] = {
  1, 1, 0, 1, 0, 2, 1, 1, 2, 2, 1,    // Primeira frase
  2, 2, 2, 1, 2, 1, 1, 2, 1,    // Segunda frase
  1, 1, 2, 1, 2, 2, 1, 1, 2, 1,    // Terceira frase
  2, 2, 2, 1, 2, 1, 1, 2, 1,    // Quarta frase
  1, 1, 2, 1, 2, 2, 1, 1, 2, 1,    // Quinta frase

  1, 1, 1, 1, 2, 1,    // Sexta frase
  1, 1, 2, 1, 1, 2, 1,    // Sétima frase
  1, 1, 1, 1, 2, 1,    // Oitava frase
  1, 0, 1, 0, 1    // Nona frase
};

///< Maior índice dentre as notas da melodia
const int maxIndex_mario = sizeof(melodia_mario) / sizeof(melodia_mario[0]);
