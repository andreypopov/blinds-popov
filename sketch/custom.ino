void setup_custom_init() {
  setup_stepper(); //освободить мотор, очень важно сразу это сделать
}

void setup_custom() {
  setup_stepper(); //еще раз инициализируем мотор, настройки уже из eeprom
  send_status(); //отправить статус сразу
}

void loop_custom() {  
  loop_stepper();
}

void send_status() {
  //mqttPublish("progress/state", String(STEPPER_STATE)); //отсылка статуса бесполезна, тут всегда 0 будет
  
  send_status_position();
}

void send_status_position() {
  mqttPublishRetained("position/percent", String(positionToPercentStrict(abs(STEPPER_POSITION))));
  if (STEPPER_STATE==0) {
    mqttPublish("position/target", String(positionToPercentStrict(abs(STEPPER_TARGET))));
  }
  //mqttPublish("meta/stepper/current_rpm", String(stepper.getCurrentRPM()));
}
