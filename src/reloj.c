#include "reloj.h"
#include <stddef.h>
#include <stdlib.h>

struct Reloj{
    uint8_t hora[6];
    uint8_t alarma[4];
    int ticks;
    int ticksMAX;
    EstadoAlarma estadoAlarma;
    void (*crtlAlarm)(bool);
    int snooze;
};



Reloj * relojCrear(int ticks, void (*ctrlAlarm)(bool)){
    Reloj * reloj = malloc(sizeof(struct Reloj));
    reloj->hora[0]= 0xF;
    reloj->crtlAlarm = ctrlAlarm;
    reloj->ticksMAX = ticks;
    reloj->estadoAlarma = OFF;
    reloj->snooze = 0;
    for (int i=0;i<4;i++) reloj->alarma[i]=0;
    return reloj;
}

void relojKill(Reloj * reloj){
    free(reloj);
}

/**
 * @brief Determina si la hora en formato BCD sin compactar es válida
 * (una hora es válida si la hora está entre 0 y 23, minutos y segundos
 * entre 0 y 59 todos los extremos incluidos)
 * @param hora 
 * @return true Hora válida
 * @return false Hora inválida
 */
static bool horaEsValida(const uint8_t hora[6]){
    const bool horaValida =    hora[DECENA_HORA]<=2 
                            && hora[UNIDAD_HORA]<=9 
                            && (hora[DECENA_HORA]<2 || hora[UNIDAD_HORA]<=3);

    const bool minutoValido = hora[DECENA_MINUTO]<6 && hora[UNIDAD_MINUTO]<10;

    const bool segundoValido = hora[DECENA_SEGUNDO]<6 && hora[UNIDAD_SEGUNDO]<10;    
    
    return horaValida && minutoValido && segundoValido;
}

bool relojGuardarHora(Reloj * reloj,const uint8_t hora[6]){
    const bool horaValida = horaEsValida(hora);
    if(horaValida){
        for (uint8_t i=0;i<6;i++) reloj->hora[i] = hora[i];
    }
    return horaValida;
}
bool relojHorario(Reloj * reloj, uint8_t hora[6]){
    bool RelojConHoraValida = horaEsValida(reloj->hora);
    if(RelojConHoraValida){
        for (uint8_t i=0;i<6;i++) hora[i] = reloj->hora[i];
    }
    return RelojConHoraValida;
}

static void borraDigitosHasta(Reloj * const reloj,const enum Digitos posicion){
    for (int i = posicion; i<6; i++) reloj->hora[i] = 0;
}
static void incDigito(Reloj * const reloj, const enum Digitos posicion){
    reloj->hora[posicion]++;
    borraDigitosHasta(reloj, posicion + 1);
}
static void nuevoDia(Reloj * reloj){
    borraDigitosHasta(reloj, DECENA_HORA);
}

static bool digitoNoRebalsa(Reloj const * const reloj,const enum Digitos posicion){
    bool noRebalsa = false;
    const int digito = reloj->hora[posicion];
    switch (posicion){
    case UNIDAD_SEGUNDO:
    case UNIDAD_MINUTO: //FALLTHRU
        noRebalsa = digito < 9;
    break;case DECENA_SEGUNDO:
          case DECENA_MINUTO: //FALLTHRU
        noRebalsa = digito < 5;
    break;case UNIDAD_HORA:
        if (reloj->hora[DECENA_HORA]<2){
            noRebalsa = digito < 9;
        }else{
            noRebalsa = digito < 3;
        }
    break;case DECENA_HORA:
        noRebalsa = digito < 2;
    break;default:
        false;
    break;
    }
    return noRebalsa;
}

void checkAlarma(Reloj * reloj){    
    bool Iguales = 1;
    if (reloj->estadoAlarma != READY) return;
    for (int i = 0; i<4; i++) {
        if (reloj->alarma[i]  != reloj->hora[i]){
            Iguales = 0;
        }
    }
    if (Iguales) {
        setAlarmaEstado(reloj,ON);
    }
}

void checkSnooze(Reloj * reloj){
    if (reloj->estadoAlarma!= SNOOZE) return;


    if (0 == reloj->snooze - 1){
        setAlarmaEstado(reloj,ON);
    } else{
        reloj->snooze--;
    }   
}

static void relojTickSegundo(Reloj * reloj){
    if (reloj->hora[DECENA_HORA] > 9 ) return; //Hora no asignada
        checkSnooze(reloj);
    if (digitoNoRebalsa(reloj,UNIDAD_SEGUNDO)){
        incDigito(reloj,UNIDAD_SEGUNDO);
    }else if(digitoNoRebalsa(reloj,DECENA_SEGUNDO)){
        incDigito(reloj,DECENA_SEGUNDO);
    }else if(digitoNoRebalsa(reloj,UNIDAD_MINUTO)){
        incDigito(reloj,UNIDAD_MINUTO);
        checkAlarma(reloj);
    } else if (digitoNoRebalsa(reloj,DECENA_MINUTO)){
        incDigito(reloj,DECENA_MINUTO);
        checkAlarma(reloj);
    }else if(digitoNoRebalsa(reloj,UNIDAD_HORA)){
        incDigito(reloj,UNIDAD_HORA);
        checkAlarma(reloj);
    } else if(digitoNoRebalsa(reloj,DECENA_HORA)){
        incDigito(reloj,DECENA_HORA);  
        checkAlarma(reloj);
    } else {
        nuevoDia(reloj);
        checkAlarma(reloj);
    }
}




bool relojTick(Reloj * const reloj){
    bool incrementaSegundo = 0;
    if (reloj->ticksMAX <= reloj->ticks ){
        reloj->ticks = 0;
        relojTickSegundo(reloj);
        incrementaSegundo = 1;
    }else{
        reloj->ticks++;
    }
    return incrementaSegundo;
}

EstadoAlarma getEstadoAlarma(Reloj * reloj){
    return reloj->estadoAlarma;
}

bool setAlarmaEstado(Reloj * reloj, EstadoAlarma estadoAlarma){
    if (reloj->hora[0] > 2 ) {
        return 0;
    }else{
    reloj->estadoAlarma = estadoAlarma;
    switch (estadoAlarma){
        case READY:  //FALLTHRU
        case OFF:  //FALLTHRU
        case SNOOZE:  //FALLTHRU
            reloj->crtlAlarm(0);       
        break; case ON:
            reloj->crtlAlarm(1);   
        break; default:
        break;          
    }
    return 1;}
}

void getAlarmaHora(Reloj * reloj, uint8_t Alarma[4]){
    for (int i=0; i<4; i++) Alarma[i] = reloj->alarma[i];
}


bool setAlarmaHora(Reloj * reloj, uint8_t Alarma[4]){
    //if (reloj->hora[0] > 9) return 0; El enunciado del TP8 no prohibe configurar la alarma sin configurar la hora
    for (int i=0; i<4; i++)  reloj->alarma[i] =Alarma[i];
        setAlarmaEstado(reloj,READY);
    return 1;
}

void relojApagarAlarma(Reloj * reloj){
    setAlarmaEstado(reloj,READY);
}


void relojSnooze(Reloj * reloj, uint8_t minutos){
    reloj->snooze += minutos*60;
    setAlarmaEstado (reloj,SNOOZE);
}