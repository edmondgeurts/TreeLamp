//TreeLamp sketch 7-12-2015

#include <SPI.h>

int latchpin = 8;
int clockpin = 13;
int dataPin = 11;
int switch_a = 2;


volatile int state = 0;
//debounce button related

volatile  unsigned long last_interrupt_time = 0;


#define HC595_COUNT 2 //IC count
#define MAX_PORTS  (HC595_COUNT * 8) //Note not all the ports are used (6 ports per IC)
#define MAX_BRIGHTNESS 4 //4-bit brightness mask
#define DEBOUNCING_TIME 1200 //debouncing time in milliseconds

//Row pins
#define  ROW_A  5
#define  ROW_B  3
#define  ROW_C  4

char current_row = 0;
//represents a single port. RGB leds use 3, one port for eatch color
typedef struct Led_type {
  boolean led[MAX_BRIGHTNESS];
  unsigned char brightness;
  
}Led;

char hc595[HC595_COUNT];//light up individual leds per IC
Led leds[MAX_PORTS];

char lookup[] = {1,2,4,8,16,32,64,128};


//set all leds to 0
void clear_leds(struct Led_type *l) {
  for (int i=0; i <= MAX_PORTS; i++) {
    for (int j=0; j <= MAX_BRIGHTNESS; j++) {
      l[i].led[j] = 0;
    }
   l[i].brightness = 0;
   
  } 
}

void clear_led(struct Led_type *l,char led) {
  switch(led) {
    case 0:
          for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[0].led[j] = 0;
            l[1].led[j] = 0;
            l[2].led[j] = 0;
            
          }
          l[0].brightness = 0;
            l[1].brightness = 0;
            l[2].brightness = 0;
          break;
    case 1:
          for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[0].led[j] = 0;
            l[1].led[j] = 0;
            l[2].led[j] = 0;
            
          }
          l[0].brightness = 0;
            l[1].brightness = 0;
            l[2].brightness = 0;
          break;
    case 2:
          for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[0].led[j] = 0;
            l[1].led[j] = 0;
            l[2].led[j] = 0;
            
          }
          l[0].brightness = 0;
            l[1].brightness = 0;
            l[2].brightness = 0;
          break;
    case 3:
          for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[8].led[j] = 0;
            l[9].led[j] = 0;
            l[10].led[j] = 0;
            
          }
          l[8].brightness = 0;
            l[9].brightness = 0;
            l[10].brightness = 0;
          break;
    case 4:
          for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[8].led[j] = 0;
            l[9].led[j] = 0;
            l[10].led[j] = 0;
            
          }
           l[8].brightness = 0;
           l[9].brightness = 0;
           l[10].brightness = 0;
          break;
    case 5:
          for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[8].led[j] = 0;
            l[9].led[j] = 0;
            l[10].led[j] = 0;
          }
           l[8].brightness = 0;
           l[9].brightness = 0;
           l[10].brightness = 0;
          break;
    case 6:
        for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[11].led[j] = 0;
            l[12].led[j] = 0;
            l[13].led[j] = 0;
          }
           l[11].brightness = 0;
           l[12].brightness = 0;
           l[13].brightness = 0;
          break;
    case 7:
        for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[11].led[j] = 0;
            l[12].led[j] = 0;
            l[13].led[j] = 0;
          }
           l[11].brightness = 0;
           l[12].brightness = 0;
           l[13].brightness = 0;
          break;
    case 8:
        for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[11].led[j] = 0;
            l[12].led[j] = 0;
            l[13].led[j] = 0;
          }
           l[11].brightness = 0;
           l[12].brightness = 0;
           l[13].brightness = 0;
          break;
    case 9:
        for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[3].led[j] = 0;
            l[4].led[j] = 0;
            l[5].led[j] = 0;
          }
           l[3].brightness = 0;
           l[4].brightness = 0;
           l[5].brightness = 0;
          break;
    case 10:
        for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[3].led[j] = 0;
            l[4].led[j] = 0;
            l[5].led[j] = 0;
          }
           l[3].brightness = 0;
           l[4].brightness = 0;
           l[5].brightness = 0;
          break;

    case 11:
        for (int j=0; j <= MAX_BRIGHTNESS; j++) {
            l[3].led[j] = 0;
            l[4].led[j] = 0;
            l[5].led[j] = 0;
          }
           l[3].brightness = 0;
           l[4].brightness = 0;
           l[5].brightness = 0;
          break;
    
        
          
  }
}


