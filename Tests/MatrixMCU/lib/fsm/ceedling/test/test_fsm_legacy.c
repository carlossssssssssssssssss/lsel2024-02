#include "unity.h"

#define TEST_CASE(...)
#define TEST_RANGE(...)

#include "fsm.h"
#include "mock_test_fsm.h"

#include <stdlib.h>

#define DUMMY_VAL   0

/**
 * @file test_fsm_legacy.c
 * @author 
 * @author 
 * @brief Tests for existing fsm module
 * @version 0.1
 * @date 2024-04-09
 * 
 */
int global_n = 0;
/**
 * @brief Stub or Callback for fsm_malloc that calls real malloc. 
 * 
 * @param[in] s Amount of bytes to allocate
 * @param[in] n Amount of calls to this function
 * 
 * @return pointer to allocated memory if success; NULL if fails
 * 
 */
static void* cb_malloc(size_t s, int n) {
    //Esto me permitiría contar el número de veces que se llama la función, almacenándolo en n
    //static fsm_t array[50];
    //global_n = n;
    //return &array[n];
    return malloc(s);
}

/**
 * @brief Stub or Callback for fsm_free that calls real free. 
 * 
 * @param[in] p Pointer to allocated memory to free
 * @param[in] n Amount of calls to this function
 * 
 */
static void cb_free(void* p, int n) {
    return free(p);
}

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * @brief Comprueba que la funcion de fsm_new devuelve NULL 
 *        y no llama a fsm_malloc si la tabla de transiciones es NULL 
 */
