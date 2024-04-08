#include "stdint.h"
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"
#include "event.h"
#include "stdio.h"
#include "stdlib.h"

extern volatile Timer_t* Timer;
extern volatile Timer_t* Timer2;

static void interrput_handler(void);
static void interrput_handler2(void);

static uint32_t internal_1ms_counter;

void Hal_timer_init(void){
    // 초기화

    Timer->timerxcontrol.bits.TimerEn = 0; //타이머 끔
    Timer->timerxcontrol.bits.TimerMode = 0; // timerxload 사용할지 않할지 안하면 최댓ㄱ밧
    Timer->timerxcontrol.bits.OneShot = 0; // 타이머가 한번 발동후 꺼짐
    Timer->timerxcontrol.bits.TimerSize = 0; //16비트 크기
    Timer->timerxcontrol.bits.TimerPre = 0; // 클럭마다 카운터 줄임
    Timer->timerxcontrol.bits.IntEnable = 0; // 인터럽트 여부
    Timer->timerxload = 0;
    Timer->timerxvalue = 0xFFFFFFFF;

    Timer->timerxcontrol.bits.TimerMode = TIMER_PERIOIC;
    Timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
    Timer->timerxcontrol.bits.OneShot = 0;
    Timer->timerxcontrol.bits.TimerPre = 0;
    Timer->timerxcontrol.bits.IntEnable = 1;

    uint32_t interval_1ms = TIMER_1MZ_INTERVAL / 1000;

    Timer->timerxload = interval_1ms;
    Timer->timerxcontrol.bits.TimerEn = 1;

    internal_1ms_counter = 0;

    Hal_interrupt_enable(TIMER_INTERRUPT);
    Hal_interrupt_register_handler(interrput_handler, TIMER_INTERRUPT);
}

static void interrput_handler(void){
    internal_1ms_counter++;
    Timer->timerxintclr = 1; // 인터럽트 처리완료 그래야 다음단계로 넘어감
}

uint32_t Hal_timer_get_1ms_counter(void){
    return internal_1ms_counter;
}

void Hal_timer2_init(void){
    // 초기화
    Timer2->timerxcontrol.bits.TimerEn = 0; //타이머 끔
    Timer2->timerxcontrol.bits.TimerMode = 0; // timerxload 사용할지 않할지 안하면 최댓ㄱ밧
    Timer2->timerxcontrol.bits.OneShot = 0; // 타이머가 한번 발동후 꺼짐
    Timer2->timerxcontrol.bits.TimerSize = 0; //16비트 크기
    Timer2->timerxcontrol.bits.TimerPre = 0; // 클럭마다 카운터 줄임
    Timer2->timerxcontrol.bits.IntEnable = 0; // 인터럽트 여부
    Timer2->timerxload = 0;
    Timer2->timerxvalue = 0xFFFFFFFF;

    Timer2->timerxcontrol.bits.TimerMode = TIMER_PERIOIC;
    Timer2->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
    Timer2->timerxcontrol.bits.OneShot = 0;
    Timer2->timerxcontrol.bits.TimerPre = 0;
    Timer2->timerxcontrol.bits.IntEnable = 1;

    Hal_interrupt_enable(TIMER2_INTERRUPT);
    Hal_interrupt_register_handler(interrput_handler2, TIMER2_INTERRUPT);
}

void Proc_Timer2_on(uint32_t runtime){

    if (Timer2->timerxcontrol.bits.TimerEn)  // 이미 타이머 돌아가는 상태
        return;
    uint32_t interval_runtime = runtime;
    Timer2->timerxload = interval_runtime;
    Timer2->timerxcontrol.bits.TimerEn = 1;
}

static void interrput_handler2(void){
    Kernel_event_flag_set(KernelEventFlag_ProcTimeout);    
    Timer2->timerxintclr = 1; // 인터럽트 처리완료 그래야 다음단계로 넘어감
    Timer2->timerxcontrol.bits.TimerEn = 0;
}
