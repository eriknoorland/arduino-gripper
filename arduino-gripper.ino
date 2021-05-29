#include "Servo.h"
#include "PacketSerial.h"

const byte REQUEST_START_FLAG = 0xA6;
const byte REQUEST_IS_READY = 0x01;
const byte REQUEST_JAW_ANGLE = 0x02;
const byte REQUEST_LIFT_ANGLE = 0x03;

const byte RESPONSE_START_FLAG_1 = 0xA6;
const byte RESPONSE_START_FLAG_2 = 0x6A;
const byte RESPONSE_READY = 0xFF;

const int JAW_SERVO_PIN = 7;
const int LIFT_SERVO_PIN = 8;

PacketSerial serial;
Servo jaws;
Servo lift;

/**
 * Send the ready response
 */
void isReady() {
  uint8_t readyResponse[4] = {
    RESPONSE_START_FLAG_1,
    RESPONSE_START_FLAG_2,
    RESPONSE_READY,
    0x00
  };

  serial.send(readyResponse, sizeof(readyResponse));
}

/**
 * Serial packet received event handler
 * @param {uint8_t} buffer
 * @param {size_t} size
 */
void onPacketReceived(const uint8_t* buffer, size_t size) {
  byte startFlag = buffer[0];
  byte command = buffer[1];

  if (startFlag == REQUEST_START_FLAG) {
    switch (command) {

      case REQUEST_JAW_ANGLE: {
        jaws.write(constrain(buffer[2], 0, 180));
        break;
      }

      case REQUEST_LIFT_ANGLE: {
        lift.write(constrain(buffer[2], 0, 180));
        break;
      }

      case REQUEST_IS_READY: {
        isReady();
        break;
      }
    }
  }
}

/**
 * Setup
 */
void setup() {
  Serial.begin(115200);

  serial.setStream(&Serial);
  serial.setPacketHandler(&onPacketReceived);

  lift.attach(LIFT_SERVO_PIN);
  jaws.attach(JAW_SERVO_PIN);

  while (!Serial) {}

  isReady();
}

/**
 * Loop
 */
void loop() {
  serial.update();
}
