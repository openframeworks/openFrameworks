this is the arduino sketch used for the serial example

steps to get it to work:

a) setup the arduino with an LED on pin 13 and get the blinking LED example to work
b) then upload this code
c) open the serial monitor and type the letter a.  It something takes 5-15 seconds to for serial i/o to respond.  You should see an LED blink and the letters ABC in the console.
d) then, close the monitor, take note of the port number (ie, "COM7") and then compile the serial example, changing the com port to the right value.
