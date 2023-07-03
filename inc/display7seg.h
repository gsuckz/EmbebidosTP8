#ifndef DISPLAY7_SEG_H
#define DISPLAY7_SEG_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint8_t * Segmentos_dig;

typedef struct Display {
    /**
     * @brief Puntero al mapa de bits que determina el estado de encendido (1) o
     * apagado (0) de los segmentos de los display de 7 segmentos que
     * componen la pantalla. b<0> : A, b<1> : B, ..., b<6> : G
     * 
     */
    Segmentos_dig  segmentos_digito;
    uint8_t num_digitos;
    void (* set_segmento) (uint8_t, bool);
    void (* set_digito)   (uint8_t, bool);

}Display;



void writeDisplay(Display * display, int numero);
/**
 * @brief Escribe en la memoria de video del controlador del Display
 * 
 * @param display Puntero al objeto  display
 * @param digito Digito que se quiere escribir
 * @param numero Numero que se quiere dibujar (0-9)
 */
void writeDisplayDig   (Display * display, uint8_t digito, uint8_t numero); //char?

/**
 * @brief Muestra lo que hay en la memoria de video en el Display
 * 
 */
void drawDisplay (Display * display);

/**
 * @brief Inicia un objeto display
 * 
 * @param set_segmento Funcion para controlar los segmentos
 * @param set_digito Funcion para controlar el Comun
 * @param set_num_digitos Cantidad de digitos del display
 * @return Display* 
 */
Display * displayInit (void (*set_segmento)(uint8_t,bool),void (*set_digito)(uint8_t,bool),uint8_t set_num_digitos );
#endif