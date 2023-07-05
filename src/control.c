#include "control.h"
#include <stdbool.h>

int TimeOut;
static bool volatile Parpadeo = 0;
static void setTimeOut(int segundos){
    TimeOut = segundos * CANTIDAD_TICKS_POR_SEGUNDO;
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
void mostrarEnPantalla(Poncho_p poncho, Reloj * reloj, ESTADOS estado,uint8_t temp[6]){  
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
    if ((getEstadoAlarma(reloj)) == ON) {
        PonchoPuntoMode(poncho,0,1);
    }else{
         PonchoPuntoMode(poncho,0,0);
    }
    PonchoWriteDisplay(poncho, hhmm); 
    PonchoDrawDisplay(poncho); 
}
void checkBotones(Poncho_p poncho, Reloj * reloj, ESTADOS * estado, ESTADOS * volver, uint8_t temp[6]){
            
            if ((getEstadoAlarma(reloj)) == ON){
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
                if(!isHighF(poncho,2)) *estado = *volver;
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
                if(PonchoBotonCancelar(poncho) || !TimeOut){
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
                if(PonchoBotonCancelar(poncho) || !TimeOut){
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
                if(PonchoBotonCancelar(poncho) || !TimeOut){
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
                if(PonchoBotonCancelar(poncho) || !TimeOut){
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
Reloj * reloj_tick;
void     tickConfig(Reloj * reloj){
    reloj_tick = reloj;
}

void SysTick_Handler(void){
    if (relojTick(reloj_tick)) {
        Parpadeo = !Parpadeo;
    }
    if(TimeOut>0) TimeOut--;
}

