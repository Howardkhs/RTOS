#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t* Uart;

void Hal_uart_init(void){
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;
}

void Hal_uart_put_char(uint8_t ch){
    while(Uart->uartfr.bits.TXFF);
    Uart->uartdr.all = (ch & 0xFF);
    
}

uint8_t Hal_uart_get_char(void){
    uint32_t data;

    while(Uart->uartfr.bits.RXFE); // 받기 버퍼 비어있으면 계속 돌아가겠네

    data = Uart->uartdr.all;

    if (data & 0xFFFFFF00){
        Uart->uartrsr.all = 0XFF;
        return 0;
    }
    return (uint8_t)(data & 0xFF);
}
