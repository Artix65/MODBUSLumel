/* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  RS485_WlaczOdbieranie();
  
  DanePomiarowe moje_pomiary;
  uint8_t bufor_rozkazu[1];
  uint8_t bufor_wysylkowy[2];
  /* USER CODE END 2 */

  while (1)
  {
      if(HAL_UART_Receive(&huart1, bufor_rozkazu, 1, HAL_MAX_DELAY) == HAL_OK) {
          if(bufor_rozkazu[0] == 'Q') {
              if(Czytaj_DHT11(&moje_pomiary)) {
                  bufor_wysylkowy[0] = moje_pomiary.wilgotnosc;
                  bufor_wysylkowy[1] = moje_pomiary.temperatura;
              } else {
                  bufor_wysylkowy[0] = 0;
                  bufor_wysylkowy[1] = 0;
              }
              opoznienie_us(500); 
              RS485_WlaczNadawanie();
              HAL_UART_Transmit(&huart1, bufor_wysylkowy, 2, 100);
              while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
              RS485_WlaczOdbieranie();
          }
      }
  }
