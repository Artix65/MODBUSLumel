# Komunikacja MODBUS STM32 (RS485/Modbus & DHT11)

Projekt przedstawia węzeł sieci czujników oparty na mikrokontrolerze STM32. System mierzy parametry (temperaturę i wilgotność) za pomocą czujnika DHT11 i przesyła dane interfejsem RS485, wykorzystując własną implementację logiki protokołu MODBUS RTU.
Głównym celem projektu edukacyjnego było zrozumienie działania niskopoziomowych protokołów komunikacyjnych poprzez ręczne napisanie sterownika 1-wire (dla DHT11) oraz obsługę transmisji RS485 bez polegania na gotowych bibliotekach wysokiego poziomu dla tych konkretnych czujników.
