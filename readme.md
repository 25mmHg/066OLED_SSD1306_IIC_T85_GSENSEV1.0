# cheap 2digit OLED counter for tools in a sheltered workshop
### Motivation
There are many tasks in a workshop requiring the counting of parts. Ideally this would be accomplished during manufacturing.
In our factory we cut small teflon tubes using a drill stand-like tool with a mounted blade and put 50 of them after cutting in a small zip lock bag. There is no way of counting the tubes by weight because they are too small. I thought about counting the movements of the tool lever. An easy way of accomplishing this is using a g-force sensor to measure the angle of the lever relative to earth's gravity. This eliminates the need for additional mechanical parts or end switches. Sticking the g-force sensor on the lever is sufficient.
### Implementation
Because the g-force sensor MMA7361 is an analog device, I can process the output values of the X-axis sensor with an analog lowpass filter. After passing the 10 bit ADC of the ATtiny 85 I use a digital FIR highpass filter with 31 taps for further signal processing. The result is a hybrid bandpass filter with a sharp PASS band of about 0.5 to 8Hz. That's sufficient for our requirements and rejects most of the analog noise and vibrations (lowpass filter) and position issues (highpass filter). 
If the sensor is moving 20° and back within 4 seconds and the filter values pass a higher and a lower threshold in a given time window the counter increments by one. The counter value is displayed on an OLED-display. 
### Hardware and software
Sensor: DFRROBOT MMA7361 x-channel via analog signal
Circuit: look at the .ino
Filter: 3Hz analog 4th order LPF and 0,5Hz FIR HPF with 31 taps and 16Hz sample rate
Display: AZ-Delivery 0,66 64x48 pixel monocolor Oled with SSD1306 controller via I2C
Display-processing and I2C: U8x8 part of U8g2 Arduino library via library manager
VCC: 3.3V
MCU: Microchip ATtiny 85 at 8MHz (intern RC-oscillator)
IDE: Arduino IDE 1.8.51 with attiny package  via https://github.com/damellis/attiny
Programmer mySmartUSB light as STK500 Dev.board at VCC=3V
