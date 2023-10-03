#include <Arduino.h>
#include <CAN.h>
#include <CAN_config.h>
#include "setting.h"

#define MSG_ID 0

CAN_device_t CAN_cfg;

void setup()
{
    Serial.begin(Setting::UART::BAUDRATE);
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_35;
    CAN_cfg.speed = CAN_SPEED_1000KBPS;
    CAN_cfg.rx_queue = xQueueCreate(1, sizeof(CAN_frame_t));

    CAN_init();
}

void loop()
{
    CAN_frame_t frame{0};

    frame.MsgID = MSG_ID;
    frame.FIR.B.RTR = CAN_no_RTR;
    frame.FIR.B.FF = CAN_frame_std;
    frame.FIR.B.DLC = Setting::Signal::BUFSIZE;

    if (pdTRUE == xQueueReceive(CAN_cfg.rx_queue, &frame, portMAX_DELAY))
    {
        Serial.write(frame.data.u8, Setting::Signal::BUFSIZE);
    }
}