#include <stdbool.h>
#include "control.h"


typedef struct Control{
    Poncho_p poncho;
    Reloj * reloj;
    ESTADOS estado;
    int TimeOut;
    uint8_t temp [6];
}Control;



static bool volatile Parpadeo = 0;
static void steTimeOut(Control * controlador, int segundos){
    controlador->TimeOut = segundos * CANTIDAD_TICKS_POR_SEGUNDO;
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
Control * crearControlador(int ticks_seg, void (*ControladorAlarma)(bool)){
    Control * controlador = malloc(sizeof(Control));
    controlador->poncho = PonchoInit();
    controlador->reloj = relojCrear(ticks_seg, ControladorAlarma);
    return controlador;
    }



void mostrarEnPantalla(Control * controlador){  
    uint8_t hhmm[6] = {0,0,0,0,0,0};
    switch (controlador->estado)
    { 
        case E_MOD_ALARMA_HOR:
        case E_MOD_ALARMA_MIN:    //FALLTHRU
        case E_MOD_ALARMA_HOR_R: //FALLTHRU
        case E_MOD_ALARMA_MIN_R://FALLTHRU
            PonchoPuntoMode(controlador->poncho,0,1);
            PonchoPuntoMode(controlador->poncho,1,1);
            PonchoPuntoMode(controlador->poncho,2,1);
            PonchoPuntoMode(controlador->poncho,3,1);
        case E_MOD_HORARIO_HOR:      //FALLTRHU
        case E_MOD_HORARIO_MIN:     //FALLTHRU
        case E_MOD_HORARIO_HOR_R:  //FALLTRHU
        case E_MOD_HORARIO_MIN_R: //FALLTHRU
            for (int i=0; i<=3 ;i++) {
                hhmm[i] = controlador->temp[i];
            }              
        void (*parpadear)(uint8_t * hhmm) =((controlador->estado == E_MOD_ALARMA_MIN)    || 
                                            (controlador->estado == E_MOD_ALARMA_MIN_R)  ||//¿Funcionaba sin estas lineas?
                                            (controlador->estado == E_MOD_HORARIO_MIN_R) ||//¿Funcionaba sin estas lineas?
                                            (controlador->estado == E_MOD_HORARIO_MIN)  
        ) ? parpadeoMM : parpadeoHH; 
        parpadear(hhmm);            
        break;case E_RESET:
        case E_ESPERA_MOD_ALARMA_R:   //FALLTHRU 
        case E_ESPERA_MOD_HORARIO_R: //FALLTHRU        
            parpadeoHHMM(hhmm);
            PonchoPuntoMode(controlador->poncho,0,0);
            PonchoPuntoMode(controlador->poncho,1,0);
            PonchoPuntoMode(controlador->poncho,2,!Parpadeo);                
            PonchoPuntoMode(controlador->poncho,3,0);            
        break;case E_MOSTRAR_HORA:
        case E_ESPERA_MOD_ALARMA:   //FALLTHRU
        case E_ESPERA_MOD_HORARIO:  //FALLTHRU
            relojHorario(controlador->reloj,hhmm);
            PonchoPuntoMode(controlador->poncho,0,0);
            PonchoPuntoMode(controlador->poncho,1,0);
            PonchoPuntoMode(controlador->poncho,2,!Parpadeo);                
            PonchoPuntoMode(controlador->poncho,3,0);            
        break;default:
        break;
    }
    if ((getEstadoAlarma(controlador->reloj)) == READY) {
        PonchoPuntoMode(controlador->poncho,0,1);
    }
    PonchoWriteDisplay(controlador->poncho, hhmm); 
    PonchoDrawDisplay(controlador->poncho); 
}
void checkBotones(Control * controlador){
            
            if ((getEstadoAlarma(controlador->reloj)) == ON){    //Los botones (ACEPTAR) y (CANCELAR) solo funcionan para la alarma
                if(PonchoBotonFuncion(controlador->poncho,1)) { // cuando esta sonando. 
                    relojSnooze(controlador->reloj,5);
                    return;}
                if(PonchoBotonCancelar(controlador->poncho)) {
                    setAlarmaEstado(controlador->reloj,READY);
                    return;}
            }
            switch (controlador->estado){
            case E_RESET: 
                for (int i=0; i<=3 ;i++) {
                    controlador->temp[i] = 0;
                }                                           
                if(isHighF(controlador->poncho,2)) {
                    steTimeOut(controlador,3);
                    controlador->estado = E_ESPERA_MOD_HORARIO_R;
                }
                if(isHighF(controlador->poncho,3)) {
                    steTimeOut(controlador,3);
                    controlador->estado = E_ESPERA_MOD_ALARMA_R;
                }
            break;case E_MOSTRAR_HORA: //FALLTHRU
                if(isHighF(controlador->poncho,2)) {
                    steTimeOut(controlador,3);
                    controlador->estado = E_ESPERA_MOD_HORARIO;
                }
                if(isHighF(controlador->poncho,3)) {
                    steTimeOut(controlador,3);
                    controlador->estado = E_ESPERA_MOD_ALARMA;
                }
                if(PonchoBotonFuncion(controlador->poncho,1)) setEstado(controlador->reloj,READY);
                if(PonchoBotonCancelar(controlador->poncho)) setEstado(controlador->reloj,OFF);
            break;case E_ESPERA_MOD_ALARMA:
                if(!isHighF(controlador->poncho,3)) controlador->estado = E_MOSTRAR_HORA;
                if(!controlador->TimeOut) {
                    steTimeOut(controlador,30);
                    getAlarmaHora(controlador->reloj,controlador->temp);
                    controlador->estado = E_MOD_ALARMA_MIN;
                }
            break;case E_ESPERA_MOD_HORARIO:
                if(!isHighF(controlador->poncho,2)) controlador->estado = E_MOSTRAR_HORA;
                if(!controlador->TimeOut) {
                    steTimeOut(controlador,30);
                    relojHorario(controlador->reloj,controlador->temp);
                    controlador->estado = E_MOD_HORARIO_MIN;
                }
            break;case E_MOD_ALARMA_MIN:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarMinutos(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarMinutos(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_MOSTRAR_HORA;
                }
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    steTimeOut(controlador,30);
                    controlador->estado = E_MOD_ALARMA_HOR;
                }
            break;case E_MOD_ALARMA_HOR:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarHoras(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarHoras(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_MOSTRAR_HORA;
                }   
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    steTimeOut(controlador,30);
                    setAlarmaHora(controlador->reloj,controlador->temp);
                    setAlarmaEstado(controlador->reloj,READY);
                    controlador->estado = E_MOSTRAR_HORA;
                }                    
            break;case E_MOD_HORARIO_MIN:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarMinutos(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarMinutos(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_MOSTRAR_HORA;
                }
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    steTimeOut(controlador,30);
                    controlador->estado = E_MOD_HORARIO_HOR;
                }           
            break;case E_MOD_HORARIO_HOR:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarHoras(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarHoras(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_MOSTRAR_HORA;
                }   
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    relojGuardarHora(controlador->reloj,controlador->temp);
                    controlador->estado = E_MOSTRAR_HORA;
                }              
            break;case E_ESPERA_MOD_ALARMA_R:
                if(!isHighF(controlador->poncho,3)) controlador->estado = E_RESET;
                if(!controlador->TimeOut) {
                    steTimeOut(controlador,30);
                    getAlarmaHora(controlador->reloj,controlador->temp);
                    controlador->estado = E_MOD_ALARMA_MIN_R;
                }
            break;case E_ESPERA_MOD_HORARIO_R:
                if(!isHighF(controlador->poncho,2)) controlador->estado = E_RESET;
                if(!controlador->TimeOut) {
                    steTimeOut(controlador,30);
                    relojHorario(controlador->reloj,controlador->temp);
                    controlador->estado = E_MOD_HORARIO_MIN_R;
                }
            break;case E_MOD_ALARMA_MIN_R:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarMinutos(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarMinutos(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_RESET;
                }
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    steTimeOut(controlador,30);
                    controlador->estado = E_MOD_ALARMA_HOR_R;
                }
            break;case E_MOD_ALARMA_HOR_R:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarHoras(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarHoras(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_RESET;
                }   
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    steTimeOut(controlador,30);
                    setAlarmaHora(controlador->reloj,controlador->temp);
                    setAlarmaEstado(controlador->reloj,READY);
                    controlador->estado = E_MOSTRAR_HORA;
                }                    
            break;case E_MOD_HORARIO_MIN_R:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarMinutos(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarMinutos(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_RESET;
                }
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    steTimeOut(controlador,30);
                    controlador->estado = E_MOD_HORARIO_HOR_R;
                }           
            break;case E_MOD_HORARIO_HOR_R:
                if(PonchoBotonFuncion(controlador->poncho,3)){
                    steTimeOut(controlador,30);
                    incrementarHoras(controlador->temp);
                }
                if(PonchoBotonFuncion(controlador->poncho,4)){
                    steTimeOut(controlador,30);
                    decrementarHoras(controlador->temp);
                }
                if(PonchoBotonCancelar(controlador->poncho) || !controlador->TimeOut){
                    controlador->estado = E_RESET;
                }   
                if(PonchoBotonFuncion(controlador->poncho,1)){
                    relojGuardarHora(controlador->reloj,controlador->temp);
                    controlador->estado = E_MOSTRAR_HORA;
                }              
            break;default:
            break;
        }    
}

void timeOutCheck(Control * controlador){
    if(controlador->TimeOut>0) controlador->TimeOut--;
}

void segRefParpadeo(void){
    Parpadeo = !Parpadeo;
}

Poncho_p ponchoDe(Control * controlador){
    return controlador->poncho;
}