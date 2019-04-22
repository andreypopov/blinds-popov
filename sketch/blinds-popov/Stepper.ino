void setup_stepper() {
  stepper.begin(MOTOR_RPM, MOTOR_MICROSTEPS);
  stepper.enable(); // disable! 
  stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  STEPPER_TARGET = STEPPER_POSITION;
}

//чисто калибровочная функция, если надо подтянуть штору
void stepper_move(long int steps) {
    STEPPER_TARGET = STEPPER_POSITION + steps;
    mqttPublish("position/target", String(positionToPercentStrict(STEPPER_TARGET)));
    STEPPER_STATE = steps>0?1:-1;
    STEPPER_TIMER = millis();
    
    stepper.disable(); //enable!!
    stepper.startMove((STEPPER_REVERSE?-1:1)*steps);
}

void stepper_move_to(long int pos) {
  STEPPER_TARGET = pos;
  mqttPublish("position/target", String(positionToPercentStrict(STEPPER_TARGET)));


  long int steps = pos - STEPPER_POSITION;

  if (steps != 0) {
    if (STEPPER_STATE!=0) {
      //if ((STEPPER_STATE == -1 && steps>0) || (STEPPER_STATE == 1 && steps<0)) {
        //stepper.startBrake();
        //stepper.enable(); // disable! 
        //delay(2000);
      //}
      stepper.stop(); //sets remaining steps to 0
    } 
    
    STEPPER_STATE = steps>0?1:-1;
    STEPPER_TIMER = millis();
    
    stepper.disable();
    stepper.startMove((STEPPER_REVERSE?-1:1)*steps);
  }
}

void stepper_stop() {
  if (STEPPER_TIMER) {
    STEPPER_TIMER_LAST = millis()-STEPPER_TIMER;
    mqttPublish("progress/timer", String(STEPPER_TIMER_LAST));
  } 
   
  STEPPER_STATE = 0;
  STEPPER_TIMER = 0;

  stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL*10, MOTOR_DECEL*10); //?? может не работает
  //stepper.stop(); //это не останавливает мотор быстро
  stepper.startBrake();
  stepper.enable(); //disable!!
  stepper.setSpeedProfile(stepper.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);  //restore accell profile

  STEPPER_TARGET = STEPPER_POSITION;
  eeWriteInt(EEaddr__STEPPER_POSITION, STEPPER_POSITION);
  debug("Stepper STOP position:" + STEPPER_POSITION);
 
  send_status();
}

//функция отдает процент открытия шторы исходя из количества шагов
int positionToPercentStrict(long int val) {
  double valf = abs(val);
  double result = 0.00;
  if (valf < stepper_calibration[1]) {
    float tmp = (10.0 - (stepper_calibration[1] - valf) / (stepper_calibration[1] - 0.0) * 10.0);
    result = tmp > 0 ? tmp : 0;
  }
  else if (valf < stepper_calibration[2]) result = 10.0 + (10.0 - ((stepper_calibration[2] - valf) / (stepper_calibration[2] - stepper_calibration[1])) * 10.0);
  else if (valf < stepper_calibration[3]) result = 20.0 + (10.0 - ((stepper_calibration[3] - valf) / (stepper_calibration[3] - stepper_calibration[2])) * 10.0);
  else if (valf < stepper_calibration[4]) result = 30.0 + (10.0 - ((stepper_calibration[4] - valf) / (stepper_calibration[4] - stepper_calibration[3])) * 10.0);
  else if (valf < stepper_calibration[5]) result = 40.0 + (10.0 - ((stepper_calibration[5] - valf) / (stepper_calibration[5] - stepper_calibration[4])) * 10.0);
  else if (valf < stepper_calibration[6]) result = 50.0 + (10.0 - ((stepper_calibration[6] - valf) / (stepper_calibration[6] - stepper_calibration[5])) * 10.0);
  else if (valf < stepper_calibration[7]) result = 60.0 + (10.0 - ((stepper_calibration[7] - valf) / (stepper_calibration[7] - stepper_calibration[6])) * 10.0);
  else if (valf < stepper_calibration[8]) result = 70.0 + (10.0 - ((stepper_calibration[8] - valf) / (stepper_calibration[8] - stepper_calibration[7])) * 10.0);
  else if (valf < stepper_calibration[9]) result = 80.0 + (10.0 - ((stepper_calibration[9] - valf) / (stepper_calibration[9] - stepper_calibration[8])) * 10.0);
  else if (valf < STEPPER_MAX_STEPS - STEPS_PRECISION) result = 90.0 + (10.0 - ((STEPPER_MAX_STEPS - valf) / (STEPPER_MAX_STEPS - stepper_calibration[9])) * 10.0);
  else if (valf >= STEPPER_MAX_STEPS - STEPS_PRECISION) result = 100;

  if (PERCENT_REVERSE) {
    result = 100 - result;
  }

  return result;
}

void loop_stepper() {  
  int current_state = stepper.getCurrentState();
 
  unsigned wait_time = stepper.nextAction();
  if (wait_time){
      if (STEPPER_STATE > 0) {
        step++;
      } else if (STEPPER_STATE < 0) {
        step--;
      }
      STEPPER_POSITION = step;
  } else {
    if (STEPPER_STATE && current_state == 0) {
      stepper_stop();
    }
  }
  
  //send status
  //штатная отправка статуса, во время покоя шторы
  static unsigned long nextStatusTimeMinute = millis() + SEND_STATUS_INTERVAL*1000L;
  if (STEPPER_STATE == 0 && nextStatusTimeMinute < millis() ) {
    nextStatusTimeMinute = millis() + SEND_STATUS_INTERVAL*1000L;
    send_status();
  }

  //when moving - slows motor
  //отправка данных на мнгновение замедляет мотор и он теряет крутящий момент
  //это слышно по характерному звуку
  //если отправлять мало данных, работает сносно
  if (SEND_WHILE_WORKING) {
    static unsigned long nextStatusTimeChange = millis() + 1000L;
    if (STEPPER_STATE != 0 && nextStatusTimeChange < millis() ) {
      nextStatusTimeChange = millis() + 1000L;
      send_status_position();
      //debug("step: "+step);
    }
  }

  //stepper protection
  //дополнительная защита мотора, он не должен долго работать подряд
  if (STEPPER_TIMER > 0 && millis()-STEPPER_TIMER > 180000) {
    debug("Stepper STOPPED, overheat protection.");
    stepper_stop();
  }
}

