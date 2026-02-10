# DIY USB Game Pedals with Arduino & Logitech G29 Wiring

This project turns **three potentiometers** into fully functional USB game pedals (clutch, brake, accelerator) using an Arduino and the [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary).  
It includes **Gaussian smoothing** for stable input, **auto-calibration** for min/max pedal travel, and an example **KiCad wiring diagram** for connecting Logitech G29 pedals.

---

## ✨ Features

- **3 Analog Pedals** mapped to USB joystick axes:
  - **Rx** → Pedal 1 (e.g., Clutch)
  - **Ry** → Pedal 2 (e.g., Brake)
  - **Rz** → Pedal 3 (e.g., Accelerator)
- **Gaussian smoothing filter** to reduce noise and jitter
- **Automatic min/max calibration** at startup and during use
- **Compatible with Logitech G29 pedal wiring** (KiCad schematic included)
- **Plug-and-play USB HID device** — no drivers needed on Windows

---

## 🛠 Hardware Requirements

- **Arduino Leonardo**, Micro, or any board with native USB HID support  
  *(Uno/Nano will not work without extra firmware flashing)*
- **3x potentiometers** (e.g., from Logitech G29 pedals)
- **Wiring** (Dupont cables or soldered connections)
- **PC with USB port**
- *(Optional)* Logitech G29 pedal set for reuse

---

## 📐 Wiring

The potentiometers are connected to the Arduino’s analog pins:

| Pedal   | Arduino Pin |
|---------|-------------|
| Pedal 1 | A0          |
| Pedal 2 | A1          |
| Pedal 3 | A2          |

Each potentiometer also connects to **5V** and **GND**.

If using Logitech G29 pedals, refer to the included [**KiCad schematic**](/hardware/kicad/g29pedals/schematic.jpg) for the correct pinout.

---

## 💻 Software Setup

1. **Install Arduino IDE**
2. **Install Joystick Library**  
   - Open Arduino IDE → Sketch → Include Library → Manage Libraries  
   - Search for `Joystick` by Matthew Heironimus and install
3. **Select Board**  
   - Tools → Board → Arduino Leonardo (or compatible)
4. **Upload the Code**  
   - Flash the provided `.ino` file to your Arduino

---

## ⚙️ How It Works

### Gaussian Smoothing
The code uses a **5-point Gaussian kernel** to smooth pedal readings:

```cpp
kernel[i] = exp(-(pow(x, 2)) / (2 * pow(sigma, 2)));
