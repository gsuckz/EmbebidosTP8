#include "mybsp.h"

#define NUM_DISPLAY 4

static void ctrl_digito(uint8_t dig, bool estado);
static void ctrl_segmento(uint8_t seg, bool estado);
typedef struct Poncho {
    Display * display;
    Pin disp_digito[4];
    Pin disp_segmentos[7];
    Pin ACEPTAR;
    Pin CANCELAR;
    Pin F[4];
    Pin BUZZER;
} Poncho;

static Poncho poncho = {.disp_digito = {[0] = {.puerto = DIGIT_1_PORT, .pin = DIGIT_1_PIN},
                                        [1] = {.puerto = DIGIT_2_PORT, .pin = DIGIT_2_PIN},
                                        [2] = {.puerto = DIGIT_3_PORT, .pin = DIGIT_3_PIN},
                                        [3] = {.puerto = DIGIT_4_PORT, .pin = DIGIT_4_PIN}},

                        .disp_segmentos =
                            {
                                [0] = {.puerto = SEGMENT_A_PORT, .pin = SEGMENT_A_PIN},
                                [1] = {.puerto = SEGMENT_B_PORT, .pin = SEGMENT_B_PIN},
                                [2] = {.puerto = SEGMENT_C_PORT, .pin = SEGMENT_C_PIN},
                                [3] = {.puerto = SEGMENT_D_PORT, .pin = SEGMENT_D_PIN},
                                [4] = {.puerto = SEGMENT_E_PORT, .pin = SEGMENT_E_PIN},
                                [5] = {.puerto = SEGMENT_F_PORT, .pin = SEGMENT_F_PIN},
                                [6] = {.puerto = SEGMENT_G_PORT, .pin = SEGMENT_G_PIN},
                            },

                        .ACEPTAR = {.puerto = KEY_ACCEPT_PORT, .pin = KEY_ACCEPT_PIN},

                        .CANCELAR = {.puerto = KEY_CANCEL_PORT, .pin = KEY_CANCEL_PIN},

                        .F = {[0] = {.puerto = KEY_F1_PORT, .pin = KEY_F1_PIN},
                              [1] = {.puerto = KEY_F2_PORT, .pin = KEY_F2_PIN},
                              [2] = {.puerto = KEY_F3_PORT, .pin = KEY_F3_PIN},
                              [3] = {.puerto = KEY_F4_PORT, .pin = KEY_F4_PIN}}

};

Poncho * PonchoInit(void) {
    configPin(&poncho.CANCELAR, ENTRADA);
    configPin(&poncho.ACEPTAR, ENTRADA);
    configPin(&poncho.BUZZER, ENTRADA);
    for (uint8_t i = 0; i <= 3; i++) {
        configPin(&poncho.F[i], ENTRADA);
    }
    for (uint8_t i = 0; i <= 3; i++) {
        configPin(&poncho.disp_digito[i], SALIDA);
    }
    for (uint8_t i = 0; i <= 6; i++) {
        configPin(&poncho.disp_segmentos[i], SALIDA);
    }
    poncho.display = displayInit(ctrl_segmento, ctrl_digito, NUM_DISPLAY);
    return &poncho;
}
static void ctrl_segmento(uint8_t seg, bool estado) {
    writePin(&(poncho.disp_segmentos[seg]), estado);
}
static void ctrl_digito(uint8_t dig, bool estado) {
    writePin(&(poncho.disp_digito[dig]), estado);
}

void PonchoWriteDisplay(Poncho * poncho, unsigned numero) {
    if (!poncho)
        return  ;
    writeDisplay(poncho->display, numero);
}

bool PonchoBotonCancelar(Poncho * poncho) {
    if (!poncho)
        return 0;
    static bool estado;
    bool estadon = readPin(&poncho->CANCELAR);

    if (estado && !estadon) {
        estado = estadon;
        return 1;
    }
    estado = estadon;
    return 0;
}

bool PonchoBotonAceptar(Poncho * poncho) {
    if (!poncho)
        return 0;
    static bool estado;
    bool estadon = readPin(&poncho->ACEPTAR);

    if (estado && !estadon) {
        estado = estadon;
        return 1;
    }
    estado = estadon;
    return 0;
}

bool PonchoBotonFuncion(Poncho * poncho, uint8_t i) {
    if (!poncho || i > 4)
        return 0;
    static bool estado[4];
    i--;
    bool estadon = readPin(&poncho->F[i]);

    if (estado[i] && !estadon) {
        estado[i] = estadon;
        return 1;
    }
    estado[i] = estadon;
    return 0;
}

void PonchoDrawDisplay(Poncho * poncho) {
    if (!poncho)
        return;
    drawDisplay(poncho->display);
}

void PonchoBuzzer(Poncho * poncho, bool estado) {
    if (!poncho)
        return;
    writePin(&poncho->BUZZER, estado);
}


bool isHighF(Poncho_p poncho, uint8_t funcion){
    return readPin(&poncho->F[funcion]);
}