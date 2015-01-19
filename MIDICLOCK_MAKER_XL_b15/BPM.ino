void midiclock(){
 

  
// -------------------------TIME->BPM->MIDICLOCK INTERN------------------- 
 if(mode_latch==0){
    unsigned long current2Millis = micros();
  if(current2Millis - previous2Millis > bpm_time) {        
      
      if(start_latch==0) Serial.write(midi_clock);
      previous2Millis = current2Millis;
      counter++;
      counter%=24;
      counter_viertel=counter/6;
  }
 /* if (digitalRead(TAP)==0 && prevtap==1){
    tapTempo();
    digitalWrite(LED, HIGH);
    }
 prevtap=digitalRead(TAP); */
 }
 
// -------------------------MIDICLOCK EXTERN->TIME->BPM -----------------  
 if(mode_latch==1){ 
  unsigned long current2Millis = micros();
  if(current2Millis - previous2Millis > bpm_time) {       
      if(start_latch==0) Serial.write(midi_clock);
      previous2Millis = current2Millis;
  }
 } 
}


void bpm(){
// -------------------------------------------------------------MIDICLOCK INTERN-------------------------------------------   
pot=analogRead(BPM_POT);
if(mode_latch==0){                                      
  oled_bpm_int();
  
  if(digitalRead(TAP)==0){
  //pot=analogRead(BPM_POT);
  bpm_int=(map(pot ,0,1024,40,201));  //http://hofa-college.de/online_campus/pages/tipps-amp-tricks/hofa-tools/bpm-in-msec.php
  bpm_pitch=0;
  }
  
  if(digitalRead(TAP)==1 && pot != bpm_pitch_pre){
    bpm_pitch_pre=pot;
    bpm_pitch=(map(pot ,0,1024,-16,17));     
  }
 
  bpm_int_pitch=bpm_int+bpm_pitch;
  bpm_time=2500000/bpm_int_pitch;    //bpm_time=(60*1000*1000/24)/bpm_int_pitch;  
  Serial.write(cc);
  Serial.write(1);
  Serial.write(bpm_int_pitch/2);
//--------------------------MIDI START TASTER--------------
 if(start_latch==0){                         
      if(pre_start_latch==0){
        Serial.write(midi_start);
        oled_start();
        pre_start_latch=1;
      }
      Sync();  
      //IRblink();  
 }
 //---------------------------MIDI STOP TASTER---------------
 if(start_latch==1){                                 
   start_latch=3;
   oled_stop();
   Serial.write(midi_stop);
   digitalWrite(LED, LOW);
   display.fillRect(25, 0, 67, 16, BLACK);
   update = true;
   
   pre_start_latch=0;
 }
}
// ----------------------------------------------------------------MIDICLOCK EXTERN-------------------------------------------  
if(mode_latch==1){                                
 oled_bpm_ext();
//---------------------------MIDI START EXTERN-------------
 if(Serial.available() > 0) {
   data = Serial.read();
    if(data == midi_start) {              
      Serial.write(midi_start);
      play_flag = 1;
      oled_start();
      start_latch=0;
      pre_start_latch=1;
  }
//---------------------------MIDI CONTINUE EXTERN----------
  else if(data == midi_continue) {
   play_flag = 1;
  }
//---------------------------MIDI STOP EXTERN--------------
  else if     (data == midi_stop){
   Serial.write(midi_stop);
   play_flag = 0;
   oled_stop();
   start_latch=3;
   digitalWrite(LED, LOW);
   counter=0;
  }
//---------------------------MIDICLOCK EXTERN------------
  else if((data == midi_clock) && (play_flag == 1)) {      
    counter++;
    counter%=24;
    counter_viertel=counter/6;
    Sync();
    midiclock2bpm();
    //Serial.write(data);
  }   
 }
}
 //--------------------------MIDI START TASTER--------------
  if(start_latch==0){                                
      if(pre_start_latch==0){
        Serial.write(midi_start);
        oled_start();
        pre_start_latch=1;
        play_flag = 1;
      }
  }
//---------------------------MIDI STOP TASTER--------------- 
  if(start_latch==1){                                 
   start_latch=3;
   oled_stop();
   Serial.write(midi_stop);
   pre_start_latch=0;
   play_flag = 0;
  }
} 

//----------------------------- do something for every 6 MIDI Clock pulse when the sequencer is running-----------------------------
void Sync() {   
  if(counter_viertel==0){
   digitalWrite(LED, HIGH);
   display.fillRect(76, 0, 16, 16, BLACK);
   display.fillRect(25, 0, 16, 16, WHITE);
   //IRblink();
  }
  
  if(counter_viertel==1){
  digitalWrite(LED, LOW);
  display.fillRect(25, 0, 16, 16, BLACK);
  display.fillRect(42, 0, 16, 16, WHITE);
  //IRblink();
  }
  
  if(counter_viertel==2){
   display.fillRect(42, 0, 16, 16, BLACK);
   display.fillRect(59, 0, 16, 16, WHITE);
   //IRblink();
  }
  
  if(counter_viertel==3){
   display.fillRect(59, 0, 16, 16, BLACK);
   display.fillRect(76, 0, 16, 16, WHITE);
   //IRblink();
  }
} 

void IRblink(){
  digitalWrite(LEDIR, HIGH);
  //delay(1);
  digitalWrite(LEDIR, LOW);
}
//--------------------------------------------------TEXTE OLED------------------------------------------------------
void oled_bpm_int(){
  display.setCursor(0,8);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.println("ext.");
  
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("int."); 
}
void oled_bpm_ext(){
  display.setCursor(0,8);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("ext.");
  
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.println("int."); 
}
void oled_start(){
  display.setCursor(104,8);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.println("stop");
  
  display.setCursor(98,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("start"); 
}
void oled_stop(){
  display.setCursor(104,8);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("stop");
  
  display.setCursor(98,0);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.println("start"); 
}
void oled_bpm(){
  display.fillRect(26, 26, 72, 28, BLACK);
  display.setCursor(26,26);
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.println(bpm_int_pitch); 

}

//-----------------------------------------MIDICLOCK2BPM-------------------------------------------------
void midiclock2bpm(){
 
    if (bpm_zaehler==0){
    currentMillis = micros();
    } 
    if (bpm_zaehler==191){
      previousMillis=(micros()-currentMillis)/192;
      if (previousMillis >=1 <=300){
        bpm_int_pitch=2500000/previousMillis;
        bpm_time=previousMillis;
      }
    }
    bpm_zaehler++;
    bpm_zaehler%=192;           // 32 takte
}

//---------------------------------------------BUTTON ABFRAGE-------------------------------------------
void button(){
  
  if (digitalRead(SYNC_INT_PIN)==0 && prevmode==1){
    mode_latch++;
    mode_latch%=2;
  }
  
  prevmode=digitalRead(SYNC_INT_PIN);
  
  if (digitalRead(START_STOP)==0 && prevstart==1){
    start_latch++;
    start_latch%=2;
  }
  
  prevstart=digitalRead(START_STOP);
  
  if (digitalRead(SYNC_PLUS)==0){
    counter++;
  }
  if (digitalRead(SYNC_MINUS)==0){
    counter--;
  }
  
  
}
