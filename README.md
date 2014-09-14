ShipMultiInstrument
===================

Receive and display ship information from multiple serial NMEA sources.

This code is intended for Arduino Mega (2560). The Arduino will display the ship information on a LCD display, the project Wiki describes the details and the wiring.

The overall purpose of this instrument is collect data from multiple NMEA enabled transducers, and to display both the collected data, and data calculated based on the collected data. 

Furthermore, the instrument could:
* Free up space on the chartplotter screen by displaying data that would otherwise be shown there
* Multiplex NMEA inputs and retransmit these on Bluetooth
* Display calculated data that the current instruments cannot


The description of the software
This section first describe the software and its function. The second is about the design decisions, where the most important thoughts are explained.

The software 

The software is structured in five parts.
1) Reception
2) Decoding
3) Calculation
4) Display
5) Retransmission

By design these five parts operate as independently as an Arduino will allow within reason.   

1) Reception
Data is received using interrupt routines. This version receive data on serial port 1 and 2. Each interrupt routine will receive data character by character, and put the characters into a line buffer, with a timestamp. The result is two line buffers, each holding the input of one serial port.

2) Decoding
The main loop of the arduino sketch reads the input buffers holding the serial input. For each line decoding takes place

3) Calculation
Calculation takes place on the newest data just before display. The calculated data is timestamped. [Calculation only takes place if the timestamp for the input values are within the allowed interval (new enough): NOT IMPLEMENTED YET]

4) Display
Each second, the display is rendered to display the current data and calculated data. [Display only takes place if the timestamp for the input values are within the allowed interval (new enough): NOT IMPLEMENTED YET]
Display rendering is called in the main loop by polling the timer.

5) Retransmission
The main loop of the arduino sketch reads the input buffers holding the serial input. For each line retransmission takes place


Design decisions:
I would have preferred the four parts: Reception, decoding, calculation, display and retransmission to be more loosely coupled, and executed independently of each other. However, the Arduino is not well suited for that. In my experience, several small routines - executing independently - are preferable to a long program, in particular when real time response is concerned. Execution is more predictable with the right triggers, it is easier to understand and maintain, since it is loosely coupled to the rest of the code, and the parts can be inspected/understood independently. 

Interrupt routines may not be strictly necessary for reception of data. However interrupts result in code that is loosely coupled and executing independently, and result in the advantages mentioned in the section above.
Furthermore, NMEA data is received at 4800 baud or 38.4 K-baud, and the Arduino UART buffers 64 bytes by default. Interrupts reduce the risk of buffer overrun. 

We prefer not to lose data, because we want to retransmit all data on bluetooth. However, we are not interested in old data for display. So, the interrupt routine for each serial port is stored in a circular line buffer, and each line is time stamped. This way the buffer will hold the data while the Arduino is busy doing other things, like refreshing the display. Should buffer overrun occur, however, the simplest way to handle it is unfortunately to discard the last data received, the newest data. But if data loss this way become a problem, I have to rethink the entire design anyway. 
Using separate line buffers for each serial port, and merging them later result in total isolation between interrupt routines.

Decoding is hardcoded to some extend. Some generic helpers has been made, but the NMEA sentences for different kind of data are very different. It is hard to make a generic and configurable approach within the limited memory and other resources of the Arduino. 

We are not retransmitting calculated data, only displaying them. Therefore we only need to make calculation before we display.

Display is designed to be flexible. Right now things are hard coded, but the using arrays for values to display, and in general decoupling display and data handling, ensures that the display routine can be extended to something more configurable if desired. 

This code contains many defined constants and some hardcoding. In part, this is due to the Arduinos limited memory, in part it is due to my wish to make this instrument without spending too much time right now. When I have seen it work I will improve the code for better maintenance. However, the overall design is prepared for flexibility and expandability. 

TinyGPS++ is a library for decoding NMEA GPS sentences. I really would like to build on something that already exists, and I am considering how to use TinyGPS++ in this project. TinyGPS++ decode NMEA GPS sentences on the fly as they are received character by character, and produce a c++ object holding the data.
I would have to handle buffers of pointers to objects, instead of strings. That is possible.
I would have to make new classes for wind, and other NMEA sentences. That is possible, in my code I will also have to make code changes for new sentences, and calculated values. However, using classes instead of strings will prevent a more generic, configurable, approach to decoding further down the road. 
Also, the retransmission would require non-decoded data to send anyway, unless classes are made for all NMEA sentences.
A display class would probably also be more difficult to make configurable using classes to hold values instead of arrays.
I expect that TinyGPS++ is more efficient for decoding that string operations.

