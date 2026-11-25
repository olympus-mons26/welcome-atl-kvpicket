# Smart Proximity Welcome System

An Arduino UNO project that detects a person using an HC-SR04 ultrasonic sensor and displays a welcome screen on a 128×64 SSD1306 OLED. The system plays a single buzzer chime when someone enters the detection range, turns on a green LED for the detected state, and turns on a red LED for the waiting state.

## Features

- Detects people and objects within **50 cm** using an HC-SR04 ultrasonic sensor.
- Displays `WELCOME!`, `Atal Tinkering Lab`, and `of PM SHRI KV Picket` when a person is detected.
- Shows a moving decorative dot below the border of the welcome screen.
- Displays an animated `.`, `..`, `...` waiting sequence when nobody is nearby.
- Plays one 2000 Hz, 200 ms welcome beep when someone enters the detection range.
- Does not repeatedly trigger the buzzer while the person remains inside the range.
- Turns on the green LED and turns off the red LED during detection.
- Turns on the red LED and turns off the green LED during the waiting state.
- Prints the measured distance in centimetres to the Serial Monitor at 9600 baud.

## Components Required

- Arduino UNO R3 or compatible Arduino board
- HC-SR04 ultrasonic distance sensor
- 0.96-inch I2C OLED display (128×64, SSD1306)
- 2-pin buzzer
- Green LED
- Red LED
- 2 × 220 Ω resistors
- Breadboard
- Jumper wires
- USB cable

> **Safety note:** Check your OLED module's voltage rating before connecting power. Use 5V only if the module supports it; otherwise, connect VCC to 3.3V. A 220 Ω resistor must be used in series with each LED.

## How It Works

The HC-SR04 continuously measures the distance in front of the sensor.

### Waiting state

When the measured distance is greater than 50 cm:

- The OLED shows the animated three-dot waiting sequence.
- The red LED is on.
- The green LED is off.
- The buzzer remains silent.

### Detected state

When the measured distance is 50 cm or less:

- The OLED shows the `WELCOME!` message and school name.
- A moving dot adds a decorative animation to the top border.
- The green LED is on.
- The red LED is off.
- The buzzer beeps once when the person enters the range.

The sketch tracks `personDetected` so the buzzer does not repeat while the same person remains inside the detection area.

## Connections

| Component | Component Pin | Arduino UNO Pin |
| --------- | ------------- | --------------- |
| 0.96" I2C OLED | SDA | A4 |
| 0.96" I2C OLED | SCL | A5 |
| 0.96" I2C OLED | VCC | 5V* |
| 0.96" I2C OLED | GND | GND |
| HC-SR04 Ultrasonic Sensor | TRIG | D9 |
| HC-SR04 Ultrasonic Sensor | ECHO | D10 |
| HC-SR04 Ultrasonic Sensor | VCC | 5V |
| HC-SR04 Ultrasonic Sensor | GND | GND |
| Buzzer | (+) | D8 |
| Buzzer | (-) | GND |
| Green LED | (+) Anode | D6 through 220 Ω resistor |
| Green LED | (-) Cathode | GND |
| Red LED | (+) Anode | D7 through 220 Ω resistor |
| Red LED | (-) Cathode | GND |

The OLED communicates over the hardware I2C pins `A4` (SDA) and `A5` (SCL). The ultrasonic sensor uses `D9` and `D10`, the buzzer uses `D8`, the green LED uses `D6`, and the red LED uses `D7`.

## Software Setup

1. Connect the circuit according to the table above.
2. Open `welcome-atl-kvpicket.ino` in the Arduino IDE.
3. Open **Sketch → Include Library → Manage Libraries**.
4. Install the **Adafruit GFX Library** and **Adafruit SSD1306** library.
5. Select **Arduino Uno** under **Tools → Board**.
6. Select the connected serial port under **Tools → Port**.
7. Upload the sketch to the Arduino.
8. Open **Tools → Serial Monitor** and set the baud rate to **9600**.

## LED and Screen Examples

### Waiting state

```text
OLED:
  128 × 64 framed screen
  . → .. → ... animated dots

Green LED: OFF
Red LED:   ON
Buzzer:    Silent
Serial:    Distance: 142.50 cm
```

### Person detected

```text
OLED:
  WELCOME!
  Atal Tinkering Lab
  of PM SHRI KV Picket

Green LED: ON
Red LED:   OFF
Buzzer:    200 ms beep at 2000 Hz
Serial:    Distance: 34.12 cm
```

## Distance Calculation

The sketch sends a 10 µs pulse on `TRIG_PIN` and measures the echo duration with `pulseIn()`:

```cpp
long duration = pulseIn(ECHO_PIN, HIGH, 30000);

if (duration == 0) {
  return 999;
}

return duration * 0.0343 / 2;
```

The result is divided by 2 because the ultrasonic pulse travels to the object and back. If the distance is less than or equal to `DETECT_DISTANCE` (`50 cm`), the welcome state is activated.

## Troubleshooting

- **OLED stays blank:** Verify `A4`/SDA and `A5`/SCL connections, power, and ground. Confirm that `OLED_ADDRESS` matches the display, typically `0x3C`.
- **Green LED does not turn on:** Check the green LED polarity and 220 Ω resistor. Verify that the LED is connected to `D6`.
- **Red LED does not turn on:** Check the red LED polarity and 220 Ω resistor. Verify that the LED is connected to `D7`.
- **Both LEDs behave incorrectly:** Confirm that only the detected LED is being driven HIGH and that the cathodes are connected to GND.
- **No buzzer sound:** Verify that the positive pin is connected to `D8`, the negative pin is connected to GND, and the buzzer has the required polarity.
- **Distance always reads 999 cm:** Check `TRIG` on `D9`, `ECHO` on `D10`, and the sensor's 5V and GND connections.
- **No Serial Monitor output:** Set the baud rate to `9600` and confirm the correct serial port is selected.

## License

This project is licensed under the MIT License. See the [`LICENSE`](LICENSE) file for details.

***

Made by Baljit Panda ([olympus-mons26](https://github.com/olympus-mons26/welcome-atl-kvpicket))
