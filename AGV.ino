#include <Arduino_FreeRTOS.h>
#include <ModbusMaster.h>
// Khai báo tasks:
// void TaskBlink(void *pvParameters);
void TaskAnalogRead(void *pvParameters);
void TaskModbusConnect(void *pvParameters);
// ---------------------
ModbusMaster node;
#define MAX485_DE 16
#define MAX485_RE 17

// These constants won't change. They'MAX485_RE used to give names to the pins used:
// const int acs712_Pin A2;
// const int battery_V_Pin A1;
// const int battery1_V_Pin A0;
// const int charging_Out_A_Pin A5;
float U41, U43, U49, U55, U61, u_Battery_V_All, u_Battery1_V_All, acs712_Volt, charging_Out_A;
int battery_V_Value, battery1_V_Value, acs712_Value, charging_Out_A_Value;

void preTransmission() {
  digitalWrite(MAX485_DE, 1);
  digitalWrite(MAX485_RE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_DE, 0);
  digitalWrite(MAX485_DE, 0);
}

void initPinMode() {
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  pinMode(A5, INPUT);
  pinMode(MAX485_DE, OUTPUT);
  pinMode(MAX485_RE, OUTPUT);
  // init in receive mode
  digitalWrite(MAX485_DE, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
  // Init pinMode
  initPinMode();
  // Initialize task
  xTaskCreate(
    TaskAnalogRead, "AnalogRead"  // A name for task
    ,
    128  // Stack size
    ,
    NULL  //Parameters for the task
    ,
    2  // Priority
    ,
    NULL);  //Task Handle

  xTaskCreate(
    TaskModbusConnect, "ModbusRead"  // A name for task
    ,
    128  // Stack size
    ,
    NULL  //Parameters for the task
    ,
    1  // Priority
    ,
    NULL);  //Task Handle
  // communicate with Modbus slave ID 1 over Serial (port 0)
  node.begin(1, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}
void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskAnalogRead(void *pvParameters __attribute__((unused)))  // This is a Task.
{
  // read the input on analog pin 0:
  int battery_V_Value = analogRead(A1);
  int battery1_V_Value = analogRead(A0);
  int acs712_Value = analogRead(A2);
  int harging_Out_A_Value = analogRead(A5);
  for (;;) {
    // Chuyển đổi giá trị đọc analog từ battery_V_Value sang giá trị điện áp
    U41 = (battery_V_Value / 18000 + 20000 + 10000) * 18000;
    U43 = (battery_V_Value / 18000 + 20000 + 10000) * 20000;
    U49 = (battery_V_Value / 18000 + 20000 + 10000) * 10000;
    u_Battery_V_All = U41 + U43 + U49;

    // Chuyển đổi giá trị đọc analog từ battery1_V_Value sang giá trị điện áp
    U55 = (battery1_V_Value / 12000 + 10000) * 12000;
    U61 = (battery1_V_Value / 12000 + 10000) * 10000;
    u_Battery1_V_All = U55 + U61;

    // Chuyển đổi giá trị đọc từ ACS712 sang giá trị điện áp
    acs712_Volt = (acs712_Value / 1023) * 5;
    // Chuyển đổi giá trị đọc từ Charging Out sang giá trị điện áp
    charging_Out_A = (charging_Out_A_Value / 1023) * 5;

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskModbusConnect(void *pvParameters __attribute__((unused)))  // This is a Task.
{
  uint8_t resultMain = node.readInputRegisters(0x3100, 6);
  for (;;) {
    if(resultMain == node.ku8MBSuccess){
      Serial.println("Connect MODBUS successful!");
    } else {
      Serial.println("Failed to connect MODBUS! Check again");

    }
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}
