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
    PWN pin and a potentiometer (pot). The circuit for the TX feather is the same
    as shown on the 
    <a href="https://www.sunfounder.com/learn/Super-Kit-V2-0-for-Arduino/lesson-3-controlling-an-led-by-pwm-super-kit.html">Sunfounder tutorial</a> website.
2.  After these basic examples, the next step is to move on to using the Long
    Range (LoRa) radio. Here we use the Radiohead drivers to handle 
    communications from the Arduino to the LoRa radio on the Feather. First
    script to try out is in the "hello_world" subfolders. For these radio 
    projects, each project folder contains two subfolders for the RX and TX
    codes. 

# Work In Progress
The following projects are not yet implemented and will be in the near future.
*   Interface with Python's serial library to send example LAFTR data 
    (located in the data folder) to the Arduino and have it repeat the message
    back to the host computer.
*   Then use the code from the project above to send the same example data to 
    the feather via serial, and have it transmit that packet to the other 
    feather to be read out by the client computer. Check data rates here.
*   Use the RHReliableDatagram subclass to acknowledge packets sent over the 
    radio to avoid losing data packets in the real world.
*   Command the client computer with the host computer via serial and radio. The
    idea of this project is to command the Raspberry Pi to show what data is
    avaliable for transmission, to have a client request a particular file be
    sent over the radio, and to check the avaliable storage space with the
    capability of deleting data when deemed necessary.