#ifndef CONTROL_H
#define CONTROL_H
#include "ajuste.h"
#include "mybsp.h"
#include "poncho.h"
#include "reloj.h"
#include "control.h"

#define CANTIDAD_TICKS_POR_SEGUNDO 100

typedef enum ESTADOS{
    E_RESET,
    E_ESPERA_MOD_HORARIO_R,
    E_ESPERA_MOD_HORARIO,
    E_ESPERA_MOD_ALARMA_R,
    E_ESPERA_MOD_ALARMA,
    E_MOSTRAR_HORA,
    E_MOD_HORARIO_MIN,
    E_MOD_HORARIO_HOR,
    E_MOD_ALARMA_MIN,
    E_MOD_ALARMA_HOR
} ESTADOS;


void     tickConfig(Reloj * reloj); 
void checkBotones(Poncho_p poncho,Reloj * reloj, ESTADOS * estado, ESTADOS * volver, uint8_t temp[6]);

void mostrarEnPantalla(Poncho_p poncho, Reloj * reloj, ESTADOS estado,uint8_t temp[6]);
#endif