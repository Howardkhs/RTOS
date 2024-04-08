#ifndef KERNEL_KERNEL_H_
#define KERNEL_KERNEL_H_

#include "task.h"
#include "msg.h"

void Kernel_yield(void);
void Kernel_start(void);
bool Kernel_send_msg(KernelMsgQ_t Qname, void* data, uint32_t count);
uint32_t Kernel_recv_msg(KernelMsgQ_t Qname, void* out_data, uint32_t count);
void Kernel_lock_sem(void);
void Kernel_unlock_sem(void);
void Kernel_lock_mutex(void);
void Kernel_unlock_mutex(void);
#endif