# AgroMonitoring
### **🛒 Что купить (минимум деталей)**
| Компонент               | Кол-во | Цена (руб) | Где купить       |
|-------------------------|--------|------------|------------------|
| **Arduino Nano**        | 2      | 200–300    | Aliexpress/Локально |
| **NRF24L01+ (PA/LNA)**  | 2      | 150–200    | Aliexpress       |
| **Датчик DHT22**        | 1      | 200        | Aliexpress       |
| **Провода и макетка**   | -      | 100        | Локальный магазин |
| **PowerBank**           | 1      | 500        | Локальный магазин |

**Итого**: ~1200–1500 руб.

---

### **📋 Пошаговый план**
#### **1. Настройка среды (30 мин)**
1. Установите **Arduino IDE** ([скачать](https://www.arduino.cc/en/software)).
2. Добавьте библиотеки:
   - **RF24** (для NRF24L01+):  
     ```cpp
     Скетч → Подключить библиотеку → Управлять библиотеками → Поиск "RF24 by TMRh20" → Установить
     ```
   - **DHT sensor library** (для датчика DHT22).

---

#### **2. Сборка передатчика (Arduino + DHT22 + NRF24L01+)**
**Схема подключения**:
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

**Код передатчика** (`transmitter.ino`):
```cpp
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

RF24 radio(9, 10); // CE, CSN
DHT dht(DHTPIN, DHTTYPE);

struct SensorData {
  float temp;
  float hum;
};

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0AA); // Адрес канала
  radio.setPALevel(RF24_PA_MAX); // Максимальная мощность
  dht.begin();
}

void loop() {
  SensorData data;
  data.temp = dht.readTemperature();
  data.hum = dht.readHumidity();

  if (radio.write(&data, sizeof(data))) {
    Serial.println("Данные отправлены!");
  } else {
    Serial.println("Ошибка передачи!");
  }
  delay(5000); // Отправка каждые 5 сек
}
```

---

#### **3. Сборка приемника (Arduino + NRF24L01+)**
**Схема подключения** (аналогична передатчику, но без DHT22).

**Код приемника** (`receiver.ino`):
```cpp
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

struct SensorData {
  float temp;
  float hum;
};

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, 0xF0F0F0F0AA); // Адрес как у передатчика
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    SensorData data;
    radio.read(&data, sizeof(data));
    Serial.print("Temp: "); Serial.print(data.temp);
    Serial.print(" Hum: "); Serial.println(data.hum);
  }
}
```

---

#### **4. Подключение к ПК**
1. Подключите **приемник (Arduino)** к компьютеру через USB.
2. Откройте **Монитор порта** (Ctrl+Shift+M в Arduino IDE).
3. Вы увидите данные в формате:  
   ```
   Temp: 25.3 Hum: 60.0
   ```

---

#### **5. Дополнительно: Python-скрипт для сохранения данных**
Создайте файл `serial_reader.py`:
```python
import serial
import time

ser = serial.Serial('COM3', 115200)  # Укажите порт Arduino

while True:
    line = ser.readline().decode().strip()
    if line:
        print(line)
        # Можно добавить запись в файл:
        with open("sensor_data.txt", "a") as f:
            f.write(line + "\n")
    time.sleep(1)
```
Запустите:  
```bash
python serial_reader.py
```

---

### **🔧 Возможные проблемы и решения**
1. **NRF24L01+ не передает данные**:
   - Проверьте **питание** (3.3V для NRF24L01+, 5V для Arduino).
   - Убедитесь, что **адреса каналов** совпадают.
   - Попробуйте **другой PALevel** (`RF24_PA_LOW`).

2. **DHT22 не работает**:
   - Проверьте подключение (**DATA → D2**, **VCC → 5V**).
   - Добавьте **подтягивающий резистор 10 кОм** между DATA и VCC.

---

### **📌 Что дальше?**
1. **Добавьте больше датчиков** (например, датчик освещенности `BH1750`).
2. **Сделайте корпус** из пластика или фанеры.
3. **Настройте питание от солнечной панели**.

---

### **🚀 Итог**
- **Бюджет**: 1200–1500 руб.
- **Сроки**: 1–2 дня.
- **Сложность**: Низкая (подходит для новичков).

Если что-то не работает — пишите, помогу разобраться! 😊