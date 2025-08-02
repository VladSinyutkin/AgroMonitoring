# **Схема подключения передатчика**:
```
Arduino Nano → NRF24L01+  
5V         → VCC  
GND        → GND  
D13        → SCK  
D12        → MISO  
D11        → MOSI  
D10        → CSN  
D9         → CE  

Arduino Nano → DHT22  
5V         → VCC  
GND        → GND  
D2         → DATA  
```
# **Схема подключения приемника**:
ESP8266 (NodeMCU) → NRF24L01+  
3V3              → VCC  
GND              → GND  
D5 (SCK)         → SCK  
D6 (MISO)        → MISO  
D7 (MOSI)        → MOSI  
D8               → CSN  
D4               → CE  