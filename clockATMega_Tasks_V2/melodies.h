
#ifndef melodies_H
#define melodies_H

#define MELODIES_NUM 2  ///< Número de melodias disponíveis

// Definição das frequências das notas musicais
#define NOTE_C4 261   // Dó (C4)
#define NOTE_CS4 277  // Dó# / Réb (C#4 / Db4)
#define NOTE_D4 293   // Ré (D4)
#define NOTE_DS4 311  // Ré# / Mib (D#4 / Eb4)
#define NOTE_E4 329   // Mi (E4)
#define NOTE_F4 349   // Fá (F4)
#define NOTE_FS4 369  // Fá# / Solb (F#4 / Gb4)
#define NOTE_G4 392   // Sol (G4)
#define NOTE_GS4 415  // Sol# / Láb (G#4 / Ab4)
#define NOTE_A4 440   // Lá (A4)
#define NOTE_AS4 466  // Lá# / Síb (A#4 / Bb4)
#define NOTE_B4 493   // Si (B4)
#define NOTE_C5 523   // Dó (C5)
#define NOTE_CS5 554  // Dó# / Réb (C#5 / Db5)
#define NOTE_D5 587   // Ré (D5)
#define NOTE_DS5 622  // Ré# / Mib (D#5 / Eb5)
#define NOTE_E5 659   // Mi (E5)
#define NOTE_F5 698   // Fá (F5)
#define NOTE_FS5 739  // Fá# / Solb (F#5 / Gb5)
#define NOTE_G5 783   // Sol (G5)
#define NOTE_GS5 830  // Sol# / Láb (G#5 / Ab5)
#define NOTE_A5 880   // Lá (A5)
#define NOTE_AS5 932  // Lá# / Síb (A#5 / Bb5)
#define NOTE_B5 987   // Si (B5)
#define NOTE_C6 1046  // Dó (C6)

// Melodia da música Saria's Song de The Legend of Zelda
extern const int melodia_zelda[];

// Durações das notas
extern const int duracoes_zelda[];

// Limite do índice / máximo de notas
extern const int maxIndex_zelda;

// Melodia do Theme de Mario
extern const int melodia_mario[];

// Durações das notas
extern const int duracoes_mario[];

// Limite do índice / máximo de notas
extern const int maxIndex_mario;

#endif
