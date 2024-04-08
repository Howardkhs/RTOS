#ifndef HAL_HALTIMER_H_
#define HAL_HALTIMER_H_

void Hal_timer_init(void);
uint32_t Hal_timer_get_1ms_counter(void);
void Hal_timer2_init(void);
void Proc_Timer2_on(uint32_t runtime);

#endif