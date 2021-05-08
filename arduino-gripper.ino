#include "Servo.h"
#include "PacketSerial.h"

const byte REQUEST_START_FLAG = 0xA6;
const byte REQUEST_IS_READY = 0x01;
const byte REQUEST_CONFIG = 0x02;
const byte REQUEST_OPEN = 0x03;
const byte REQUEST_CLOSE = 0x04;
const byte REQUEST_LIFT = 0x05;
const byte REQUEST_LOWER = 0x06;

const byte RESPONSE_START_FLAG_1 = 0xA6;
const byte RESPONSE_START_FLAG_2 = 0x6A;
const byte RESPONSE_READY = 0xFF;

const int JAW_SERVO_PIN = 9;
const int LIFT_SERVO_PIN = 10;

int jawCloseAngle = 0;
int jawOpenAngle = 0;
int liftUpAngle = 0;
int liftDownAngle = 0;

Servo jaws;
Servo lift;
PacketSerial serial;

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
      case REQUEST_CONFIG: {
        jawCloseAngle = buffer[2];
        jawOpenAngle = buffer[3];
        liftUpAngle = buffer[4];
        liftDownAngle = buffer[5];
        break;
      }

      case REQUEST_OPEN: {
        if (jawOpenAngle != 0) {
          jaws.write(jawOpenAngle);
        }
        break;
      }

      case REQUEST_CLOSE: {
        if (jawCloseAngle != 0) {
          jaws.write(jawCloseAngle);
        }
        break;
      }

      case REQUEST_LIFT: {
        if (liftUpAngle != 0) {
          lift.write(liftUpAngle);
        }
        break;
      }

      case REQUEST_LOWER: {
        if (liftDownAngle != 0) {
          lift.write(liftDownAngle);
        }
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
