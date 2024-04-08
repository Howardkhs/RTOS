#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t  sTask_list[MAX_TASK_NUM];
static KernelTcb_t* sCurrent_tcb;
static KernelTcb_t* sNext_tcb;
static uint32_t     sAllocated_tcb_index;
static uint32_t     sCurrent_tcb_index;

static KernelTcb_t* Scheduler_round_robin_algorithm(void);
static void Save_context(void);
static void Restore_context(void);

void Kernel_task_init(void){
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;

    for (uint32_t i = 0; i < MAX_TASK_NUM; i++){
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
        sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;
        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t* ctx = (KernelTaskContext_t*)sTask_list[i].sp;
        ctx->pc = 0; // 메모리 직접 접근해서 할당
        ctx->spsr = ARM_MODE_BIT_SYS;
    }
}

void Kernel_task_start(void){ /// task 0번 시작시 한번만 호출
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context(); // 0번의 tcb 메모리 상에 데이터를 레지스터로 불러옴
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc, uint32_t priority){

    KernelTcb_t* new_tcb = &sTask_list[sAllocated_tcb_index++];

    if (sAllocated_tcb_index > MAX_TASK_NUM){
        return NOT_ENOUGH_TASK_NUM;
    }

    new_tcb->priority = priority;

    KernelTaskContext_t* ctx = (KernelTaskContext_t*)new_tcb->sp;
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);
}

static KernelTcb_t* Scheduler_round_robin_algorithm(void){
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}

void Kernel_task_scheduler(void){
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();

    Kernel_task_context_switching();
}

uint32_t Kernel_task_get_current_task_id(void){
    return sCurrent_tcb_index;
}

__attribute__ ((naked)) void Kernel_task_context_switching(void){
    __asm__ ("B Save_context");
    __asm__ ("B Restore_context");
}

static __attribute__ ((naked)) void Save_context(void){
    __asm__ ("PUSH {lr}");
    __asm__ ("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("MRS  r0, cpsr");
    __asm__ ("PUSH {r0}");

    __asm__ ("LDR  r0, =sCurrent_tcb");
    __asm__ ("LDR  r0, [r0]");
    __asm__ ("STMIA r0!, {sp}");
}

static __attribute__ ((naked)) void Restore_context(void)
{
    __asm__ ("LDR   r0, =sNext_tcb");
    __asm__ ("LDR   r0, [r0]");
    __asm__ ("LDMIA r0!, {sp}");

    __asm__ ("POP  {r0}");
    __asm__ ("MSR   cpsr, r0");
    __asm__ ("POP  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("POP  {pc}");  // 이 pc 값으로 다음 명령어 자동 실행
}


// static KernelTcb_t* Scheduler_priority_algorithm(void){
//     for(uint32_t i = 0; i < sAllocated_tcb_index; i++){
//         KernelTcb_t* pNextTcb = &sTask_list[i];
//         if (pNextTcb != sCurrent){
//             if (pNextTcb->priority <= sCurrent->priority){
//                 return pNextTcb;
//             }
//         }
//     }
//     return sCurrent_tcb;
// }