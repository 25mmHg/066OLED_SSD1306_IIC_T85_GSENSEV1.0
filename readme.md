# cheap 2digit OLED counter for tools in a social manufactury
### Why
There are many tasks in a manufactury with the needing of counting objects after process steps.
In Our factury We cut small teflon tubes with a tool like a drill stand with a mounted blade and put 50 of them after cutting in a small zip lock bag. There are no ways for counting the tubes by weight, because they are too small. So I think about counting the action of the tool lever. Easy is an position-controlled counting with a G-force sensor, because there is no needing of additional mechanical parts or end switches. Sticking the G-force sensor on the lever is enough.
### How
Because the G-force sensor MMA7361 is an analogue one, I can process the output values of the X-axis sensor by an analogue Lowpass-filter. After passing the 10 bit ADC of the ATtiny 85 I use a digital FIR Highpass-filter with 31 taps for further signal processing. The result is a hybrid BP-filter with a sharp PASS-band of about 0.5 to 8Hz. That's fine for Our needs and rejects the most of the analogue noise and vibrations and otherwise position issues. 
If the sensor is about 20° moving and back within 4 seconds, the filtervalues passes a higher and a lower Threshold in a time window and then the counter increments. The counter value is displayed on an OLED-display. 
### Hardware and software
**Sensor:** DFRROBOT MMA7361 X-channel via analog signal
**Circuit:** look at the .ino
**Filter:** 3Hz analog 4th order LPF and 0,5Hz FIR HPF with 31 taps and 16Hz sample rate
**Display:** AZ-Delivery 0,66 64x48 pixel monocolor Oled with SSD1306 controller via I2C
**Display-processing and I2C:** U8x8 part of U8g2 Arduino library via library manager
**VCC:** 3.3V
**MCU:** Microchip ATtiny 85 at 8MHz (intern RC-oscillator)
**IDE:** Arduino IDE 1.8.51 with attiny package  via https://github.com/damellis/attiny
**Programmer** mySmartUSB light as STK500 Dev.board at **VCC=3V**
