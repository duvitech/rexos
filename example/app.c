/*
    RExOS - embedded RTOS
    Copyright (c) 2011-2015, Alexey Kramarenko
    All rights reserved.
*/

#include "../userspace/sys.h"
#include "../userspace/gpio.h"
#include "../drv/stm32/stm32_core.h"
#include "../userspace/stm32_driver.h"
#include "../userspace/stdio.h"
#include "../userspace/stdlib.h"
#include "../userspace/ipc.h"
#include "../userspace/timer.h"
#include "../userspace/wdt.h"
#include "../userspace/uart.h"
#include "../midware/pinboard.h"
#include "app_private.h"
#include "comm.h"
#include "config.h"
#include "../userspace/adc.h"


void app();

const REX __APP = {
    //name
    "App main",
    //size
    1024,
    //priority
    200,
    //flags
    PROCESS_FLAGS_ACTIVE | REX_HEAP_FLAGS(HEAP_PERSISTENT_NAME),
    //ipc size
    5,
    //function
    app
};

static inline void stat()
{
    TIME uptime;
    int i;
    unsigned int diff;

    get_uptime(&uptime);
    for (i = 0; i < TEST_ROUNDS; ++i)
        svc_test();
    diff = time_elapsed_us(&uptime);
    printf("average kernel call time: %d.%dus\n\r", diff / TEST_ROUNDS, (diff / (TEST_ROUNDS / 10)) % 10);

    get_uptime(&uptime);
    for (i = 0; i < TEST_ROUNDS; ++i)
        process_switch_test();
    diff = time_elapsed_us(&uptime);
    printf("average switch time: %d.%dus\n\r", diff / TEST_ROUNDS, (diff / (TEST_ROUNDS / 10)) % 10);

    printf("core clock: %d\n\r", get(object_get(SYS_OBJ_CORE), HAL_CMD(HAL_POWER, STM32_POWER_GET_CLOCK), STM32_CLOCK_CORE, 0, 0));
    process_info();
}

static inline void app_setup_dbg()
{
    BAUD baudrate;
    ack(object_get(SYS_OBJ_CORE), HAL_CMD(HAL_GPIO, STM32_GPIO_ENABLE_PIN), DBG_CONSOLE_TX_PIN, STM32_GPIO_MODE_AF | GPIO_OT_PUSH_PULL | GPIO_SPEED_HIGH, DBG_CONSOLE_TX_PIN_AF);
    ack(object_get(SYS_OBJ_UART), HAL_CMD(HAL_UART, IPC_OPEN), DBG_CONSOLE, FILE_MODE_WRITE, 0);
    baudrate.baud = DBG_CONSOLE_BAUD;
    baudrate.data_bits = 8;
    baudrate.parity = 'N';
    baudrate.stop_bits= 1;
    uart_set_baudrate(DBG_CONSOLE, &baudrate);
    uart_setup_printk(DBG_CONSOLE);
    uart_setup_stdout(DBG_CONSOLE);
    open_stdout();
}

static inline void app_init(APP* app)
{
    process_create(&__STM32_CORE);

    gpio_enable_pin(B14, GPIO_MODE_OUT);
    gpio_reset_pin(B14);


    app_setup_dbg();

    app->timer = timer_create(0, HAL_APP);
    timer_start_ms(app->timer, 1000, 0);

    stat();
    printf("App init\n\r");
}

static inline void app_timeout(APP* app)
{
    printf("app timer timeout test\n\r");
    printf("vlcd: %d\n\r", adc_get(STM32_ADC_VREF));
    timer_start_ms(app->timer, 1000, 0);
}

void app()
{
    APP app;
    IPC ipc;
    bool need_post = false;

    app_init(&app);
    comm_init(&app);


    ack(object_get(SYS_OBJ_ADC), HAL_CMD(HAL_ADC, IPC_OPEN), STM32_ADC_DEVICE, 0, 0);
    ack(object_get(SYS_OBJ_ADC), HAL_CMD(HAL_ADC, IPC_OPEN), STM32_ADC_VREF, STM32_ADC_SMPR_55_5, 0);
    adc_get(STM32_ADC_VREF);


    for (;;)
    {
        ipc_read(&ipc);
        need_post = false;
        switch (HAL_ITEM(ipc.cmd))
        {
        case USBD_ALERT:
            comm_usbd_alert(&app, ipc.param1);
            break;
        case IPC_STREAM_WRITE:
            comm_usbd_stream_rx(&app, ipc.param3);
            break;
        case IPC_TIMEOUT:
            app_timeout(&app);
            break;
        default:
            printf("Unhandled cmd: %#X\n\r", ipc.cmd);
            printf("sender: %#X\n\r", ipc.process);
            printf("p1: %#X\n\r", ipc.param1);
            error(ERROR_NOT_SUPPORTED);
            need_post = true;
            break;
        }
        if (need_post)
            ipc_post_or_error(&ipc);
    }
}
