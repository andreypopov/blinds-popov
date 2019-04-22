void setup_webserver() {
  if (WEB_SERVER_ON) {
    server.on("/set_top", [](){
      debug("webserver: /set_top");
      exec_command("config_set_top_position", "1");
      handleRedirect(String("/"));
    });
    server.on("/set_bottom", [](){
      debug("webserver: /set_bottom");
      exec_command("config_set_bottom_position", "1");
      handleRedirect(String("/"));
    });
    server.on("/up", [](){
      debug("webserver: /up");
      exec_command("stepper", "up");
      handleRedirect(String("/"));
    });
    server.on("/down", [](){
      debug("webserver: /down");
      exec_command("stepper", "down");
      handleRedirect(String("/"));
    });
    server.on("/set", [](){
      debug("webserver: /set?pos="+server.arg("pos"));
      exec_command("stepper", String(server.arg("pos")));
      handleRedirect(String("/"));
    });
    server.on("/move", [](){
      debug("webserver: /move?steps="+server.arg("steps"));
      exec_command("move", String(server.arg("steps")));
      handleRedirect(String("/"));
    });
    server.on("/stop", [](){
      debug("webserver: /stop");
      exec_command("stepper", "stop");
      handleRedirect(String("/"));
    });
    server.on("/", handleRoot);
    server.begin();
    debug("WebServer is ready");
  }
}

void loop_webserver() {
  if(WEB_SERVER_ON && client.connected()) {
    server.handleClient();
  }
}

void handleRedirect(String url) { 
  server.sendHeader("Location", url, true);
  server.send(302, "text/plain", "");
}

void handleRoot() { 
  if (!server.authenticate(WEB_SERVER_username, WEB_SERVER_password)) {
    return server.requestAuthentication(DIGEST_AUTH, "Custom Auth Realm", "Authentication Failed");
  }
  
  if ( server.hasArg("MOTOR_MICROSTEPS") ) {
    exec_command("config_set_motor_microsteps", server.arg("MOTOR_MICROSTEPS"));
  }
  if ( server.hasArg("MOTOR_STEPS") ) {
    exec_command("config_set_motor_steps", server.arg("MOTOR_STEPS"));
  }
  if ( server.hasArg("MOTOR_ACCEL") ) {
    exec_command("config_set_motor_accel", server.arg("MOTOR_ACCEL"));
  }
  if ( server.hasArg("MOTOR_DECEL") ) {
    exec_command("config_set_motor_decel", server.arg("MOTOR_DECEL"));
  }
  if ( server.hasArg("MOTOR_RPM") ) {
    exec_command("config_set_motor_rpm", server.arg("MOTOR_RPM"));
  }
  if ( server.hasArg("STEPPER_MAX_STEPS") ) {
    exec_command("config_set_stepper_max_steps", server.arg("STEPPER_MAX_STEPS"));
  }
  if ( server.hasArg("STEPPER_REVERSE") ) {
    exec_command("config_set_stepper_reverse", server.arg("STEPPER_REVERSE"));
  }
  if ( server.hasArg("SEND_WHILE_WORKING") ) {
    exec_command("config_set_send_while_working", server.arg("SEND_WHILE_WORKING"));
  }
  if ( server.hasArg("SEND_STATUS_INTERVAL") ) {
    exec_command("config_set_send_status_interval", server.arg("SEND_STATUS_INTERVAL"));
  }
  server.send ( 200, "text/html", getPage() );
}

