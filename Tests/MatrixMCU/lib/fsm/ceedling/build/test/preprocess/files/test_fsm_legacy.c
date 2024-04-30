#include "build/test/mocks/mock_test_fsm.h"
#include "../include/fsm.h"
#include "/var/lib/gems/3.0.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"


#define DUMMY_VAL 0



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

    //Esto me permitira contar el nmero de veces que se llama la funcin, almacenndolo en n

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

 * @brief Comprueba que la funcin de inicializacin devuelve false si el puntero a la maquina de estado es NULL 

 *

 */

void test_fsm_init_falseWhenNullFsm(void)

{

    fsm_trans_t *p=(fsm_trans_t*)1;

    bool f=fsm_init(NULL,p);

    

    TEST_ASSERT_EQUAL (false, f);

}



/**

 * @brief Funcin de inicializacion devuelve false si la tabla de transiciones es nula

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

* @brief La mquina de estados devuelve NULL 

*        y no llama a fsm_malloc si el estado de origen 

*        de la primera transicin es -1 (fin de la tabla)

*/

void test_fsm_nullWhenFirstOrigStateIsMinusOne (void) {

  fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};

  fsm_t *f = (fsm_t*)1;

  //Para comprobar que no se llama no se pone callback ni funcin Expect

  //Si se llamase a callback dara error

  f = fsm_new(tt);

  TEST_ASSERT_EQUAL (NULL, f);

}



/**

 * @brief La mquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transicin es -1 (fin de la tabla)

 * 

 */

void test_fsm_nullWhenFirstDstStateIsMinusOne (void) {

  fsm_trans_t tt[] = {

    {1, is_true, -1, do_nothing},

    {-1, NULL, -1, NULL}

  };

  fsm_t *f = (fsm_t*)1;

  

  //fsm_malloc_ExpectAnyArgsAndReturn(0); //COMO PUEDO HACER QUE NO ESPERE UNA LLAMADA SIN IGNORE?

  //fsm_malloc_AddCallback(cb_malloc); 



  f = fsm_new(tt);

  TEST_ASSERT_EQUAL (NULL, f);

}



/**

 * @brief La mquina de estados devuelve NULL y no llama a fsm_malloc si la funcin de comprobacin de la primera transicin es NULL (fin de la tabla)

 * 

 */

void test_fsm_nullWhenFirstCheckFunctionIsNull (void) {

  fsm_trans_t tt[] = {

    {1, NULL, -1, do_nothing},

    {-1, NULL, -1, NULL}

    };

  fsm_t *f = (fsm_t*)1;

  

  //fsm_malloc_ExpectAnyArgsAndReturn(0); //COMO PUEDO HACER QUE NO ESPERE UNA LLAMADA SIN IGNORE?

  //fsm_malloc_AddCallback(cb_malloc); 



  f = fsm_new(tt);

  TEST_ASSERT_EQUAL (NULL, f);

}





/**

 * @brief Devuelve puntero no NULL y llama a fsm_malloc (AddCallback) al crear la maquina de estados con una transicin vlida con funcin de actualizacin (salida) NULL o no NULL.

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

    //Si usases solo expect tendras que especificar la entrada

    //fsm_malloc_ExpectAndReturn(sizeof(fsm_t), NULL);



    f = fsm_new(tt);

    TEST_ASSERT_NOT_EQUAL(NULL, f);  

    free(f);

}





/**

 * @brief Estado inicial corresponde al estado de entrada de la primera transicin de la lista al crear una maquina de estados y es valido. 

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

 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la funcin de guarda cuando se comprueba una transicin

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

 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transicin devuelve true (cambia) y cuando devuelve false (mantiene)

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

 * @brief La creacin de una mquina de estados devuelve NULL si la reserva de memoria falla (Mock, no Stub)

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

 * @brief Comprueba que solo se llame a la funcin de guarda que toca segn el estado actual

 * 

 */

void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)

{

    fsm_t *f = (fsm_t*)1;

    fsm_trans_t tt[] = {

        {0, is_true, 1, NULL},

        {1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva funcin para mock is_true2

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

 * @brief Comprueba que se pueden crear dos instancias de mquinas de estados simultnteas y son punteros distintos

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


