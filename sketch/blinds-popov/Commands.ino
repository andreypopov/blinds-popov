void exec_command(String control, String argument)
{  
  if (control.equalsIgnoreCase("status")) {
    send_status();
  } else if(control.equalsIgnoreCase("stepper")) {
    if (argument.equalsIgnoreCase("stop")) {
       stepper_stop();
    } else if(argument.equalsIgnoreCase("down")) {
       stepper_move_to(STEPPER_MAX_STEPS);
    } else if(argument.equalsIgnoreCase("up")) {
       stepper_move_to(0);
    } else { //percent 0-100
      long int newPosition = STEPPER_POSITION;
      int val = argument.toInt();
      if (val > 100) val = 100;
      if (val < 0) val = 0;
      val%10<5?val-=val%10:val=(val-val%10)+10; //округлить к 10
      
      if (PERCENT_REVERSE) {
         switch (val) {
            case 0: newPosition = STEPPER_MAX_STEPS; break;
            case 10: newPosition = stepper_calibration[9]; break;
            case 20: newPosition = stepper_calibration[8]; break;
            case 30: newPosition = stepper_calibration[7]; break;
            case 40: newPosition = stepper_calibration[6]; break;
            case 50: newPosition = stepper_calibration[5]; break;
            case 60: newPosition = stepper_calibration[4]; break;
            case 70: newPosition = stepper_calibration[3]; break;
            case 80: newPosition = stepper_calibration[2]; break;
            case 90: newPosition = stepper_calibration[1]; break;
            case 100: newPosition = 0; break;
         }
      } else {
         switch (val) {
            case 0: newPosition = 0; break;
            case 10: newPosition = stepper_calibration[1]; break;
            case 20: newPosition = stepper_calibration[2]; break;
            case 30: newPosition = stepper_calibration[3]; break;
            case 40: newPosition = stepper_calibration[4]; break;
            case 50: newPosition = stepper_calibration[5]; break;
            case 60: newPosition = stepper_calibration[6]; break;
            case 70: newPosition = stepper_calibration[7]; break;
            case 80: newPosition = stepper_calibration[8]; break;
            case 90: newPosition = stepper_calibration[9]; break;
            case 100: newPosition = STEPPER_MAX_STEPS; break;
         }
      }
      
      stepper_move_to(newPosition);      
    }

  } else if (control.equals("move")) {
      stepper_move(argument.toInt());
  } else if (control.equals("move_to")) {
      stepper_move_to(argument.toInt());    
  } else if (control.equals("config_set_top_position")) {
      step = STEPPER_POSITION = 0;
      stepper_stop();
  } else if (control.equals("config_set_bottom_position")) {
      step = STEPPER_POSITION = STEPPER_MAX_STEPS;
      stepper_stop();
  } else if (control.equals("config_set_stepper_max_steps")) {
      STEPPER_MAX_STEPS = argument.toInt();
      eeWriteInt(EEaddr__STEPPER_MAX_STEPS, STEPPER_MAX_STEPS);
  } else if(control.equals("config_set_stepper_reverse")) {
      STEPPER_REVERSE = argument.toInt();
      eeWriteInt(EEaddr__STEPPER_REVERSE, STEPPER_REVERSE);
  } else if(control.equals("config_set_percent_reverse")) {
      PERCENT_REVERSE = argument.toInt();
      eeWriteInt(EEaddr__PERCENT_REVERSE, PERCENT_REVERSE);      
  } else if (control.equals("config_set_stepper_max_steps")) {
      STEPPER_MAX_STEPS = argument.toInt();
      eeWriteInt(EEaddr__STEPPER_MAX_STEPS, STEPPER_MAX_STEPS);      
  } else if (control.equals("config_set_motor_accel")) {
      MOTOR_ACCEL = argument.toInt();
      stepper_stop();
      setup_stepper();
      eeWriteInt(EEaddr__MOTOR_ACCEL, MOTOR_ACCEL);          
  } else if (control.equals("config_set_motor_decel")) {
      MOTOR_DECEL = argument.toInt();
      stepper_stop();
      setup_stepper();
      eeWriteInt(EEaddr__MOTOR_DECEL, MOTOR_DECEL); 
  } else if (control.equals("config_set_motor_rpm")) {
      MOTOR_RPM = argument.toInt();
      stepper_stop();
      setup_stepper();
      eeWriteInt(EEaddr__MOTOR_RPM, MOTOR_RPM);      
  } else if (control.equals("config_set_motor_steps")) {
      MOTOR_STEPS = argument.toInt();
      eeWriteInt(EEaddr__MOTOR_STEPS, MOTOR_STEPS); 
  } else if (control.equals("config_set_motor_microsteps")) {
      MOTOR_MICROSTEPS = argument.toInt();
      setup_stepper();
      eeWriteInt(EEaddr__MOTOR_MICROSTEPS, MOTOR_MICROSTEPS); 
  } else if (control.equals("config_set_send_while_working")) {
      SEND_WHILE_WORKING = argument.toInt()?1:0;
      eeWriteInt(EEaddr__SEND_WHILE_WORKING, SEND_WHILE_WORKING); 
  } else if (control.equals("config_set_send_status_interval")) {
      SEND_STATUS_INTERVAL = argument.toInt();
      eeWriteInt(EEaddr__SEND_STATUS_INTERVAL, SEND_STATUS_INTERVAL);       
  } else if(control.equals("meta")) {
      mqttPublish("meta/version", String(VERSION));
      mqttPublish("meta/wifi_ip", WiFi.localIP().toString());
      mqttPublish("meta/stepper/reverse", STEPPER_REVERSE?"1":"0");
      mqttPublish("meta/stepper/position_steps", String(STEPPER_POSITION));
      mqttPublish("meta/stepper/position_percent", String(positionToPercentStrict(STEPPER_POSITION)));
      mqttPublish("meta/stepper/max_steps", String(STEPPER_MAX_STEPS));
//    mqttPublish("meta/stepper/state", String(stepper.getCurrentState()));
//    mqttPublish("meta/stepper/current_rpm", String(stepper.getCurrentRPM()));
      mqttPublish("meta/status_interval", String(SEND_STATUS_INTERVAL));
      mqttPublish("meta/send_while_working", String(SEND_WHILE_WORKING));
      mqttPublish("meta/motor/accel", String(MOTOR_ACCEL));
      mqttPublish("meta/motor/decel", String(MOTOR_DECEL));
      mqttPublish("meta/motor/rpm", String(MOTOR_RPM));
      mqttPublish("meta/motor/steps", String(MOTOR_STEPS));
      mqttPublish("meta/motor/microsteps", String(MOTOR_MICROSTEPS));
  }
}



