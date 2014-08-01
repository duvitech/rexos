/*
    RExOS - embedded RTOS
    Copyright (c) 2011-2014, Alexey Kramarenko
    All rights reserved.
*/

#include "sys.h"
#include "sys_config.h"
#include "../userspace/process.h"
#include "../userspace/lib/stdio.h"
#include "../userspace/ipc.h"

#if defined (STM32)
#include "drv/stm32_core.h"
#if (UART_DRIVER)
#include "drv/stm32_uart.h"
#endif //UART_DRIVER
#if (ADC_DRIVER)
#include "drv/stm32_adc.h"
#endif //ADC_DRIVER
#if (USB_DRIVER)
#include "drv/stm32_usb.h"
#endif //USB_DRIVER
#endif
#include "sys_config.h"

extern const REX __SYS;

#if (SYS_APP)
extern const REX __APP;
#endif

void init(void);

const REX __INIT = {
    //name
    "INIT",
    //size
    256,
    //priority - init priority
    ((unsigned int)-1),
    //flags - init must be called frozen)
    0,
    //ipc size - no ipc
    0,
    //function
    init
};

void init()
{
    //start the system
    __HEAP->system = process_create(&__SYS);

#if defined(STM32)
    process_create(&__STM32_CORE);
#if (UART_DRIVER)
    process_create(&__STM32_UART);
#endif
#if (ADC_DRIVER)
    process_create(&__STM32_ADC);
#endif //ADC_DRIVER
#if (USB_DRIVER)
    process_create(&__STM32_USB);
#endif //USB_DRIVER

#else
#warning No drivers loaded. System is abstract!
#endif

    //start user application, if required
#if (SYS_APP)
    process_create(&__APP);
#endif

    for (;;)
    {
#if (SYS_POWERSAVE)
#if defined(CORTEX_M)
        __WFI();
#endif //CORTEX_M
#endif
    }
}
