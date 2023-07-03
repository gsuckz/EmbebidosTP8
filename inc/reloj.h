#ifndef RELOJ_H
#define RELOJ_H
#include <stdint.h>
#include <stdbool.h>

enum Digitos {
    DECENA_HORA,
    UNIDAD_HORA,
    DECENA_MINUTO,
    UNIDAD_MINUTO,
    DECENA_SEGUNDO,
    UNIDAD_SEGUNDO
};

typedef struct Reloj Reloj;


// OFF:    Alarma Apagada y Desactivada 
// READY:  Alarma Apagada y Activada
// ON:     Alarma Encendida y Activada
// SNOOZE: Alarma Pospuesta y Activada
typedef enum EstadoAlarma {
        OFF,     // Alarma Apagada y Desactivada 
        READY,   // Alarma Apagada y Activada
        ON,      // Alarma Encendida y Activada
        SNOOZE   // Alarma Pospuesta y Activada

}EstadoAlarma;
//NOTA: Si el horario de la alarma llega mientras se encuentra otra alarma esta pospuesta, se ignora la nueva alarma
//      y solo sonará cuando termina el tiempo de Snooze. 
//NOTA: Modificar la alarma durante el ((Snooze)) la lleva al estado ((READY))
//NOTA: Modificar la alarma mientras suena, la lleva al estado ((READY))


/**
 * @brief Crea un objeto Reloj
 * 
 * @param ticks Numero de ticks para incrementar el segundero
 * @param ctrlAlarm Funcion para controlar la alarma
 * @return Reloj* 
 */
Reloj * relojCrear(int ticks, void (*ctrlAlarm)(bool));
/**
 * @brief Libera la memoria reservada para el objeto reloj
 * 
 * @param reloj 
 */
void relojKill(Reloj * reloj);
/**
 * @brief Guarda la hora en el reloj
 * 
 * @param reloj 
 * @param hora Valor de hora en formato BCD sin compactar
 * {<decena de hora>,<unidad de hora>,<d. de minuto>,<u. de minuto>,<d. de segundo>,<u. de segundo>}
 * @return true si se ingresó una hora valida
 * @return false si no se ingresó una hora valida, ademas no tiene efectos secundarios
 */
bool relojGuardarHora(Reloj * reloj,const uint8_t hora[6]);
/**
 * @brief Recupera la hora del reloj. 
 * 
 * 
 * @param reloj
 * @param hora Luego del llamado contiene la hora actual del reloj en formato BCD sin compactar
 * {<decena de hora>,<unidad de hora>,<d. de minuto>,<u. de minuto>,<d. de segundo>,<u. de segundo>}
 * @return true Hora contiene la hora del reloj
 * @return false No fue asignada una hora correcta
 */
bool relojHorario(Reloj * reloj, uint8_t hora[6]);
/**
 * @brief Incrementa los segundos al ser llamada el numero
 * configurado de veces
 * 
 * @param reloj 
 */
void relojTick(Reloj * reloj);
/**
 * @brief Avisa si la alarma esta sonando o no
 * 
 * @return true Si esta sonando     (Estado ON)
 * @return false Si no esta sonando (Estados: OFF,READY,SNOOZE)
 */
bool alarmaSonando(Reloj * reloj);
/**
 * @brief Get the Estado Alarma object
 * 
 * @param reloj 
 * @param alarma Arreglo donde guardarla hora de la alarma
 * @return EstadoAlarma 
 */
EstadoAlarma getEstadoAlarma(Reloj * reloj);
/**
 * @brief Set the Alarma Hora object
 * 
 * @param reloj 
 * @param Alarma Array con la hora de la alarm. Formato: [HH:MM]
 *@returns true si pudo ajustar la alrama
 *@return false si no pudo ajustar la alarma
 */
bool setAlarmaHora(Reloj * reloj, uint8_t Alarma[4]);
/**
 * @brief Set the Alarma Hora object
 * 
 * @param reloj 
 * @param Alarma Array para guardar la hora de la alarm. Formato: [HH:MM]
 */
void getAlarmaHora(Reloj * reloj, uint8_t Alarma[4]);
/**
 * @brief Set the Alarma Estado object
 * 
 * @param reloj 
 * @param estadoAlarma 
 * @return true Si pudo fijar el estado deseado
 * @return false Si no pudo fijar el estado indicado (Sin efecto)
 */
bool setAlarmaEstado(Reloj * reloj, EstadoAlarma estadoAlarma);
/**
 * @brief Pone la alarma en estado READY : 
 * Alarma sin sonar y  activada
 * @param reloj 
 * 
 */
void relojApagarAlarma(Reloj * reloj);
/**
 * @brief Pospone la alarma el numero de minutos indicado
 * 
 * @param reloj 
 * @param minutos 
 */
void relojSnooze(Reloj * reloj, uint8_t minutos);

#endif

