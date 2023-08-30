# About serialExample

![Screenshot of serialExample](serialExample.png)


### Learning Objectives

This example demonstrates communication with external devices over the serial port. In this example, you will connect to a serial device, read incoming byte packets from the connected device, and send response messages over serial.

This example covers the following functions:

* Connecting to a serial device
* Reading incoming byte packets from the connected device
* Sending response messages over serial
* Updating graphics based on the incoming byte information

In the code, pay attention to:

* Listing all serial devices with ```serial.listDevices()```, which logs a list to the console. Make sure the port ID you specify is the same that your Arduino or other device is connected on.
* When a device is connected, check the console logs to ensure that you're not getting any Serial connection errors and have successfully connected to the specified port
* To signal successful communication back to the serial device, send the trigger message that was typed: ```serial.writeBytes( messageToSend.c_str(), (int)messageToSend.size() );```
* Read incoming messages with ```serial.readBytes(serialReadBuffer, numBytesToRead );```
* Store incoming messages in a vector ```receivedSerialMessages``` to match the set of data being sent from the serial device. This can be changed to match the size of discrete data packets being sent from serial device.


### Expected Behavior

When launching this app, you should see a white screen with text showing the following information:

* Serial connected status string
* Device list
* Message to send
* Received Messages

### Instructions for use:

* Type a serial message and then hit enter to send a communication trigger to a serial device and receive a serial packet in return. The displayed values onscreen should change to reflect the received data.

### Other classes used in this file

This example uses no other classes.
