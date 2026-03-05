# FH-Firmware

Co-written by Christopher Wells and Nicholas Peterson

This is software written for a college project, the choice of the project was left for the students
to decide. In this case, we chose a remote locator that displays the proximity to the nearest
other device. Due to a two month time limit, many features we would want to add are unable to be 
implemented. 

The software runs on the Heltec Wifi Lora V3 esp32 microcontroller, and is written in C language. 
As of February 21st (the time of writing this), it continues to be updated as progress continues.
In its state at the time of writing this, it can successfully assess an approximate distance 
between the two devices using perceived signal strength then display the distance on the built-in
OLED display. 

Some functions have been added to convert the signal strength into a non-logarithmic scale for a
more intuitive visual on the screen as well as a time estimate between received pings. Our intent 
is to average out the calculated distance between time and signal strength data for a more accurate
and reliable distance. As of the time of writing this, the functions in this paragraph are untested.

