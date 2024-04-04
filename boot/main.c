#include "stdint.h"
#include "stdbool.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

#include "stdio.h"
#include "stdlib.h"

#include "Kernel.h"

static void Hw_init(void);
static void Kernel_init(void);

static void Printf_test(void);
static void Timer_test(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);

void main(void)
{
    Hw_init();

    uint32_t i = 100;
    while(i--)
    {
        Hal_uart_put_char('N');
    }
    Hal_uart_put_char('\n');

    putstr("Hello World!\n");

    Printf_test();
    Timer_test();

    Kernel_init();

    while(true);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}


static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();

    taskId = Kernel_task_create(User_task0, 0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1, 0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2, 0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task2 creation fail\n");
    }

    Kernel_start();
}

static void Printf_test(void)
{
    char* str = "printf pointer test";
    char* nullptr = 0;
    uint32_t i = 5;
    uint32_t* sysctrl0 = (uint32_t*)0x10001000;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xff, 0xff);
    debug_printf("print zero %u\n", 0);
    debug_printf("SYSCTRL0 %x\n", *sysctrl0);
}

static void Timer_test(void)
{
    for(uint32_t i = 0; i < 5 ; i++)
    {
        debug_printf("current count : %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}

void User_task0(void)
{
    uint32_t local = 0;
    uint32_t start_time = Hal_timer_get_1ms_counter();
    debug_printf("User Task #0 SP=0x%x Started at %u\n", &local, start_time);

    while(true)
    {
        if (start_time + 1000 == Hal_timer_get_1ms_counter()){
            debug_printf("User Task #0 Switched at %u\n", start_time);
            Kernel_yield();
            start_time = Hal_timer_get_1ms_counter();
        }
    }
}

void User_task1(void)
{
    uint32_t local = 0;
    uint32_t start_time = Hal_timer_get_1ms_counter();
    debug_printf("User Task #1 SP=0x%x Started at %u\n", &local, start_time);

    while(true)
    {
        if (start_time + 1000 == Hal_timer_get_1ms_counter()){
            debug_printf("User Task #1 Switched at %u\n", start_time);
            Kernel_yield();
            start_time = Hal_timer_get_1ms_counter();
        }
    }
}

void User_task2(void)
{
    uint32_t local = 0;
    uint32_t start_time = Hal_timer_get_1ms_counter();
    debug_printf("User Task #2 SP=0x%x Started at %u\n", &local, start_time);

    while(true)
    {
        if (start_time + 1000 == Hal_timer_get_1ms_counter()){
            debug_printf("User Task #2 Switched at %u\n", start_time);
            Kernel_yield();
            start_time = Hal_timer_get_1ms_counter();
        }
    }
}