/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "mybsp.h"
#include "poncho.h"
#include "reloj.h"
#include <stdbool.h>

#define CANTIDADTICKS 10000

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */
typedef enum ESTADOS{
    E_RESET,
    E_HORA,
    E_MOD_HORA,
    E_MOD_ALARMA
} ESTADOS;

typedef enum Eventos{
    F1,
    F2,
    F3,
    F4,
    ACEPTAR,
    CANCELAR,
    TIMEOUT
} Eventos;
/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */
static int hhmmToInt(uint8_t * numero){
    int resultado =0;
    resultado += numero[3];
    resultado += 10*numero[2];
    resultado += 100*numero[1];
    resultado += 1000*numero[0];
    return resultado;
}
/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */
void alar (bool a){
    return;
}

int TimeOut;
void setTimeOut(int time){
    TimeOut = time / CANTIDADTICKS;
}

Poncho_p poncho;
Reloj *  reloj;


void modificar(ESTADOS estado, uint8_t hhmm[6], bool inc){

}



int main(void) {
    Poncho_p poncho;
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / CANTIDADTICKS);
    poncho = PonchoInit();
    reloj = relojCrear(CANTIDADTICKS, alar);
    ESTADOS estado = E_HORA;
    static uint8_t hhmm[6] = {0,0,0,0,0,0};
    relojGuardarHora(reloj,hhmm);
    while (1){ ///LAZO PRINCIPAL 
    //Decide que mostrar en pantalla   

        switch (estado)
        {
        case E_HORA:        
            relojHorario(reloj,hhmm);
            TimeOut = 3000;
            while(isHighF(poncho,2)){
                if (!TimeOut) {
                    estado = E_MOD_HORA;
                    TimeOut = 30000;
                    break;
                }                    
            }
            while(isHighF(poncho,3)){
            TimeOut = 3000;
                if (!TimeOut) {
                    estado = E_MOD_ALARMA;
                    getAlarmaHora(reloj,hhmm);
                    TimeOut = 30000;
                    break;
                }
            }
        break;case E_MOD_ALARMA:   
            if(!TimeOut) estado = E_HORA;
            if(PonchoBotonFuncion(poncho,3)){
                modificar(estado,hhmm,0);
                TimeOut = 30000;    
                }
            if(PonchoBotonFuncion(poncho,4)){
                modificar(estado,hhmm,1);
                TimeOut = 30000;
                }
            if(PonchoBotonAceptar(poncho)){
                setAlarmaHora(reloj,hhmm);
                estado = E_HORA;
            }

        break;case E_MOD_HORA:
            if(!TimeOut) estado = E_HORA;
            if(PonchoBotonFuncion(poncho,3)){
                modificar(estado,hhmm,0);
                TimeOut = 30000;
                }
            if(PonchoBotonFuncion(poncho,4)){
                modificar(estado,hhmm,1);                    
                TimeOut = 30000;
                }
            if(PonchoBotonAceptar(poncho)){
                relojGuardarHora(reloj,hhmm);
                estado = E_HORA;
            }
        break;default:

        break;
        }

        PonchoWriteDisplay(poncho, hhmmToInt(hhmm));
        PonchoDrawDisplay(poncho);
        }
    }


void SysTick_Handler(void){
    relojTick(reloj);
    if(TimeOut>0) TimeOut--;
}
/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
