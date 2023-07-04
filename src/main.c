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
#include "ajuste.h"
#include <stdbool.h>
/* === Macros definitions ====================================================================== */
#define CANTIDADTICKS 100
/* === Private data type declarations ========================================================== */
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
/* === Public variable definitions ============================================================= */
int TimeOut;
/* === Private variable definitions ============================================================ */
static bool volatile Parpadeo = 0;
static Poncho_p poncho;
static Reloj *  reloj;

/* === Private function implementation ========================================================= */
static void setTimeOut(int segundos){
    TimeOut = segundos * CANTIDADTICKS;
}
static void ControladorAlarma(bool estado){
    PonchoBuzzer(poncho,estado);
    return;
}
static void parpadeoMM(uint8_t * hhmm){
    if (Parpadeo) {
        hhmm[2] = -1;
        hhmm[3] = -1;
    }
}
static void parpadeoHH(uint8_t * hhmm){
    if (Parpadeo) {
        hhmm[0] = -1;
        hhmm[1] = -1;
    }
}
static void parpadeoHHMM(uint8_t * hhmm){
    parpadeoHH(hhmm);
    parpadeoMM(hhmm);
}
static void mostrarEnPantalla(Reloj * reloj, ESTADOS estado,uint8_t temp[6]){  
    uint8_t hhmm[4];
    for (int i=0;i<=4;i++) hhmm[i] = temp[i];
    switch (estado)
    {
        case E_MOD_ALARMA_MIN:
        case E_MOD_HORARIO_MIN: //FALLTHRU
            parpadeoMM(hhmm); 
        break;
        case E_MOD_ALARMA_HOR:
            PonchoPuntoMode(poncho,0,Parpadeo);
            PonchoPuntoMode(poncho,1,Parpadeo);
            PonchoPuntoMode(poncho,2,Parpadeo);
            PonchoPuntoMode(poncho,3,Parpadeo);
        case E_MOD_HORARIO_HOR: //FALLTRHU
            parpadeoHH(hhmm);   
        break;
        case E_RESET:
            parpadeoHHMM(hhmm);
        default:
            PonchoPuntoMode(poncho,2,Parpadeo);
        break;
    }
    if (getEstadoAlarma == ON) {
        PonchoPuntoMode(poncho,0,1);
    }else{
         PonchoPuntoMode(poncho,0,0);
    }
    PonchoWriteDisplay(poncho, hhmm); 
    PonchoDrawDisplay(poncho); 
}

static checkBotones(ESTADOS * estado, ESTADOS * volver, uint8_t temp[6]){
            
            if (getEstadoAlarma == ON){
                if(PonchoBotonAceptar(poncho)) relojSnooze(reloj,5);
                if(PonchoBotonCancelar(poncho)) setAlarmaEstado(reloj,READY);
            }
            switch (*estado){
            case E_RESET:                      
            case E_MOSTRAR_HORA: //FALLTHRU
                relojHorario(reloj,temp);
                if(isHighF(poncho,2)) {
                    *volver = *estado;
                    setTimeOut(3);
                    *estado = E_ESPERA_MOD_HORARIO;
                }
                if(isHighF(poncho,3)) {
                    *volver = *estado;
                    setTimeOut(3);
                    *estado = E_ESPERA_MOD_ALARMA;
                }
                if(PonchoBotonAceptar(poncho)) setAlarmaEstado(reloj,READY);
                if(PonchoBotonCancelar(poncho)) setAlarmaEstado(reloj,OFF);
            break;case E_ESPERA_MOD_ALARMA:
                if(!isHighF(poncho,3)) *estado = *volver;
                if(!TimeOut) {
                    setTimeOut(30);
                    getAlarmaHora(reloj,temp);
                    *estado = E_MOD_ALARMA_MIN;
                }
            break;case E_ESPERA_MOD_HORARIO:
                if(!isHighF(poncho,3)) *estado = *volver;
                if(!TimeOut) {
                    setTimeOut(30);
                    relojHorario(reloj,temp);
                    *estado = E_MOD_HORARIO_MIN;
                }
            break;case E_MOD_ALARMA_MIN:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarMinutos(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarMinutos(temp);
                }
                if(PonchoBotonCancelar || !TimeOut){
                    *estado = *volver;
                }
                if(PonchoBotonAceptar(poncho)){
                    setTimeOut(30);
                    *estado = E_MOD_ALARMA_HOR;
                }
            break;case E_MOD_ALARMA_HOR:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarHoras(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarHoras(temp);
                }
                if(PonchoBotonCancelar || !TimeOut){
                    *estado = *volver;
                }   
                if(PonchoBotonAceptar(poncho)){
                    setTimeOut(30);
                    setAlarmaHora(reloj,temp);
                    setAlarmaEstado(reloj,READY);
                    *estado = E_MOSTRAR_HORA;
                }                    
            break;case E_MOD_HORARIO_MIN:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarMinutos(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarMinutos(temp);
                }
                if(PonchoBotonCancelar || !TimeOut){
                    *estado = *volver;
                }
                if(PonchoBotonAceptar(poncho)){
                    setTimeOut(30);
                    *estado = E_MOD_HORARIO_HOR;
                }           
            break;case E_MOD_HORARIO_HOR:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarHoras(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarHoras(temp);
                }
                if(PonchoBotonCancelar || !TimeOut){
                    *estado = *volver;
                }   
                if(PonchoBotonAceptar(poncho)){
                    relojGuardarHora(reloj,temp);
                    *estado = E_MOSTRAR_HORA;
                }              
            break;default:
            break;
        }    
}
/* === Public function implementation ========================================================= */
void SysTick_Handler(void){
    if (relojTick(reloj)) {
        Parpadeo = !Parpadeo;
    }
    if(TimeOut>0) TimeOut--;
}
int main(void) {
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / (CANTIDADTICKS));
    poncho = PonchoInit();
    reloj = relojCrear(CANTIDADTICKS, ControladorAlarma);
    ESTADOS estado = E_RESET, volver = E_RESET;
    uint8_t temp[6] = {0,0, 0,0 ,0,0};    
    while (1){ ///LAZO PRINCIPAL 
        mostrarEnPantalla(reloj,estado,temp);  
        checkBotones(&estado,&volver,temp);         
    }
}



/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
