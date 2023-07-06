#include <stdbool.h>
#include "control.h"

int TimeOut;
static ESTADOS estado;
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
    uint8_t hhmm[6] = {0,0,0,0,0,0};
    switch (estado)
    { 
        case E_MOD_ALARMA_HOR:
        case E_MOD_ALARMA_MIN:    //FALLTHRU
        case E_MOD_ALARMA_HOR_R: //FALLTHRU
        case E_MOD_ALARMA_MIN_R://FALLTHRU
            PonchoPuntoMode(poncho,0,1);
            PonchoPuntoMode(poncho,1,1);
            PonchoPuntoMode(poncho,2,1);
            PonchoPuntoMode(poncho,3,1);
        case E_MOD_HORARIO_HOR:      //FALLTRHU
        case E_MOD_HORARIO_MIN:     //FALLTHRU
        case E_MOD_HORARIO_HOR_R:  //FALLTRHU
        case E_MOD_HORARIO_MIN_R: //FALLTHRU
            for (int i=0; i<=3 ;i++) {
                hhmm[i] = temp[i];
            }              
        void (*parpadear)(uint8_t * hhmm) =((estado == E_MOD_ALARMA_MIN)    || 
                                            (estado == E_MOD_ALARMA_MIN_R)  ||//¿Funcionaba sin estas lineas?
                                            (estado == E_MOD_HORARIO_MIN_R) ||//¿Funcionaba sin estas lineas?
                                            (estado == E_MOD_HORARIO_MIN)  
        ) ? parpadeoMM : parpadeoHH; 
        parpadear(hhmm);            
        break;case E_RESET:
        case E_ESPERA_MOD_ALARMA_R:   //FALLTHRU 
        case E_ESPERA_MOD_HORARIO_R: //FALLTHRU        
            parpadeoHHMM(hhmm);
            PonchoPuntoMode(poncho,0,0);
            PonchoPuntoMode(poncho,1,0);
            PonchoPuntoMode(poncho,2,!Parpadeo);                
            PonchoPuntoMode(poncho,3,0);            
        break;case E_MOSTRAR_HORA:
        case E_ESPERA_MOD_ALARMA:   //FALLTHRU
        case E_ESPERA_MOD_HORARIO:  //FALLTHRU
            relojHorario(reloj,hhmm);
            PonchoPuntoMode(poncho,0,0);
            PonchoPuntoMode(poncho,1,0);
            PonchoPuntoMode(poncho,2,!Parpadeo);                
            PonchoPuntoMode(poncho,3,0);            
        break;default:
        break;
    }
    if ((getEstadoAlarma(reloj)) == READY) {
        PonchoPuntoMode(poncho,0,1);
    }
    PonchoWriteDisplay(poncho, hhmm); 
    PonchoDrawDisplay(poncho); 
}
void checkBotones(Poncho_p poncho, Reloj * reloj, uint8_t temp[6]){
            
            if ((getEstadoAlarma(reloj)) == ON){    //Los botones (ACEPTAR) y (CANCELAR) solo funcionan para la alarma
                if(PonchoBotonFuncion(poncho,1)) { // cuando esta sonando. 
                    relojSnooze(reloj,5);
                    return;}
                if(PonchoBotonCancelar(poncho)) {
                    setAlarmaEstado(reloj,READY);
                    return;}
            }
            switch (*estado){
            case E_RESET: 
                for (int i=0; i<=3 ;i++) {
                    temp[i] = 0;
                }                                           
                if(isHighF(poncho,2)) {
                    setTimeOut(3);
                    *estado = E_ESPERA_MOD_HORARIO_R;
                }
                if(isHighF(poncho,3)) {
                    setTimeOut(3);
                    *estado = E_ESPERA_MOD_ALARMA_R;
                }
            break;case E_MOSTRAR_HORA: //FALLTHRU
                if(isHighF(poncho,2)) {
                    setTimeOut(3);
                    *estado = E_ESPERA_MOD_HORARIO;
                }
                if(isHighF(poncho,3)) {
                    setTimeOut(3);
                    *estado = E_ESPERA_MOD_ALARMA;
                }
                if(PonchoBotonFuncion(poncho,1)) setAlarmaEstado(reloj,READY);
                if(PonchoBotonCancelar(poncho)) setAlarmaEstado(reloj,OFF);
            break;case E_ESPERA_MOD_ALARMA:
                if(!isHighF(poncho,3)) *estado = E_MOSTRAR_HORA;
                if(!TimeOut) {
                    setTimeOut(30);
                    getAlarmaHora(reloj,temp);
                    *estado = E_MOD_ALARMA_MIN;
                }
            break;case E_ESPERA_MOD_HORARIO:
                if(!isHighF(poncho,2)) *estado = E_MOSTRAR_HORA;
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
                    *estado = E_MOSTRAR_HORA;
                }
                if(PonchoBotonFuncion(poncho,1)){
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
                    *estado = E_MOSTRAR_HORA;
                }   
                if(PonchoBotonFuncion(poncho,1)){
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
                    *estado = E_MOSTRAR_HORA;
                }
                if(PonchoBotonFuncion(poncho,1)){
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
                    *estado = E_MOSTRAR_HORA;
                }   
                if(PonchoBotonFuncion(poncho,1)){
                    relojGuardarHora(reloj,temp);
                    *estado = E_MOSTRAR_HORA;
                }              
            break;case E_ESPERA_MOD_ALARMA_R:
                if(!isHighF(poncho,3)) *estado = E_RESET;
                if(!TimeOut) {
                    setTimeOut(30);
                    getAlarmaHora(reloj,temp);
                    *estado = E_MOD_ALARMA_MIN_R;
                }
            break;case E_ESPERA_MOD_HORARIO_R:
                if(!isHighF(poncho,2)) *estado = E_RESET;
                if(!TimeOut) {
                    setTimeOut(30);
                    relojHorario(reloj,temp);
                    *estado = E_MOD_HORARIO_MIN_R;
                }
            break;case E_MOD_ALARMA_MIN_R:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarMinutos(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarMinutos(temp);
                }
                if(PonchoBotonCancelar(poncho) || !TimeOut){
                    *estado = E_RESET;
                }
                if(PonchoBotonFuncion(poncho,1)){
                    setTimeOut(30);
                    *estado = E_MOD_ALARMA_HOR_R;
                }
            break;case E_MOD_ALARMA_HOR_R:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarHoras(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarHoras(temp);
                }
                if(PonchoBotonCancelar(poncho) || !TimeOut){
                    *estado = E_RESET;
                }   
                if(PonchoBotonFuncion(poncho,1)){
                    setTimeOut(30);
                    setAlarmaHora(reloj,temp);
                    setAlarmaEstado(reloj,READY);
                    *estado = E_MOSTRAR_HORA;
                }                    
            break;case E_MOD_HORARIO_MIN_R:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarMinutos(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarMinutos(temp);
                }
                if(PonchoBotonCancelar(poncho) || !TimeOut){
                    *estado = E_RESET;
                }
                if(PonchoBotonFuncion(poncho,1)){
                    setTimeOut(30);
                    *estado = E_MOD_HORARIO_HOR_R;
                }           
            break;case E_MOD_HORARIO_HOR_R:
                if(PonchoBotonFuncion(poncho,3)){
                    setTimeOut(30);
                    incrementarHoras(temp);
                }
                if(PonchoBotonFuncion(poncho,4)){
                    setTimeOut(30);
                    decrementarHoras(temp);
                }
                if(PonchoBotonCancelar(poncho) || !TimeOut){
                    *estado = E_RESET;
                }   
                if(PonchoBotonFuncion(poncho,1)){
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

void timeOutCheck(void){
    if(TimeOut>0) TimeOut--;
}

void segRefParpadeo(void){
    Parpadeo = !Parpadeo;
}