String getPage() {
  String webpage = "<html charset=UTF-8>";
  webpage += "<meta charset='UTF-8'>";
  webpage += "<meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  webpage += "<head>";
  webpage += "<script src='https://code.jquery.com/jquery-3.2.1.slim.min.js' integrity='sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN' crossorigin='anonymous'></script>";
  webpage += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script>";  
  webpage += "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js'></script>";
  webpage += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css'>";
  webpage += "<title>Blinds: "+String(MQTT_device)+"</title>";
  webpage += "</head>";
  webpage += "<body>";
  
  webpage += "<div class='container-fluid'>";
  webpage += "<h1>Штора: /"+String(MQTT_namespace)+"/"+String(MQTT_device)+"</h1>";

           
  webpage += "<form action='/' method='POST'>";
  webpage += "<div class='row'>";


  webpage += "<div class='col-lg-7 col-md-6 col-sm-12 pb-6'>";
   
    webpage += "<div class='btn-group pb-6' role='group'>"; 
        
        webpage += "<div class='btn-group' role='group'>";
          webpage += "<button class='btn btn-success dropdown-toggle' type='button' id='menuSet' data-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>";
          webpage += String(positionToPercentStrict(STEPPER_TARGET))+"%";
          webpage += "</button>";
          webpage += "<div class='dropdown-menu' aria-labelledby='menuSet'>";
            webpage += "<a class='dropdown-item' href='/set?pos=100'>100%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=90'>90%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=80'>80%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=70'>70%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=60'>60%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=50'>50%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=40'>40%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=30'>30%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=20'>20%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=10'>10%</a>";
            webpage += "<a class='dropdown-item' href='/set?pos=0'>0%</a>";
          webpage += "</div>";
        webpage += "</div>";
    
      webpage += formControlButton("Вверх", "/up", "success");
      webpage += formControlButton("Стоп", "/stop", "success");
      webpage += formControlButton("Вниз", "/down", "success");
    webpage += "</div><br>"; 



 

  
    webpage += "STEPPER_POSITION: "+String(positionToPercentStrict(STEPPER_POSITION))+"% ("+String(STEPPER_POSITION)+")<br>";
    webpage += "STEPPER_TARGET: "+String(positionToPercentStrict(STEPPER_TARGET))+"% ("+String(STEPPER_TARGET)+")<br>";
    webpage += "STEPPER_STATE: "+String(STEPPER_STATE)+"<br>";
    webpage += "STEPPER_TIMER: "+String(STEPPER_TIMER_LAST)+"ms<br>";

    webpage += "<div class='btn-group pb-6' role='group'>";
          webpage += formControlButton("SET TOP", "/set_top", "info");
            webpage += "<div class='btn-group' role='group'>";
              webpage += "<button class='btn btn-info dropdown-toggle' type='button' id='menuMove' data-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>";
              webpage += "MOVE";
              webpage += "</button>";
              webpage += "<div class='dropdown-menu' aria-labelledby='menuMove'>";
                webpage += "<h6 class='dropdown-header'>Вверх</h6>";
                webpage += "<a class='dropdown-item' href='/move?steps=-10000'>-10000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-5000'>-5000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-3000'>-3000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-2000'>-2000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-1000'>-1000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-500'>-500</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-300'>-300</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-200'>-200</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=-100'>-100</a>";
                webpage += "<h6 class='dropdown-header'>Вниз</h6>";
                webpage += "<a class='dropdown-item' href='/move?steps=100'>100</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=200'>200</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=300'>300</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=500'>500</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=1000'>1000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=2000'>2000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=3000'>3000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=5000'>5000</a>";
                webpage += "<a class='dropdown-item' href='/move?steps=10000'>10000</a>";
              webpage += "</div>";
            webpage += "</div>";
          webpage += formControlButton("SET BOTTOM", "/set_bottom", "info"); 
      webpage += "</div>";
    webpage += "</div>";
 
  
  webpage += "<div class='col-lg-5 col-md-6 col-sm-12 pb-4'>";
  
  webpage += "<table class='table'>";
  webpage += "<tr><th width='350px'>Параметр</th><th width='150px'>Значение</th></tr>";
 // webpage += "<tr><td>MOTOR_STEPS<div class='form-text text-muted small'>2048 для шаговика 28byj-48</div></td><td>"+formControlInput("MOTOR_STEPS", String(MOTOR_STEPS))+"</td></tr>";
 // int choiceMicroSteps[] = {1,2};
 // webpage += "<tr><td>MOTOR_MICROSTEPS<div class='form-text text-muted small'>Режим микрошага: 1 - full, 2 - half (в два раза дольше)</div></td><td>"+formControlSelect("MOTOR_MICROSTEPS", String(MOTOR_MICROSTEPS), choiceMicroSteps, 2)+"</td></tr>";

  webpage += "<tr><td>STEPPER_MAX_STEPS<div class='form-text text-muted small'>Подбирается индивидуально. Количество шагов, нужное, чтобы размотать штору.</div></td><td>"+formControlInput("STEPPER_MAX_STEPS", String(STEPPER_MAX_STEPS))+"</td></tr>";
  
  int choiceMotorRpm[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
  webpage += "<tr><td>MOTOR_RPM<div class='form-text text-muted small'>Количество оборотов в минуту. 28byj-48 поддерживает не больше 12-15rpm, однако, если поставить больше, это отразится на скорости из-за кривой разгона.</div></td><td>"+formControlSelect("MOTOR_RPM", String(MOTOR_RPM), choiceMotorRpm, 30)+"</td></tr>";
  
  webpage += "<tr><td>MOTOR_ACCEL<div class='form-text text-muted small'>Плавное увеличение скорости при старте. Чем плавнее разгоняется, тем выше крутящий момент.</div></td><td>"+formControlInput("MOTOR_ACCEL", String(MOTOR_ACCEL))+"</td></tr>";
  webpage += "<tr><td>MOTOR_DECEL<div class='form-text text-muted small'>Как быстро мотор остановится.</div></td><td>"+formControlInput("MOTOR_DECEL", String(MOTOR_DECEL))+"</td></tr>";

  int choiceReverse[] = {0,1};
  webpage += "<tr><td>STEPPER_REVERSE<div class='form-text text-muted small'>Направление вращения мотора. 0 - мотор установлен справа, 1 - слева</div></td><td>"+formControlSelect("STEPPER_REVERSE", String(STEPPER_REVERSE), choiceReverse, 2)+"</td></tr>";
  webpage += "<tr><td>PERCENT_REVERSE<div class='form-text text-muted small'>1: 100% открыто, 0: 100% закрыто</div></td><td>"+formControlSelect("PERCENT_REVERSE", String(PERCENT_REVERSE), choiceReverse, 2)+"</td></tr>";
  
  int choiceSend[] = {0,1};
  webpage += "<tr><td>SEND_WHILE_WORKING<div class='form-text text-muted small'>Посылать данные во время работы мотора, могут быть микропрерывания.</div></td><td>"+formControlSelect("SEND_WHILE_WORKING", String(SEND_WHILE_WORKING), choiceSend, 2)+"</td></tr>";

  int choiceInterval[] = {10,30,60};
  webpage += "<tr><td>SEND_STATUS_INTERVAL<div class='form-text text-muted small'>Время в секундах, как часто отправлять данные</div></td><td>"+formControlSelect("SEND_STATUS_INTERVAL", String(SEND_STATUS_INTERVAL), choiceInterval, 3)+"</td></tr>";

  webpage += "</table>";
  webpage += "";
  webpage += "<div class='form-group'><button type='submit' class='btn btn-success'>Save</button></div>";
  webpage += "</div>"; //1st col

  
  webpage += "</div>"; //class="row"
  webpage += "</form>";
  webpage += "</div>"; //class="container"
  webpage += "</body>";
  webpage += "</html>";

  return webpage;
}

String formControlButton(String name, String url, String theme) {
    String html = "<a class='btn btn-"+theme+"' href='"+url+"'>"+name+"</a>";
    return html;
}

String formControlInput(String name, String val) {
    String html = "<input class='form-control' name='"+name+"' value='"+val+"'>";
    return html;
}

String formControlSelect(String name, String val, int* options, int len) {
    String html = "<select class='form-control' name='"+name+"'>";

    for (int i = 0; i < len; i++) {
      if (val.equalsIgnoreCase(String(options[i]))) {
        html += "<option value='"+String(options[i])+"' selected='selected'>"+String(options[i])+"</option>";
      } else {
        html += "<option value='"+String(options[i])+"'>"+String(options[i])+"</option>";
      }
    }
    html += "</select>";
    return html;
}

