#include <stdint.h>
#include "ajuste.h"

enum Digitos {
    DECENA_HORA,
    UNIDAD_HORA,
    DECENA_MINUTO,
    UNIDAD_MINUTO,
    DECENA_SEGUNDO,
    UNIDAD_SEGUNDO
};

static void borraDigitosHasta(uint8_t temp[],const enum Digitos posicion){
    for (int i = posicion; i<4; i++) temp[i] = 0;
}
static void incDigito(uint8_t temp[], const enum Digitos posicion){
    temp[posicion]++;
    borraDigitosHasta(temp, posicion + 1);
}
static void nuevoDia(uint8_t temp[]){
    borraDigitosHasta(temp, DECENA_HORA);
}

static bool digitoNoRebalsa(uint8_t temp[],const enum Digitos posicion){
    bool noRebalsa = false;
    const int digito = temp[posicion];
    switch (posicion){
    case UNIDAD_SEGUNDO:
    case UNIDAD_MINUTO: //FALLTHRU
        noRebalsa = digito < 9;
    break;case DECENA_SEGUNDO:
          case DECENA_MINUTO: //FALLTHRU
        noRebalsa = digito < 5;
    break;case UNIDAD_HORA:
        if (temp[DECENA_HORA]<2){
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

void incrementarMinutos(uint8_t  temp[]){
    if(digitoNoRebalsa(temp,UNIDAD_MINUTO)){
        incDigito(temp,UNIDAD_MINUTO);        
    } else if (digitoNoRebalsa(temp,DECENA_MINUTO)){
        incDigito(temp,DECENA_MINUTO);
    }else if(digitoNoRebalsa(temp,UNIDAD_HORA)){
        incDigito(temp,UNIDAD_HORA);
    } else if(digitoNoRebalsa(temp,DECENA_HORA)){
        incDigito(temp,DECENA_HORA);  
    } else {
        nuevoDia(temp);
    }
}

static int anteriorDeCero(uint8_t temp[],const enum Digitos posicion){
    int previo;
    //const int digito = temp[posicion];
    switch (posicion){
    case UNIDAD_MINUTO: //FALLTHRU
        previo = 9;
    break;case DECENA_MINUTO: //FALLTHRU
        previo = 5;
    break;case UNIDAD_HORA:
        if (temp[DECENA_HORA]>0){
            previo = 9;
        }else{
            previo = 3;
        }
    break;case DECENA_HORA:
        previo = 2;
    break;default:
        previo = 0;
    break;
    }
    return previo;
}

static bool decDigito(uint8_t temp[], const enum Digitos posicion){
    bool noDebeDecrementarSiguiente = 1;
    if(temp[posicion]){
        temp[posicion]--;
    }else{
        temp[posicion]=anteriorDeCero(temp,posicion);
        noDebeDecrementarSiguiente = 0;
    }
    return noDebeDecrementarSiguiente;
}

void decrementarMinutos(uint8_t  temp[]){
    if(decDigito(temp,UNIDAD_MINUTO)){
    }else if (decDigito(temp,DECENA_MINUTO)){
    }else if(decDigito(temp,UNIDAD_HORA)){
    }else {decDigito(temp,DECENA_HORA);}    
}

static void borraDigitosSoloHoras(uint8_t temp[],const enum Digitos posicion){
    for (int i = posicion; i<2; i++) temp[i] = 0;
}
static void incDigitoSoloHoras(uint8_t temp[], const enum Digitos posicion){
    temp[posicion]++;
    borraDigitosSoloHoras(temp, posicion + 1);
}

void incrementarHoras(uint8_t temp[]){
    if(digitoNoRebalsa(temp,UNIDAD_HORA)){
        incDigitoSoloHoras(temp,UNIDAD_HORA);
    } else if(digitoNoRebalsa(temp,DECENA_HORA)){
        incDigitoSoloHoras(temp,DECENA_HORA);  
    } else {
        borraDigitosSoloHoras(temp,DECENA_HORA);
    }
}

void decrementarHoras(uint8_t temp[]){
    if(decDigito(temp,UNIDAD_HORA)){
    }else {
        decDigito(temp,DECENA_HORA);}    
}