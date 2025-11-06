/*
File: q1.cpp
Triggered when a specific CAN frame is received
    Analog read PIN_PC0,
    Send the reading over CAN.
While this happens, make an LED turn on to indicate activity.
    This LED is connected to PIN_PD2.
The CAN frames are received and sent via an MCP2515 CAN controller.
    It is connected via SPI with CS on PIN_PB2.
    The crystal is 20MHz and the CAN speed is 500kbps.
*/

// include the necessary libraries
#include <Arduino.h>
#include <mcp2515.h>

// create MCP2515 object
MCP2515 canController(PIN_PB2);/* todo 1 */

void setup() {
    // set the pinMode on the input pin
    pinMode(PIN_PC0, INPUT);/* todo 1 */
    // set the pinMode on the LED pin
    pinMode(PIN_PC2, OUTPUT);/* todo 1 */
    // initialize the CAN controller at 500kbps
    canController.reset();  /* todo 1 */
    canController.setBitrate(CAN_500KBPS, MCP_20MHZ);
    canController.setNormalMode(); 
}

void loop() {
    // create CAN frame objects
    // make one for receiving, another for transmitting
    struct can_frame txFrame;
    struct can_frame rxFrame;
    /* todo 2 */

    // check if a CAN frame is received
    if (canController.readMessage(&rxFrame) == MCP2515::ERROR_OK) {
        // check if the received frame is of ID 0x420 and first data byte is 0x69
        if (rxFrame.can_id == 0x420 && rxFrame.data[0] == 0x69) {
            // turn on the LED to indicate activity
            /* todo 3 */
            digitalWrite(PIN_PB2, HIGH);

            // read the analog value from the input pin and store it as a variable
            /* todo 3 */
            uint16_t analogValue = analogRead(PIN_PC0);

            // prepare the CAN frame to send
            txFrame.can_id = 0x690; // replace with desired send ID
            txFrame.can_dlc = 2; // data length code
            txFrame.data[0] = (analogValue >> 8) & 0xFF; // high byte
            txFrame.data[1] = analogValue & 0xFF; // low byte

            // send the CAN frame
            /* todo 3 */
            canController.sendMessage(&txFrame);
            // turn off the LED after sending
            /* todo 3 */
            digitalWrite(PIN_PB2, LOW);
        }
    }
}