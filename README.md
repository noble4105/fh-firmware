# FH-Firmware

Co-written by Christopher Wells and Nicholas Peterson

This is software written for a college project, the choice of the project was left for the students
to decide. In this case, we chose a remote locator that displays the proximity to the nearest
other device. Due to a two month time limit, many features we would want to add are unable to be 
implemented. 

The software runs on the Heltec Wifi Lora V3 esp32 microcontroller, and is written in C language. 
As of March 24th (updated), it can reliably assess signal strength and convert it to distance then 
average multiple samples and display it on the OLED screen. 

A single button input has been implemented with two input modes: short press to switch between
display styles while another device is in range, and long press to turn the device on or off.
There is a timeout function; once no devices are detected the counter begins and if the timeout
is exceeded, the short press will no longer be operational and a message is displayed on the 
screen to indicate no other devices found. In addition, there is a power on indicator LED, and
the device has officially been switched to battery powered. Ideally, the display will indicate
the battery level at all times while powered on but we have yet to implement it.
