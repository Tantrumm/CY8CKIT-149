# Sine wave generator

This project demonstrates how to generate sine wave signal using PWM on CY8CKIT-149 board and digitize it by ADC with printing in UART. 

All periphery was set through Decive Configurator.

Frequency of generating sine wave: about 2.7 Hz

Frequency of changing PWM: 48 MHz

Frequency of analog to digital converting: 100 Hz


Sine values are stored in array ```sine_table```, which contains 256 values for 1 period of sine wave (range of values: from 0 to 0xFFFF).

## Hardware setup:

![](/images/Picture.jpg "")

Pinout of CY8CKIT-149 pins:

**P3.4** - PWM (output)

**P2.1** - ADC (input)

To digitize sine wave is required low pass filter:

![](/images/Low_pass.jpg "")

Analog to digital converted result in serial port:

![](/images/2k-10uF.jpg "")
 


