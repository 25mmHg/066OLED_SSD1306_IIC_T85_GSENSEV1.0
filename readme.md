# Howto OLED G-force counter for sozial enterprices
### Why
There are many jobs in a manufacture with the needing of counting objects after prozess steps.
In our fab we cut small tubes with a tool like a drill stand and put them in a small plastic bag.
There are no ways for counting the tubes by weight, because they are too small. So I think about counting the move of the tool lever. Easy is an position-controlled counting with a G-force sensor. Then there is no needing of additional mechanical parts. Sticking the G-force sensor on the lever is enough.
### How
Because the G-force sensor MMA7361 is an analogue one, I can process the output values of the sensor by an analogue LP-filter and later after passing the 10 bit ADC I use a digital FIR HP-filter with 31 taps for further processing. The result is a hybrid BP-filter with a sharp PASS-band of about 0.5 to 8Hz.
That's fine for Our needs and reject some analogue noise and vibrations and also position issues. 
If the sensor is about 20° moving and back within 2 seconds, the filtervalues passes an higher and an lower Threshold in a time window and then the counter increments.
The counter value is displayed on an OLED-display. 
### Hardware and software
Sensor: DFRROBOT MMA7361 x-channel via analog signal
Circuit: look at the .ino
Filter: 3Hz analog 4th order LPF and 0,5Hz FIR HPF with 31 taps
Display: AZ-Delivery 0,66 Oled via I2C
VCC: 3.3V
MCU: Microchip ATtiny 85 at 8MHz (intern RC-oscillator)
IDE: Arduino IDE 1.8.51 with attiny package  via https://github.com/damellis/attiny


