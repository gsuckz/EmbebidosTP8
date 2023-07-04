#include <unity.h>
#include <stddef.h>
#include "ajuste.h"
#include "stdio.h"


void setUp(void){

}

void tearDown(void){

}

void test_reloj_debe_incrementar_minutos_0008(void){
    const uint8_t HORA[6] = {0,0, 0,8, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,0, 0,9, 5,9};
    incrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_minutos_0009(void){
    const uint8_t HORA[6] = {0,0, 0,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,0, 1,0, 5,9};
    incrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_minutos_0059(void){
    const uint8_t HORA[6] = {0,0, 5,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,1, 0,0, 5,9};
    incrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_minutos_0959(void){
    const uint8_t HORA[6] = {0,9, 5,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {1,0, 0,0, 5,9};
    incrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_minutos_2359(void){
    const uint8_t HORA[6] = {2,3, 5,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,0, 0,0, 5,9};
    incrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}
void test_reloj_debe_decrementar_minutos_0008(void){
    const uint8_t HORA_ESPERADA[6] = {0,0, 0,8, 5,9};
    const uint8_t HORA[6] = {0,0, 0,9, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_minutos_0009(void){
    const uint8_t HORA_ESPERADA[6] = {0,0, 0,9, 5,9};
    const uint8_t HORA[6] = {0,0, 1,0, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_minutos_0059(void){
    const uint8_t HORA_ESPERADA[6] = {0,0, 5,9, 5,9};
    const uint8_t HORA[6] = {0,1, 0,0, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_minutos_0959(void){
    const uint8_t HORA_ESPERADA[6] = {0,9, 5,9, 5,9};
    const uint8_t HORA[6] = {1,0, 0,0, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_minutos_2359(void){
    const uint8_t HORA_ESPERADA[6] = {2,3, 5,9, 5,9};
    const uint8_t HORA[6] = {0,0, 0,0, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_minutos_2059(void){
    const uint8_t HORA_ESPERADA[6] = {1,9, 5,9, 5,9};
    const uint8_t HORA[6] = {2,0, 0,0, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_minutos_1059(void){
    const uint8_t HORA_ESPERADA[6] = {0,9, 5,9, 5,9};
    const uint8_t HORA[6] = {1,0, 0,0, 5,9};
    decrementarMinutos(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

///TEST HORAS///

void test_reloj_debe_incrementar_Horas_0000(void){
    const uint8_t HORA[6] = {0,0, 0,0, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,1, 0,0, 5,9};
    incrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_Horas_0900(void){
    const uint8_t HORA[6] = {0,9, 0,0, 5,9};
    const uint8_t HORA_ESPERADA[6] = {1,0, 0,0, 5,9};
    incrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_Horas_1900(void){
    const uint8_t HORA[6] = {1,9, 5,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {2,0, 5,9, 5,9};
    incrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_Horas_2300(void){
    const uint8_t HORA[6] = {2,3, 5,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,0, 5,9, 5,9};
    incrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_incrementar_Horas_2359(void){
    const uint8_t HORA[6] = {2,3, 5,9, 5,9};
    const uint8_t HORA_ESPERADA[6] = {0,0, 5,9, 5,9};
    incrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}


void test_reloj_debe_decrementar_Horas_0100(void){
    const uint8_t HORA_ESPERADA[6] = {0,0, 0,0, 5,9};
    const uint8_t HORA[6] = {0,1, 0,0, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_Horas_1000(void){
    const uint8_t HORA_ESPERADA[6] = {0,9, 0,0, 5,9};
    const uint8_t HORA[6] = {1,0, 0,0, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_Horas2359(void){
    const uint8_t HORA_ESPERADA[6] = {2,2, 5,9, 5,9};
    const uint8_t HORA[6] = {2,3, 5,9, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_Horas_1059(void){
    const uint8_t HORA_ESPERADA[6] = {0,9, 5,9, 5,9};
    const uint8_t HORA[6] = {1,0, 5,9, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_Horas_0059(void){
    const uint8_t HORA_ESPERADA[6] = {2,3, 5,9, 5,9};
    const uint8_t HORA[6] = {0,0, 5,9, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_Horas_2059(void){
    const uint8_t HORA_ESPERADA[6] = {1,9, 5,9, 5,9};
    const uint8_t HORA[6] = {2,0, 5,9, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}

void test_reloj_debe_decrementar_Horas_1040(void){
    const uint8_t HORA_ESPERADA[6] = {0,9, 4,0, 5,9};
    const uint8_t HORA[6] = {1,0, 4,0, 5,9};
    decrementarHoras(HORA);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(HORA_ESPERADA,HORA,6);
}