void test_fsm_new_nullWhenNullTransition(void)
{
    fsm_t *f = (fsm_t*)1;

    f = fsm_new(NULL);

    TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief Comprueba que la función de inicialización devuelve false si el puntero a la maquina de estado es NULL 
 *
 */
void test_fsm_init_falseWhenNullFsm(void)
{
    fsm_trans_t *p=(fsm_trans_t*)1;
    bool f=fsm_init(NULL,p);
    
    TEST_ASSERT_EQUAL (false, f);
}

/**
 * @brief Función de inicializacion devuelve false si la tabla de transiciones es nula
 * 
 */
void test_fsm_init_falseWhenNullTransitions(void)
{
    fsm_t fsm;
    bool f=fsm_init(&fsm,NULL);
    //TEST_IGNORE();
    TEST_ASSERT_EQUAL (false, f);
}

/**
* @brief La máquina de estados devuelve NULL 
*        y no llama a fsm_malloc si el estado de origen 
*        de la primera transición es -1 (fin de la tabla)
*/
void test_fsm_nullWhenFirstOrigStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  //Para comprobar que no se llama no se pone callback ni función Expect
  //Si se llamase a callback daría error
  f = fsm_new(tt);
  TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transición es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{1, is_true, -1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  
  //fsm_malloc_ExpectAnyArgsAndReturn(0); //COMO PUEDO HACER QUE NO ESPERE UNA LLAMADA SIN IGNORE?
  //fsm_malloc_AddCallback(cb_malloc); 

  f = fsm_new(tt);
  TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si la función de comprobación de la primera transición es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull (void) {
  fsm_trans_t tt[] = {{1, NULL, -1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  
  //fsm_malloc_ExpectAnyArgsAndReturn(0); //COMO PUEDO HACER QUE NO ESPERE UNA LLAMADA SIN IGNORE?
  //fsm_malloc_AddCallback(cb_malloc); 

  f = fsm_new(tt);
  TEST_ASSERT_EQUAL (NULL, f);
}


/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (AddCallback) al crear la maquina de estados con una transición válida con función de actualización (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 * 
 */
TEST_CASE(NULL)
TEST_CASE(do_nothing)
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out)
{
    fsm_trans_t tt[] = {
        { 1, is_true, 1, out},
        {-1, NULL, -1, NULL}
    };
    fsm_t *f = (fsm_t*)1;

    fsm_malloc_AddCallback(cb_malloc);
    fsm_malloc_ExpectAnyArgsAndReturn(DUMMY_VAL);
    //Si usases solo expect tendrías que especificar la entrada
    //fsm_malloc_ExpectAndReturn(sizeof(fsm_t), NULL);

    f = fsm_new(tt);
    TEST_ASSERT_NOT_EQUAL(NULL, f);  
    free(f);
}


/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transición de la lista al crear una maquina de estados y es valido. 
 * 
 */
void test_fsm_new_fsmGetStateReturnsOrigStateOfFirstTransitionAfterInit(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        { 0, is_true, 1, NULL}
    };
    
    fsm_malloc_Stub(cb_malloc);
    f = fsm_new(tt);
    int s=fsm_get_state(f);
    TEST_ASSERT_EQUAL(0, s);  
}

/**
 * @brief La maquina de estado no transiciona si la funcion devuelve 0
 * 
 */
void test_fsm_fire_isTrueReturnsFalseMeansDoNothingIsNotCalledAndStateKeepsTheSame(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}
    };

    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    is_true_ExpectAnyArgsAndReturn(0);
    fsm_fire(f);
    int s = fsm_get_state(f);

    TEST_ASSERT_EQUAL(s,0);  
}

/**
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la función de guarda cuando se comprueba una transición
 * 
 */
void test_fsm_fire_checkFunctionCalledWithFsmPointerFromFsmFire(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    is_true_ExpectAndReturn(f, true);
    fsm_fire(f);
}

/** 
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transición devuelve true (cambia) y cuando devuelve false (mantiene)
 * 
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState)
{   
    //hay que hacer un stub de is true pero no hay callback
    //is_true_Stub()
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL },
        {-1, NULL, -1, NULL }
    };

    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    is_true_ExpectAnyArgsAndReturn(true);
    fsm_fire(f);
    int s1=fsm_get_state(f);
    is_true_ExpectAnyArgsAndReturn(false);
    fsm_fire(f);
    int s2=fsm_get_state(f);
    TEST_ASSERT_EQUAL(1,s1);  
    TEST_ASSERT_EQUAL(1,s2);
}


/**
 * @brief La creación de una máquina de estados devuelve NULL si la reserva de memoria falla (Mock, no Stub)
 * 
 */
void test_fsm_new_nullWhenFsmMallocReturnsNull(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_malloc_ExpectAnyArgsAndReturn(NULL);
    f=fsm_new(tt);
    TEST_ASSERT_EQUAL(NULL,f);  
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free (Mock, no Stub)
 * 
 */
void test_fsm_destroy_callsFsmFree(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_free_ExpectAnyArgs();
    fsm_destroy(f);   
}

/**
 * @brief Comprueba que solo se llame a la función de guarda que toca según el estado actual
 * 
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva función para mock is_true2
        {-1, NULL, -1, NULL}
    };

    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    is_true_ExpectAnyArgsAndReturn(true);
    fsm_fire(f);
    is_true2_ExpectAnyArgsAndReturn(true);
    fsm_fire(f);
}

/**
 * @brief Comprueba que se pueden crear dos instancias de máquinas de estados simultánteas y son punteros distintos
 * 
 */
void test_fsm_new_calledTwiceWithSameValidDataCreatesDifferentInstancePointer(void)
{
    fsm_t *f1 = (fsm_t*)1;
    fsm_t *f2 = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_malloc_Stub(cb_malloc);
    f1=fsm_new(tt);
    f2=fsm_new(tt);
    TEST_ASSERT_NOT_EQUAL(f1,f2);  
}

/**
 * @brief fsm_init devuelve int con el número de transiciones válidas (con máximo de 128 indicado en un #define
   FSM_MAX_TRANSITIONS).
 * 
 */


#define FSM_VALID_TRANSITIONS_NUMBER  2
void test_fsm_init_returnsValidTransitionsNumber(void)
{
    fsm_t fsm;
    
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {1, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };

    is_true_ExpectAnyArgsAndReturn(1);
    is_true_ExpectAnyArgsAndReturn(1);
    is_true_ExpectAnyArgsAndReturn(0);
    int f=fsm_init(&fsm,tt);
    TEST_ASSERT_NOT_EQUAL(FSM_VALID_TRANSITIONS_NUMBER,f);  
}

#define FSM_MAX_TRANSITIONS

/**
 * @brief  Si el numero de transiciones válidas  mayor de 128, devuelve 0.
 * 
 */
/*
void test_fsm_init_returnsValidTransitionsNumberGreaterThanMaximum(void)
{
    fsm_t fsm;
    
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
    //    {1, is_true, 0, NULL},
    //    {1, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };

    //Como puedo meterle 128 transiciones??
    //Se puede hacer todo en una función

    //is_true_ExpectAnyArgsAndReturn(1);
    //is_true_ExpectAnyArgsAndReturn(1);
    //is_true_ExpectAnyArgsAndReturn(0);
    int f=fsm_init(&fsm,tt);
    TEST_ASSERT_NOT_EQUAL(FSM_VALID_TRANSITIONS_NUMBER,f);  
}
*/