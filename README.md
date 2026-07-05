# Water Level Monitor for Arduino Uno

A real-time water level monitoring system built with an **Arduino Uno**, an **analog water level sensor**, and a **0.96" SSD1306 OLED display**.

The project uses a custom calibration table and an exponential digital filter to provide stable and accurate level measurements. The OLED display presents both a graphical representation of the tank and the current water level percentage.

---

## Features

- Real-time water level monitoring
- Arduino Uno compatible
- Analog water level sensor
- 0.96" I2C OLED display (SSD1306)
- Custom calibration table
- Linear interpolation between calibration points
- Exponential digital filtering for stable readings
- Non-blocking programming using `millis()`
- Graphical tank level indicator
- Serial Monitor output for debugging

---

## Hardware Required

- Arduino Uno
- Analog Water Level Sensor
- 0.96" OLED Display (SSD1306, I2C)
- Breadboard
- Jumper wires
- USB cable

---

## Libraries

Install the following libraries using the Arduino IDE Library Manager:

- Adafruit GFX
- Adafruit SSD1306

---

## Wiring

| OLED Display | Arduino Uno |
|--------------|-------------|
| VCC | 3.3V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

### Water Level Sensor

| Sensor | Arduino Uno |
|---------|-------------|
| VCC | 5V |
| GND | GND |
| S | A0 |

<img width="4160" height="3120" alt="sensor_level water" src="https://github.com/user-attachments/assets/0db5ac49-d6bb-4876-bbfe-b0cb098c11c6" />

---

## Display

The OLED interface shows:

- Water tank animation
- Water level percentage
- Tank status
    - EMPTY
    - LOW
    - NORMAL
    - HIGH
    - FULL
- Raw ADC value

---

## Calibration

The sensor is calibrated using experimentally obtained ADC values.

```cpp
static const int adcCal[] = {
    65,
    310,
    340,
    350,
    400
};

static const int nivelCal[] = {
     0,
    25,
    50,
    75,
   100
};
```

Linear interpolation is performed between calibration points to improve measurement accuracy.

---

## Signal Filtering

An exponential moving average filter is implemented:

```cpp
sensorFiltered =
(1.0 - FILTER_ALPHA) * sensorFiltered +
FILTER_ALPHA * sensorReading;
```

This significantly reduces noise while maintaining a fast response.

---

## Main Characteristics

- Fast execution
- No delays (`delay()`)
- Smooth OLED updates
- Efficient memory usage
- Easy to modify calibration table
- Modular and well-documented code

---

## Project Structure

```
Water_Level_Monitor.ino
README.md
LICENSE
```

---

## Demonstration

<img width="720" height="1612" alt="image_1" src="https://github.com/user-attachments/assets/c87fa258-a1a1-4ae9-a6b2-99da9736c62d" />


---

## Source Code

The complete Arduino sketch is available in this repository.

---

## License

This project is released under the MIT License.

Feel free to use, modify and distribute it.

---

## Author

Developed by Jhonatan Franco.

If you found this project useful, consider giving it a ⭐ on GitHub.
