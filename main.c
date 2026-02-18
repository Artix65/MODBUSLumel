#include "string.h"
#include "stdio.h"

typedef struct {
    uint8_t wilgotność;
    uint8_t temperatura;
} DanePomiarowe;

#define PORT_RS485 GPIOA
#define PIN_RS485  GPIO_PIN_1
#define PORT_DHT11 GPIOA
#define PIN_DHT11  GPIO_PIN_0

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;

void opoznienie_us(uint16_t czas_us) {
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < czas_us);
}


void RS485_WlaczNadawanie() {
    HAL_GPIO_WritePin(PORT_RS485, PIN_RS485, GPIO_PIN_SET);
    opoznienie_us(10); //NIE USUWAĆ
}

void RS485_WlaczOdbieranie() {
    HAL_GPIO_WritePin(PORT_RS485, PIN_RS485, GPIO_PIN_RESET);
}

void UstawPinDHT(uint8_t tryb) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = PIN_DHT11;
    if (tryb == 1) {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    } else {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    HAL_GPIO_Init(PORT_DHT11, &GPIO_InitStruct);
}

uint8_t Czytaj_DHT11(DanePomiarowe *dane) {
    uint8_t i, j;
    uint8_t bajty[5] = {0};

    UstawPinDHT(1);
    HAL_GPIO_WritePin(PORT_DHT11, PIN_DHT11, GPIO_PIN_RESET);
    HAL_Delay(18); 
    HAL_GPIO_WritePin(PORT_DHT11, PIN_DHT11, GPIO_PIN_SET);
    opoznienie_us(20);
    UstawPinDHT(0);

    opoznienie_us(40);
    if (HAL_GPIO_ReadPin(PORT_DHT11, PIN_DHT11)) return 0; // Brak odpowiedzi
    opoznienie_us(80);
    if (!HAL_GPIO_ReadPin(PORT_DHT11, PIN_DHT11)) return 0; // Błąd
    while (HAL_GPIO_ReadPin(PORT_DHT11, PIN_DHT11));

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {
            while (!HAL_GPIO_ReadPin(PORT_DHT11, PIN_DHT11));
            opoznienie_us(40);
            if (HAL_GPIO_ReadPin(PORT_DHT11, PIN_DHT11)) {
                bajty[j] |= (1 << (7 - i));
            }
            while (HAL_GPIO_ReadPin(PORT_DHT11, PIN_DHT11));
        }
    }

    // Do danych dodajemy sumę kontrolną
    if ((uint8_t)(bajty[0] + bajty[1] + bajty[2] + bajty[3]) == bajty[4]) {
        dane->wilgotność = bajty[0];
        dane->temperatura = bajty[2];
        return 1;
    }
    return 0;
}
