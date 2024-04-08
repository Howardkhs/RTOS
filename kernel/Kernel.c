#include "stdint.h"
#include "stdbool.h"
#include "Kernel.h"
#include "msg.h"
#include "synch.h"

void Kernel_yield(void){
    Kernel_task_scheduler();
}

void Kernel_start(void){
    Kernel_task_start();
}

bool Kernel_send_msg(KernelMsgQ_t Qname, void* data, uint32_t count)
{
    uint8_t* d = (uint8_t*)data;

	if (count > (MSG_Q_SIZE_BYTE - Kernel_msgQ_count(Qname)))
	{
		return false;
	}

    for (uint32_t i = 0 ; i < count ; i++)
    {
        if (false == Kernel_msgQ_enqueue(Qname, *d))
        {
            return false;
        }
        d++;
    }

    return true;
}

uint32_t Kernel_recv_msg(KernelMsgQ_t Qname, void* out_data, uint32_t count)
{
    uint8_t* d = (uint8_t*)out_data;

    for (uint32_t i = 0 ; i < count ; i++)
    {
        if (false == Kernel_msgQ_dequeue(Qname, d))
        {
            return i;
        }
        d++;
    }

    return count;
}

void Kernel_lock_sem(void){
    while(false == Kernel_sem_test()){
        Kernel_yield();
    }
}

void Kernel_unlock_sem(void){
    Kernel_sem_release();
}

void Kernel_lock_mutex(void){
    while(true){
        uint32_t current_task_id = Kernel_task_get_current_task_id();
        if (false == Kernel_mutex_lock(current_task_id)){
            Kernel_yield();
        }
        else{
            break;
        }
    }
}

void Kernel_unlock_mutex(void){
    uint32_t current_task_id = Kernel_task_get_current_task_id();
    if (false == Kernel_mutex_unlock(current_task_id)){
        Kernel_yield();
    }

}