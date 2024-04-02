#ifndef HAL_HALUART_H_
#define HAL_HALUART_H_

static void interrput_handler(void);
void Hal_uart_init(void);
void Hal_uart_put_char(uint8_t ch);
uint8_t Hal_uart_get_char(void);

#endif