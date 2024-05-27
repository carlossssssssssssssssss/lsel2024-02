/**
 * @file stm32_print.c
 * @brief File that redefines the function _write to redirect the printf function to the ITM console.
 *
 * The ITM is a an application driven trace source that supports printf style debugging to trace Operating System (OS) and
 * application events, and emits diagnostic system information.
 *
 * @author Sistemas Digitales II
 * @date 2022-12-19
 */
#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Function able to use printf via SWO:ITM. It prints the messages on a terminal in VSCode.
 *
 * @param file
 * @param ptr
 * @param len
 * @return int
 */
int _write(int file, char *ptr, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        ITM_SendChar((*ptr++));
    }
    return len;
}
