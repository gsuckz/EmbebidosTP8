#ifndef MYBSP_H
#define MYBSP_H
#include "myhal.h"
#include "poncho.h"
#include "display7seg.h"
#include <stdint.h>


typedef struct Poncho * Poncho_p;

/**
 * @brief Inicializa los pines del poncho, ademàs devuleve un puntero al obj. Poncho
 *
 * @return Poncho*
 */
Poncho_p PonchoInit(void);

bool PonchoBotonCancelar(Poncho_p poncho);
bool PonchoBotonAceptar(Poncho_p poncho);
bool PonchoBotonFuncion(Poncho_p poncho, uint8_t i);

/**
 * @brief Escribe el numero en el Display del Poncho
 *
 * @param poncho El poncho
 * @param numero
 */
void PonchoWriteDisplay(Poncho_p poncho, uint8_t  numero[]);
/**
 * @brief Refresca el dispaly del poncho
 *
 * @param poncho El poncho
 */
void PonchoDrawDisplay(Poncho_p poncho);


/**
 * @brief Enciende o apaga el buzzer
 * 
 */
void PonchoBuzzer(Poncho_p poncho,bool estado);

bool isHighF(Poncho_p poncho, uint8_t funcion);

void PonchoPuntoMode(Poncho_p poncho,uint8_t i, bool estado);

#endif