//set the color of a single leave
void set_leave_color(struct Led_type *l, char leave, char r, char g, char b) {
  switch (leave) {
    case 0 : 
            set_led(l, 0, r);
            set_led(l, 1, g);
            set_led(l, 2, b);
            current_row = ROW_A;
            break;
   case 1 : 
           set_led(l, 0, r);
           set_led(l, 1, g);
           set_led(l, 2, b);
           current_row = ROW_C;
           break;
   case 2 : 
           set_led(l, 0, r);
           set_led(l, 1, g);
           set_led(l, 2, b);
           current_row = ROW_B;
           break;

   case 3 : 
            set_led(l, 8, r);
            set_led(l, 9, g);
            set_led(l, 10, b);
            current_row = ROW_B;
            break;
   case 4 : 
            set_led(l, 8, r);
            set_led(l, 9, g);
            set_led(l, 10, b);
           current_row = ROW_C;
           break;
   case 5 : 
           set_led(l, 8, r);
           set_led(l, 9, g);
           set_led(l, 10, b);
           current_row = ROW_A;
           break;
   case 6 : 
           set_led(l, 11, r);
           set_led(l, 12, g);
           set_led(l, 13, b);
           current_row = ROW_B;
           break;
   case 7 : 
           set_led(l, 11, r);
           set_led(l, 12, g);
           set_led(l, 13, b);
           current_row = ROW_C;
           break;
   case 8 : 
           set_led(l, 11, r);
           set_led(l, 12, g);
           set_led(l, 13, b);
           current_row = ROW_A;
           break;
   case 9 : 
           set_led(l, 3, r);
           set_led(l, 4, g);
           set_led(l, 5, b);
           current_row = ROW_A;
           break;
   case 10 : 
           set_led(l, 3, r);
           set_led(l, 4, g);
           set_led(l, 5, b);
           current_row = ROW_C;
           break;
   case 11 : 
           set_led(l, 3, r);
           set_led(l, 4, g);
           set_led(l, 5, b);
           current_row = ROW_B;
           break;
   
  }
}

//set the brightness of a led
void set_led(struct Led_type *l, int led_number, char brightness) {
  brightness = constrain(brightness, 0, 15);
  led_number = constrain(led_number, 0, MAX_PORTS);
  //convert decimal number into binary
  for (char i = MAX_BRIGHTNESS; i >= 0; i--) {
    if (brightness - (1 << i) >= 0) {
      brightness -= (1 << i);
      l[led_number].led[i] = 1;
    }
    else {
     l[led_number].led[i] = 0; 
    }//end if
  }//end for
}

//use 4-bit BAM

