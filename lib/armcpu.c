#include "armcpu.h"
// gccc는 어셈 사용해야함 armcc는 cpsr 접근 가능 이걸로 인터럽트 제어
void enable_irq(void){

    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS  r0, cpsr");
    __asm__ ("BIC  r1, r0, #0x80");
    __asm__ ("MSR  cpsr, r1");
    __asm__ ("POP  {r0, r1}");
}

void enable_fiq(void){

    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS  r0, cpsr");
    __asm__ ("BIC  r1, r0, #0x40");
    __asm__ ("MSR  cpsr, r1");
    __asm__ ("POP  {r0, r1}");
}

void disable_irq(void){

    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS  r0, cpsr");
    __asm__ ("ORR  r1, r0, #0x80");
    __asm__ ("MSR  cpsr, r1");
    __asm__ ("POP  {r0, r1}");
}

void disable_fiq(void){

    __asm__ ("PUSH {r0, r1}");
    __asm__ ("MRS  r0, cpsr");
    __asm__ ("ORR  r1, r0, #0x40");
    __asm__ ("MSR  cpsr, r1");
    __asm__ ("POP  {r0, r1}");
}