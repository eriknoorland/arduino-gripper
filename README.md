# Arduino Gripper
Firmware to manage two servo motors with an Arduino Nano communicating over USB using COBS encoding.

## Packet Descriptions

### Request Packet Format

| Start Flag | Command | Payload Size |
|------------|---------|--------------|
| 1 byte     | 1 byte  | x bytes      |

### Response Packet Format

| Start Flag 1 | Start Flag 2 | Command | Response Data Length | Response |
|--------------|--------------|---------|----------------------|----------|
| `0xA6`       | `0x6A`       | 1 byte  | 1 byte               | x bytes  |

### Requests Overview

| Request  | Value  | Payload                                                                                              |
|----------|--------|------------------------------------------------------------------------------------------------------|
| IS_READY | `0x01` | N/A                                                                                                  |
| CONFIG   | `0x02` | jaw open angle (1 byte), jaw close angle (1 byte), lift lower angle (1 byte), lift up angle (1 byte) |
| OPEN     | `0x03` | N/A                                                                                                  |
| CLOSE    | `0x04` | N/A                                                                                                  |
| LIFT     | `0x05` | N/A                                                                                                  |
| LOWER    | `0x06` | N/A                                                                                                  |

#### Is ready Request
Request: `0xA6` `0x01`

Triggers the ready response to make sure the Arduino is ready for operation.

#### Config Request
Request: `0xA6` `0x02` `0x[jaw open angle]` `0x[jaw close angle]` `0x[lift lower angle]` `0x[lift up angle]`

Triggers the ready response to make sure the Arduino is ready for operation.

#### Open Request
Request `0xA6` `0x03`

Sets the jaw servo to the configured angle

#### Close Request
Request `0xA6` `0x04`

Sets the jaw servo to the configured angle

#### Lift Request
Request `0xA6` `0x05`

Sets the lift servo to the configured angle

#### Lower Request
Request `0xA6` `0x06`

Sets the lift servo to the configured angle


### Responses Overview

| Request  | Value  | Payload |
|----------|--------|---------|
| IS_READY | `0xFF` | N/A     |

#### Ready Response
**Response:** `0xA6` `0x6A` `0xFF` `0x00`

This response will be sent when the Teensy is ready to be controlled.
