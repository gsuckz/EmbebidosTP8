#include "myhal.h"
#include "tablaGPIO.h"
#include <chip.h>
#include <stddef.h>

pinGPIO const * getPin(Pin const * pin) {
    if (!pin)
        return 0;
    pinGPIO const * valor = NULL;
    if (pin->puerto < MUX_NUM_PUERTOS && pin->pin < MUX_NUM_PINES)
        valor = &tablaConfigPinGpio[pin->puerto][pin->pin];
    return valor;
}

void configPin(Pin const * pin_p, HAL_ModoPin modo) {
    if (!pin_p)
        return;
    pinGPIO const * pin = getPin(pin_p);
    uint16_t modo_ = 64;
    if (modo)
        modo_ = 80;
    Chip_SCU_PinMuxSet(pin_p->puerto, pin_p->pin, modo_ | pin->funcion);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, pin->numRegistroGPIO, pin->bitRegistroGPIO, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, pin->numRegistroGPIO, pin->bitRegistroGPIO, modo);
}

bool readPin(Pin const * pin_p) {
    if (!pin_p)
        return 0;
    pinGPIO const * pin = getPin(pin_p);
    return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, pin->numRegistroGPIO, pin->bitRegistroGPIO);
}

void writePin(Pin const * pin_p, bool valor) {
    if (!pin_p)
        return;
    pinGPIO const * pin = getPin(pin_p);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, pin->numRegistroGPIO, pin->bitRegistroGPIO, valor);
    return;
}

void tooglePin(Pin const * pin_p) {
    if (!pin_p)
        return;
    pinGPIO const * pin = getPin(pin_p);
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, pin->numRegistroGPIO, pin->bitRegistroGPIO);
}