  HAL_TIM_Base_Start(&htim1);
  RS485_WlaczOdbieranie();
  
  DanePomiarowe moje_pomiary;
  DanePomiarowe obce_pomiary;
  uint8_t bufor_nadawczy[10];
  uint8_t bufor_odbiorczy[2];


  while (1)
  {
      if(Czytaj_DHT11(&moje_pomiary)) {
      } else {
          moje_pomiary.temperatura = 0;
          moje_pomiary.wilgotnosc = 0;
      }

      RS485_WlaczNadawanie();
      uint8_t zapytanie = 'Q'; //Q jako Query
      HAL_UART_Transmit(&huart1, &zapytanie, 1, 100);
      RS485_WlaczOdbieranie(); //powrót do nasłuchu

      if(HAL_UART_Receive(&huart1, bufor_odbiorczy, 2, 100) == HAL_OK) {
          obce_pomiary.wilgotnosc = bufor_odbiorczy[0];
          obce_pomiary.temperatura = bufor_odbiorczy[1];
      } else {
          obce_pomiary.wilgotnosc = 0xFF;
          obce_pomiary.temperatura = 0xFF;
      }
      HAL_Delay(2000); //Dla DHT11
  }
