#include "stdint.h"
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"

extern volatile Timer_t* Timer;

static void interrput_handler(void);

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