void shiftreg(struct Led_type *l) {
  unsigned char tmp;
  unsigned char maskindex;
   
  for (unsigned char bam = 0; bam < 16; bam++) {
    //digitalWrite(latchpin, LOW);
    //set the latch pin LOW
    PORTB &= B11111110; //port 8 LOW
    //tmp = 0;
    //reset the switch position
    for (char i=0; i < HC595_COUNT; i++) {
      hc595[i] = 0;
    }
    
    for (char ledno = MAX_PORTS - 1; ledno >= 0; ledno--) {
      tmp = 0;
      maskindex = l[ledno].brightness;
      
      if (bam == 1 && l[ledno].led[maskindex]) {
        if (ledno >= 0 && ledno <= 7) { tmp |= lookup[ledno]; hc595[0] |= tmp; }
          else if (ledno >= 8 && ledno <= 15) {tmp |= lookup[ledno-8]; hc595[1] |= tmp;  }
            else if (ledno >= 16 && ledno <= 23) {tmp |= lookup[ledno-16]; hc595[2] |= tmp;  }
       
      }
        else if ((bam == 2 || bam == 3) && l[ledno].led[maskindex]) {
          if (ledno >= 0 && ledno <= 7) { tmp |= lookup[ledno]; hc595[0] |= tmp; }
            else if (ledno >= 8 && ledno <= 15) {tmp |= lookup[ledno-8]; hc595[1] |= tmp; }
              else if (ledno >= 16 && ledno <= 23) {tmp |= lookup[ledno-16]; hc595[2] |= tmp;  }
          
        }
          else if (bam >= 4 && bam <= 7 && l[ledno].led[maskindex]) {
            if (ledno >= 0 && ledno <= 7) { tmp |= lookup[ledno]; hc595[0] |= tmp; }
              else if (ledno >= 8 && ledno <= 15) {tmp |= lookup[ledno-8]; hc595[1] |= tmp; }
                else if (ledno >= 16 && ledno <= 23) {tmp |= lookup[ledno-16]; hc595[2] |= tmp;  }
            
          }
            else if (bam >= 8 && bam <= 15 && l[ledno].led[maskindex]) {
              if (ledno >= 0 && ledno <= 7) { tmp |= lookup[ledno]; hc595[0] |= tmp; }
                else if (ledno >= 8 && ledno <= 15) {tmp |= lookup[ledno-8]; hc595[1] |= tmp; }
                  else if (ledno >= 16 && ledno <= 23) {tmp |= lookup[ledno-16]; hc595[2] |= tmp;  }
      }
      l[ledno].brightness++;
      if (l[ledno].brightness > 3) {
        l[ledno].brightness = 0;
      }//end if max brichtness
    }//end FOR LED
    
    //output the result. Turn the leds on or off
    for (char i=HC595_COUNT - 1; i >= 0; i--) {
      SPI.transfer(hc595[i]);
    }
    //set the latch pin HIGH
    PORTB |= B00000001; //port 8 HIGH
    //clear rows
    PORTD &= B11000111; //port 3, 4 & 5 LOW
  
    //set the row
    switch (current_row) {
      case 5: PORTD |= B00100000;
          break;
      case 3: PORTD |= B00001000;
          break;
      case 4: PORTD |= B00010000;
          break;
         
    }//end switch
  }//end FOR BAM
}//end fuction shiftreg



void reset_bus() {
 for (int i=HC595_COUNT; i >= 0; i--) {
      SPI.transfer(0);
    }
    digitalWrite(latchpin, HIGH);
}//end reset bus

void setup() {
    clear_leds(leds);
  
  //set_led(leds,0,15);
  //set_led(leds,1,15);
  //set_led(leds,5,15);
  /*
  for (int i=0; i < 14; i++) {
    set_led(leds,i,15);
  }
 */
  
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  //SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE2));
  //reset the data bus
  reset_bus();
  
  //set the interrupt based switch pin
  pinMode(switch_a, OUTPUT);
  attachInterrupt(0, anim_cycle, CHANGE);
  
  pinMode(latchpin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(ROW_A, OUTPUT);
  pinMode(ROW_B, OUTPUT);
  pinMode(ROW_C, OUTPUT);
  Serial.begin(9600);
}

void anim_cycle() {
 // interrupt_time = micros();
  
  if (long(micros() - last_interrupt_time) >= DEBOUNCING_TIME ) {
    set_led(leds,state,15);
    state++;
  }
 // Serial.println(micros() - last_interrupt_time );
  last_interrupt_time = micros();
  
  
}//end anim cycle

//animations
void anim1a() {
  char max_leave = 12;
  unsigned long play_time = 26000;
  unsigned long anim_transition = 800;
  unsigned long time_offset = millis();
  unsigned long anim_transition_offset = 0;
  int leave_counter = 0;

  clear_leds(leds);
  shiftreg(leds);
   
  while ( leave_counter <= max_leave ) {
    //clear_leds(leds);
    anim_transition_offset = millis();
    while ((long) (millis() - anim_transition_offset) <= anim_transition) {
      for (int i=0; i < leave_counter; i++) {
        //clear_leds(leds);
        
        
        set_leave_color(leds,i,15,15,15);
        shiftreg(leds);
        clear_led(leds,i);
        //delay(1500);
      }
      
    }//end while anim transition offset
    leave_counter++;
  }//end while
  
}//end anim1a





void loop() {
  anim1a();  
 
  
}
