# feather_32u4_LoRa

Small projects to explore how to program the Feather 32u4 and the LoRa RF95 
radio. A few useful references for the radio are
*   <a href="https://www.airspayce.com/mikem/arduino/RadioHead/">RadioHead Arduino</a>
*   <a href="https://www.airspayce.com/mikem/arduino/RadioHead/classRHGenericDriver.html">RadioHead GenericDriver</a>
*   <a href="https://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html">RadioHead LoRa radio driver (RF95)</a>
*   <a href="https://www.airspayce.com/mikem/arduino/RadioHead/classRHReliableDatagram.html">RadioHead Reliable Datagram</a> (to acknowledge datagrams i.e. packets).


# Examples
1.  To start out, program the feather Arduino with scripts in the 
    "serial_commands" folder to rurn led on pin 13 on/off via the computer's
    serial console, and "led_pot/led_pot" to change an LED's brightness using a
    PWN pin and a potentiometer (pot). 
2.  After these basic examples, the next step is to move on to using the Long
    Range (LoRa) radio. Here we use the Radiohead drivers to handle 
    communications from the Arduino to the LoRa radio on the Feather. First
    script to try out is in the "hello_world" subfolders. For these radio 
    projects, each project folder contains two subfolders for the RX and TX
    codes. 
    