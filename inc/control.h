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
    E_MOD_ALARMA_HOR,
    E_MOD_HORARIO_MIN_R,
    E_MOD_HORARIO_HOR_R,
    E_MOD_ALARMA_MIN_R,
    E_MOD_ALARMA_HOR_R
} ESTADOS;
typedef struct Control Control;
/**
 * @brief Verifica el estado de los botones asociados al controlador y
 * realiza las funciones correspondientes
 * 
 * @param controlador 
 */
void checkBotones(Control * controlador);
/**
 * @brief Refresca el display del controlador mostrando la informacion adecuada
 * 
 * @param controlador 
 */
void mostrarEnPantalla(Control * controlador);
/**
 * @brief Funcion para actualizar los valores de tiempo del controlador
 * 
 * @param ctrl 
 */
void sysTickCtrl(Control * ctrl);
Control * crearControlador(int ticks_seg, void (*ControladorAlarma)(bool), Poncho_p poncho);
/**
 * @brief Devuelve el putnero al reloj del controlador, se usa para podes usar la funcion
 * relojTick() de la libreria reloj
 * 
 * @param controlador 
 * @return Reloj* 
 */
Reloj * relojDe(Control * controlador);
#endif