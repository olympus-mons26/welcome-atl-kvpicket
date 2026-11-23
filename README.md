# Smart Proximity Welcome System

An Arduino UNO project that detects an approaching person using an ultrasonic sensor and greets them with a welcome message on an OLED display and a short buzzer chime. When nobody is nearby, the OLED displays a continuous `. → .. → ...` waiting animation.

## Features

- Detects objects and people within a **50 cm** threshold using an HC-SR04 ultrasonic sensor.
- Displays a three-dot standby animation (`. → .. → ...`) when the area is clear.
- Displays a framed `WELCOME!` message for the Atal Tinkering Lab of PM SHRI KV Picket with an animated border dot when triggered.
- Emits a single 2000 Hz welcome beep upon entry without continuously re-triggering while the person remains nearby.
- Prints real-time distance measurements in centimetres to the Serial Monitor.

## Components Required

- Arduino UNO R3 (or a compatible Arduino board)
- HC-SR04 ultrasonic distance sensor
- 0.96-inch I2C OLED display module (128×64, SSD1306)
- 2-pin buzzer (active or passive)
- Breadboard
- Jumper wires
- USB cable for programming and Serial Monitor communication

> **Safety note:** Check your OLED module's voltage rating before connecting it. Use the 5V pin only if your module supports 5V logic/power; otherwise, connect VCC to the Arduino's **3.3V** pin.

## Connections

| Component | Component Pin | Arduino UNO Pin |
| --------- | ------------- | --------------- |
| 0.96" I2C OLED | GND | GND |
| | VCC | 5V (or 3.3V) |
| | SCK (SCL) | A5 |
| | SDA | A4 |
| HC-SR04 Sensor | VCC | 5V |
| | GND | GND |
| | TRIG | D9 |
| | ECHO | D10 |
| Buzzer | + (Positive) | D8 |
| | - (Negative) | GND |

The I2C OLED communicates over the hardware I2C pins (`A4` and `A5`), while the ultrasonic sensor and buzzer use standard digital I/O pins (`D8`, `D9`, and `D10`).

## How to Use

1. Connect the circuit according to the connections table.
2. Open the Arduino IDE and go to **Sketch → Include Library → Manage Libraries...**
3. Install **Adafruit GFX Library** and **Adafruit SSD1306**.
4. Open the project sketch in the Arduino IDE.
5. Select your board (**Arduino Uno**) and port from **Tools**.
6. Upload the sketch to the Arduino.
7. *(Optional)* Open **Tools → Serial Monitor** and set the baud rate to **9600** to view live distance readings.

### Example

**Standby State (Distance > 50 cm):**

```text
OLED Screen: . → .. → ...
Buzzer: Silent
Serial Monitor: Distance: 142.50 cm
```

**Detected State (Distance ≤ 50 cm):**

```text
OLED Screen: 
  WELCOME!
  Atal Tinkering Lab
  of PM SHRI KV Picket
Buzzer: 200 ms beep (2000 Hz)
Serial Monitor: Distance: 34.12 cm
```

## Distance Calculation

The sketch triggers a $10\,\mu\text{s}$ ultrasonic pulse on `TRIG_PIN` and measures the echo return duration in microseconds using `pulseIn()`. It converts this round-trip time into centimetres using the speed of sound ($0.0343\,\text{cm}/\mu\text{s}$):

```cpp
return duration * 0.0343 / 2;
```

Dividing by $2$ accounts for the sound wave traveling to the object and back:

$$\text{Distance (cm)} = \frac{\text{Duration} \times 0.0343}{2}$$

If the measured distance is less than or equal to `DETECT_DISTANCE` ($50\,\text{cm}$), the system triggers the welcome state.

## Troubleshooting

- **OLED stays blank:** Verify the wiring on `A4` (SDA) and `A5` (SCL), and check whether your display uses I2C address `0x3C` or `0x3D` in `#define OLED_ADDRESS`.
- **No sound from buzzer:** Confirm polarity (`+` to D8, `-` to GND). If using a passive buzzer, ensure `tone()` is not conflicting with other timers.
- **Distance always reads 999 cm:** Check that `TRIG` is on D9, `ECHO` is on D10, and the sensor is receiving a solid 5V supply.
- **No Serial Monitor output:** Ensure the baud rate is set to `9600`.

## License

This project is licensed under the MIT License. See the [`LICENSE`](LICENSE) file for the full license text.

***

Made by Baljit Panda ([olympus-mons26](https://olympus-mons26/welcome-atl-kvpicket))