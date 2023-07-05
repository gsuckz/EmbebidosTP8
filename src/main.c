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
#include "control.h"
#include <stdbool.h>
/* === Macros definitions ====================================================================== */
/* === Private data type declarations ========================================================== */
/* === Private variable declarations =========================================================== */
/* === Private function declarations =========================================================== */
/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

static Poncho_p poncho;
static Reloj *  reloj;
/* === Private function implementation ========================================================= */
static void ControladorAlarma(bool estado){
    PonchoBuzzer(poncho,estado);
    return;
}
/* === Public function implementation ========================================================= */

int main(void) {
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / (CANTIDAD_TICKS_POR_SEGUNDO));
    poncho = PonchoInit();
    reloj = relojCrear(CANTIDAD_TICKS_POR_SEGUNDO, ControladorAlarma);
    ESTADOS estado = E_RESET, volver = E_RESET;
    uint8_t temp[6] = {0,0, 0,0 ,0,0};
    tickConfig(reloj);    
    while (1){ ///LAZO PRINCIPAL 
        checkBotones(poncho,reloj,&estado,&volver,temp);         
        mostrarEnPantalla(poncho,reloj,estado,temp);  
    }
}
/* === End of documentation ==================================================================== */
/** @} End of module definition for doxygen */