# Battery-Monitoring-System-V2
Evolução do projeto anterior de Monitoramento de Parâmetros Elétricos e Térmicos de Duas Células de Bateria de 4 V

<img width="735" height="550" alt="image" src="https://github.com/user-attachments/assets/8c6d4141-8982-49fa-a0ae-bc912e85d8f8" />

## Descrição
Este projeto realiza a medição da tensão e da corrente de duas células de bateriar pondendo estar em carga usando o módulo TP4056 utilizando o sensor INA219 current sensor module por meio do protocolo I²C, além de monitorar a temperatura através de um termistor NTC222 conectado a um conversor ADC. Essa versão é mais enxuta que a anterior pois os componentes usados estão todos soldados em uma que foi feita no KiCad.
Quando a temperatura de uma das duas células ultrapassa um determinado limiar (threshold), um buzzer é acionado como mecanismo de alerta.

## Materiais
Para a implementação deste projeto, foram utilizados os seguintes componentes: duas células de bateria de 4 V com suporte (shell), dois sensores de corrente e tensão INA219 current sensor module, dois termistores NTC 222, dois resistores de 10 kΩ, dois resistores de 1 kΩ, um buzzer, dois capacitores de 10 nF, um módulo de carregamento TP4056, uma ESP32-D e uma PCB feita no KiCad.

## Conexões 
Como foram utilizados dois sensores INA219 current sensor module, foi necessário configurar endereços I²C distintos para cada dispositivo. O primeiro sensor foi mantido no endereço padrão (0x40), enquanto o segundo teve seu endereço ajustado para 0x44 por meio da configuração de offset.

O buzzer foi conectado ao GPIO 27 da ESP32-D e a comunicação I²C foi realizada utilizando o GPIO 21 como SDA e o GPIO 22 como SCL.
Para a medição de temperatura, os termistores foram ligados a divisores resistivos: o primeiro conectado ao GPIO 34 e o segundo ao GPIO 35 do microcontrolador.
O TP4056 foi conectado em paralelo com as duas células de 4 volts, sendo alimentando por um cabo micro-usb. 

Abaixo um esquema das conexões feitas:
<img width="772" height="441" alt="image" src="https://github.com/user-attachments/assets/b2e823aa-031f-485e-82ac-a9f27ada1d4a" />

