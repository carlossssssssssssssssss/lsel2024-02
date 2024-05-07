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
  f = fsm_new(tt);
  TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transición es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne (void) {
  fsm_trans_t tt[] = {
    {1, is_true, -1, do_nothing},
    {-1, NULL, -1, NULL}
  };
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
  TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si la función de comprobación de la primera transición es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull (void) {
  fsm_trans_t tt[] = {
    {1, NULL, -1, do_nothing},
    {-1, NULL, -1, NULL}
    };
  fsm_t *f = (fsm_t*)1;
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
        { 0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
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
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL }
    };

    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    fsm_set_state(f,0);
    is_true_ExpectAnyArgsAndReturn(returnValue);
    fsm_fire(f);
    int actual_state=fsm_get_state(f);
    TEST_ASSERT_EQUAL(expectedState,actual_state);  
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



TEST_CASE(2,2)
TEST_CASE(FSM_MAX_TRANSITIONS,FSM_MAX_TRANSITIONS)
TEST_CASE(129,0)

void test_fsm_init_returnsValidTransitionsNumber(int transitions,int returned_transitions)
{
    fsm_t fsm;
    
    fsm_trans_t tt[transitions+1];

    for(int i=0;i<(transitions);i++){
        tt[i].dest_state=0;
        tt[i].in=is_true;
        tt[i].orig_state=1;
        tt[i].out=NULL;
    }

    tt[transitions].dest_state=-1;
    tt[transitions].in=NULL;
    tt[transitions].orig_state=-1;
    tt[transitions].out=NULL;
    
    for(int i=0;i<transitions;++i){
        is_true_ExpectAnyArgsAndReturn(true);   
    }

    int valid_transitions=fsm_init(&fsm,tt);
    TEST_ASSERT_EQUAL(returned_transitions,valid_transitions);  
}

/**
 * @brief  Una funcion de guarda NULL en una transición equivale a que se cumpla siempre (devuelve true) 
 * 
 */
void test_fsm_fire_NullGuardFunctionIsEqualToTrue(void)
{
    fsm_t fsm;
    fsm_trans_t tt[] = {
        {0,  NULL,  1, do_nothing},
        {1,  NULL,  0, NULL},
        {-1, NULL, -1, NULL}
    };

    int n=fsm_init(&fsm,tt);
    fsm_set_state(&fsm,0);
    do_nothing_ExpectAnyArgs();
    fsm_fire(&fsm);   
    int state1=fsm_get_state(&fsm);
    TEST_ASSERT_EQUAL(1,state1); 
    fsm_fire(&fsm);   
    int state0=fsm_get_state(&fsm); 
    TEST_ASSERT_EQUAL(0,state0); 
}


/**
 * @brief  fsm_fire devuelve -1 si no hay ninguna transición para el estado actual 
 * 
 */
void test_fsm_fire_returnsNegativeOneIfCurrentStateHasNoTransition(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {1, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };
    //is_true_ExpectAnyArgsAndReturn(1);
    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    fsm_set_state(f,0);
    int n=fsm_fire(f);
    TEST_ASSERT_EQUAL(-1,n);  
}

/**
 * @brief  fsm_fire devuelve 0 si hay ninguna transición para el estado actual pero la función de guarda devuelve 0
 * 
 */
void test_fsm_fire_returnsZeroIfCurrentStateHasTransitionsButGuardIsFalse(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    is_true_ExpectAnyArgsAndReturn(0);
    fsm_malloc_Stub(cb_malloc);
    f=fsm_new(tt);
    int n=fsm_fire(f);
    TEST_ASSERT_EQUAL(0,n); 
}

/**
 * @brief  fsm_fire devuelve 1 si hay alguna transición para el estado actual y la función de guarda devuelve 1
 * 
 */
void test_fsm_fire_returnsOneIfCurrentStateHasTransitionsAndGuardIsTrue(void)
{
    fsm_t fsm;
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {1, is_true, 2, NULL},
        {-1, NULL, -1, NULL}
    };
    is_true_ExpectAnyArgsAndReturn(1);
    is_true_ExpectAnyArgsAndReturn(1);
    is_true_ExpectAnyArgsAndReturn(1);
    do_nothing_ExpectAnyArgs();
    is_true_ExpectAnyArgsAndReturn(1);

    fsm_init(&fsm,tt);
    int n1=fsm_fire(&fsm);
    TEST_ASSERT_EQUAL(1,n1); 
    int n2=fsm_fire(&fsm);
    TEST_ASSERT_EQUAL(1,n2); 
}

/**
 * @brief Llamada a fsm_destroy llama a fsm_free solo si el puntero pasado no es NULL
 * 
 */
void test_fsm_destroy_onlyCallsFsmFreeIfPointerIsNotNULL(void)
{
    fsm_t *f=NULL;
    fsm_destroy(f);
    //Hay que comprobar tb el caso en que el puntero no sea nulo????
}

/**
* @brief fsm_init devuelve 0 si el estado de origen o el de salida es -1
*/
TEST_CASE(-1,0)
TEST_CASE(0,-1)

TEST_CASE(-1,-1)
void test_fsm_initReturnsZeroIfOrigStateOrDestStateAreInvalid (int orig_state, int dest_state) {
  fsm_trans_t tt[] = {
    {orig_state, NULL, dest_state, NULL},
    {-1, NULL, -1, NULL}
    };
  fsm_t *f = (fsm_t*)1;
  int n = fsm_init(f,tt);
  TEST_ASSERT_EQUAL (0, n);
}

/**
* @brief fsm_init devuelve 0 si las todas las funciones de guarda devuelven 0
*/
void test_fsm_initReturnsZeroIfAllGuardsAreInvalid (void) {
  fsm_trans_t tt[] = {
    {0, is_true, 1, NULL},
    {0, is_true, 1, do_nothing},
    {-1, NULL, -1, NULL}
    };

  is_true_ExpectAnyArgsAndReturn(0);
  is_true_ExpectAnyArgsAndReturn(0);
  fsm_t *f = (fsm_t*)1;
  int n = fsm_init(f,tt);
  TEST_ASSERT_EQUAL (0, n);
}