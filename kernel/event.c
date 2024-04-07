#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "event.h"
#include "memio.h"

static uint32_t sEventFlag;

void Kernel_event_flag_init(void){
    sEventFlag = 0;
}

void Kernel_event_flag_set(KernelEventFlag_t event){
    sEventFlag |= (uint32_t)event;
}

void Kernel_event_flag_clear(KernelEventFlag_t event){
    sEventFlag &= ~((uint32_t)event);
}

bool Kernel_event_flag_check(KernelEventFlag_t event){
    if (sEventFlag & (uint32_t)event){
        Kernel_event_flag_clear(event);
        return true;
    }
    return false;
}

void Kernel_send_events(uint32_t event_list){
    for (uint32_t i = 0; i < 32; i++){
        if ((event_list >> i) & 1){
            KernelEventFlag_t sending_event = KernelEventFlag_Empty;
            sending_event = (KernelEventFlag_t)SET_BIT(sending_event, i);
            Kernel_event_flag_set(sending_event);
        }
    }
}

KernelEventFlag_t Kernel_wait_events(uint32_t waiting_list){
    for (uint32_t i = 0; i < 32; i++){
        if ((waiting_list >> i) & 1){
            KernelEventFlag_t waiting_event = KernelEventFlag_Empty;
            waiting_event = (KernelEventFlag_t)SET_BIT(waiting_event, i);

            if (Kernel_event_flag_check(waiting_event)){
                return waiting_event;
            }
        }
    }
    return KernelEventFlag_Empty;
}