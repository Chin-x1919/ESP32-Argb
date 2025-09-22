## ⚠️ This doesn't work — if you want a working project, this isn't for you ⚠️

# How It Should Work

1. The ESP32 creates its own Wi-Fi network (`ESP32_RGB`) that you can connect to with a phone or PC.  
2. When you open `192.168.4.1`, the ESP32 serves an HTML page with buttons.  
3. Clicking a button sends an HTTP request to the ESP32.  
4. The ESP32 code (using the Adafruit_NeoPixel library) updates the ARGB LEDs connected to a GPIO pin.  

# Hardware Setup

- ESP32 GPIO (example: GPIO4) → ARGB Data pin  
- ESP32 GND → ARGB GND  
- 5V from PSU (or motherboard ARGB header) → ARGB VCC (5V)  

# Why It Doesn't Work

### Power Supply Issue
- The ESP32 USB port only provides 3.3V–5V with very limited current.  
- Addressable RGB LEDs often need **5V and a lot of current** (up to 60mA per LED at full brightness white).  
- If you try to run many LEDs directly from the ESP32, they won’t light up, flicker, or behave incorrectly.  

### Voltage Mismatch
- ESP32 outputs 3.3V logic on its GPIO pins.  
- WS2812/SK6812 LEDs are designed for 5V power and ~5V logic.  
- Sometimes the 3.3V signal works, sometimes it doesn’t → no response.  
- Solution: use a **level shifter (3.3V → 5V)** or run the LEDs at 3.3V (dim, unreliable but okay for testing code).  

### Pin Size Issue
- ARGB 3-pin connector uses: `[5V | Data | GND]`.  
- Standard jumper wires are **slightly too big** for ARGB headers → bad contact, random failure.  

# License
MIT License — just fork it, remake it, or whatever.  
