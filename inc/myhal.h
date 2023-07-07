#ifndef HAL_H
#define HAL_H
#include <stdbool.h>
#include <stdint.h>

typedef enum HAL_ModoPin { ENTRADA , SALIDA } HAL_ModoPin;

typedef struct Pin {
    uint8_t puerto;
    uint8_t pin;
} Pin;

typedef struct pinGPIO {
    uint8_t numRegistroGPIO;
    uint8_t bitRegistroGPIO;
    uint8_t funcion;
} pinGPIO;



/**
 * @brief Configura el pin del puerto como entrada o salida, si es salida inicia en 0
 *
 * @param pin Puntero a objeto pin
 * @param modo HAL_ModoPin "ENTRADA" o "SALIDA"
 */
void configPin(Pin const * pin, HAL_ModoPin modo);
/**
 * @brief Devuelve un Bool con el valor del pin leido
 *
 * @param pin Puntero a objeto pin
 * @return true Si esta en alto
 * @return false Si esta en bajo
 */
bool readPin(Pin const * pin);
/**
 * @brief Pone el pin del puerto en el valor
 *
 *
 * @param pin Puntero a objeto pin
 * @param valor Valor a asignar al pin del puerto
 */
void writePin(Pin const * pin, bool valor);
/**
 * @brief
 *
 * @param pin
 */
void tooglePin(Pin const * pin);
#endif
