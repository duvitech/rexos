/*
    RExOS - embedded RTOS
    Copyright (c) 2011-2014, Alexey Kramarenko
    All rights reserved.
*/

#include "lib_lib.h"
#include "../kernel/dbg.h"
#include "kernel_config.h"
#include "lib_std.h"
#include "lib_stdio.h"
#include "lib_systime.h"
#include "lib_heap.h"
#include "lib_gpio.h"

void lib_stub ()
{
#if (KERNEL_DEBUG)
    printk("Warning: lib stub called\n");
#endif //KERNEL_DEBUG
}

const void *const __LIB[] = {
    //lib_stdio.h
    (const void *const)&__LIB_STD,
    //lib_stdio.h
    (const void *const )&__LIB_STDIO,
    //lib_systime.h
    (const void *const)&__LIB_SYSTIME,
    //lib_heap.h
    (const void *const )&__LIB_HEAP,
    //hardware specific
    (const void *const)&__LIB_GPIO
};


