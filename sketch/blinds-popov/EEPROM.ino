void setup_eeprom() {
  EEPROM.begin(512);

  if (eeGetInt(EEaddr__MOTOR_STEPS) > 0) {
    MOTOR_STEPS = eeGetInt(EEaddr__MOTOR_STEPS);
  }
  if (eeGetInt(EEaddr__MOTOR_MICROSTEPS) > 0) {
    MOTOR_MICROSTEPS = eeGetInt(EEaddr__MOTOR_MICROSTEPS);
  }
  if (eeGetInt(EEaddr__MOTOR_ACCEL) > 0) {
    MOTOR_ACCEL = eeGetInt(EEaddr__MOTOR_ACCEL);
  }
  if (eeGetInt(EEaddr__MOTOR_DECEL) > 0) {
    MOTOR_DECEL = eeGetInt(EEaddr__MOTOR_DECEL);
  }
  if (eeGetInt(EEaddr__MOTOR_RPM) > 0) {
    MOTOR_RPM = eeGetInt(EEaddr__MOTOR_RPM);
  }
  if (eeGetInt(EEaddr__STEPPER_POSITION) > 0) {
    step = STEPPER_POSITION = eeGetInt(EEaddr__STEPPER_POSITION);
  }
  if (eeGetInt(EEaddr__STEPPER_MAX_STEPS) > 0) {
    STEPPER_MAX_STEPS = eeGetInt(EEaddr__STEPPER_MAX_STEPS);
  }
  if (eeGetInt(EEaddr__STEPPER_REVERSE) >= 0) {
    STEPPER_REVERSE = eeGetInt(EEaddr__STEPPER_REVERSE);
  }
  if (eeGetInt(EEaddr__PERCENT_REVERSE) >= 0) {
    PERCENT_REVERSE = eeGetInt(EEaddr__PERCENT_REVERSE);
  }
  if (eeGetInt(EEaddr__SEND_WHILE_WORKING) >= 0) {
    SEND_WHILE_WORKING = eeGetInt(EEaddr__SEND_WHILE_WORKING);
  }
  if (eeGetInt(EEaddr__SEND_STATUS_INTERVAL) > 0) {
    SEND_STATUS_INTERVAL = eeGetInt(EEaddr__SEND_STATUS_INTERVAL);
  }

  debug("EEPROM ready");
}

void eeWriteFloat(int pos, float val) {
  if (val != eeGetFloat(pos)) {
    byte *x = (byte *)&val;
    for (byte i = 0; i < 4; i++) EEPROM.write(i + pos, x[i]);
  }
}

float eeGetFloat(int addr) {
  byte x[4];
  for (byte i = 0; i < 4; i++) x[i] = EEPROM.read(i + addr);
  float *y = (float *)&x;
  return y[0];
}

void eeWriteInt(int pos, int val) {
  byte* p = (byte*) &val;
  EEPROM.write(pos, *p);
  EEPROM.write(pos + 1, *(p + 1));
  EEPROM.write(pos + 2, *(p + 2));
  EEPROM.write(pos + 3, *(p + 3));
  EEPROM.commit();
}

int eeGetInt(int pos) {
  int val;
  byte* p = (byte*) &val;
  *p        = EEPROM.read(pos);
  *(p + 1)  = EEPROM.read(pos + 1);
  *(p + 2)  = EEPROM.read(pos + 2);
  *(p + 3)  = EEPROM.read(pos + 3);
  return val;
}
