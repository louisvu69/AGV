// These constants won't change. They're used to give names to the pins used:
const int acs712_Pin = A2;
const int battery_V_Pin = A1;
const int battery1_V_Pin = A0;
const int charging_Out_A_Pin = A5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Đọc giá trị điện áp từ chân A0, A1, A2, A5
  int battery_V_Value = analogRead(battery_V_Pin);
  int battery1_V_Value = analogRead(battery1_V_Pin);
  int acs712_Value = analogRead(acs712_Pin);
  int charging_Out_A_Value = analogRead(charging_Out_A_Pin);

  // Chuyển đổi giá trị đọc analog từ battery_V_Value sang giá trị điện áp
  float U41 = (battery_V_Value / 18000 + 20000 + 10000) * 18000;
  float U43 = (battery_V_Value / 18000 + 20000 + 10000) * 20000;
  float U49 = (battery_V_Value / 18000 + 20000 + 10000) * 10000;
  float u_Battery_V_All = U41 + U43 + U49;

  // Chuyển đổi giá trị đọc analog từ battery1_V_Value sang giá trị điện áp
  float U55 = (battery1_V_Value / 12000 + 10000) * 12000;
  float U61 = (battery1_V_Value / 12000 + 10000) * 10000;
  float u_Battery1_V_All = U55 + U61;

  // Chuyển đổi giá trị đọc từ ACS712 sang giá trị điện áp
  float acs712_Volt = (acs712_Value / 1023) * 5;
  // Chuyển đổi giá trị đọc từ Charging Out sang giá trị điện áp
  float charging_Out_A = (charging_Out_A_Value / 1023) * 5;
  
  // In giá trị lên Serial Monitor
  
  delay(1000);
}
