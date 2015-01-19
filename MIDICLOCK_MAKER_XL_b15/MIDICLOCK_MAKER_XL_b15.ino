

//--------------------------------------------------MIDICLOCK MAKER & MIDICLOCK2BPM-------------------------------------------

#include <TimerOne.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 99
Adafruit_SSD1306 display(OLED_RESET);


const unsigned long BPM_RATE = 80;
unsigned long bpm_update;
byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xb0;
byte cc = 0xb0; //Continuous controller

int play_flag = 1;
byte data;
byte counter;            
//                                              Arduino Nano
#define LED 12            //  Led 1             D12
#define LEDIR 8           //  IRled syncout     D8
#define SYNC_INT_PIN 7    //  bpm in/ext        D7
#define SYNC_PLUS 6       //  sync+ button      D6
#define SYNC_MINUS 5      //  sync- button      D5
#define TAP 4             //  tap button        D4
#define START_STOP 3      //  start/stop button D3
#define SHIFT_BUTTON 2    //  shift button      D2     - noch nicht vorhanden
#define BUTTON 13         //  button            D13    - noch nicht vorhanden
int BPM_POT = A0;         //  BPM POT           A0

byte prevmode,prevtap;
byte mode_latch;
byte prevstart, start_latch;
byte pre_start_latch;
byte sync_int;
int bpm_int = 120;
int bpm_tap,bpm_pitch,bpm_pitch_pre,bpm_int_pitch;
int pot;
unsigned long bpm_time;
unsigned long previousMillis;
unsigned long previous2Millis;
unsigned long currentMillis;
int bpm_gemessen;
int bpm_zaehler;
byte counter_viertel;
bool update = false;



void setup() {
    display.begin(SSD1306_SWITCHCAPVCC);

    // Display the splash screen (we're legally required to do so)
    display.display();
    unsigned long start = millis();

    display.clearDisplay();

    while(millis() - start < 2000);
    display.drawRect(0, 16, 128, 48, WHITE);
    display.setCursor(100,47);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("bpm");
    display.display();

    bpm_update =  millis();
    oled_stop();

  Serial.begin(31250);
  pinMode(LED, OUTPUT);
  pinMode(SYNC_PLUS, INPUT_PULLUP);
  pinMode(SYNC_MINUS, INPUT_PULLUP);
  pinMode(TAP, INPUT_PULLUP);
  pinMode(START_STOP, INPUT_PULLUP);
  pinMode(SYNC_INT_PIN, INPUT_PULLUP);
  pinMode(LEDIR, OUTPUT);
  
  digitalWrite(LED, HIGH);
  delay(50);
  Timer1.initialize(100);
  Timer1.attachInterrupt(midiclock);

}





void loop() {
    //bool update = false;
    unsigned long time = millis();
    bpm();
    //static bool up_state = false;
    //static bool down_state = false;
    
    //up_state |= (digitalRead(UP_BUTTON) == LOW);
    //down_state |= (digitalRead(DOWN_BUTTON) == LOW);
    
      if (time > bpm_update){
        button();
        oled_bpm();
        //  Serial.println(data);
        bpm_update += BPM_RATE;
        update = true;
        if(update)
          display.display();         
      }
}




