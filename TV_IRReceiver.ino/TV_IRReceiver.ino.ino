/*
 * 20DF9E61 = Ratio
 * 20DF8976 = AD
 */

#include <IRremote.h>
#define IR_RATIO	0x20DF9E61
#define IR_AD		 0x20DF8976
#define IR_TEXT	 0x20DF04FB
#define IR_TOPT	 0x20DF847B
#define IR_SUBTITLE 0x20DF9C63
#define IR_STOP	 0x20DF8D72
#define IR_PLAY	 0x20DF0DF2
#define IR_PAUSE	0x20DF5DA2
#define IR_PREV	 0x20DFF10E
#define IR_NEXT	 0x20DF718E
#define IR_SIMPLX 0x20DF7E81
#define IR_RED		0x20DF4EB1
#define IR_GREEN	0x20DF8E71
#define IR_BLUE	 0x20DF8679
#define IR_YELL	 0x20DFC639
#define RECV_PIN	12
#define LED			 5
#define LED_BOARD 13
#define PIN_IR_VCC 10
#define PIN_IR_GND 9

//#define DBG 1

//int pwm_step[] = {0,5,10,15,20,25,35,45,55,66,80,100,130,190,255};
int pwm_step[] = {0,1,2,5,8,11,15,20,26,33,41,51,64,80,100,125,160,200,255};
int pwm_index;

int woot=0;	//Counter for woot mode

IRrecv irrecv(RECV_PIN);
decode_results results;
void setup()
{
	pinMode(LED, OUTPUT);
	pinMode(LED_BOARD, OUTPUT);
	digitalWrite(LED_BOARD, HIGH);
	pinMode(PIN_IR_VCC, OUTPUT);
	digitalWrite(PIN_IR_VCC, HIGH);
	pinMode(PIN_IR_GND, OUTPUT);
	digitalWrite(PIN_IR_GND, LOW);
  #ifdef DBG
	  Serial.begin(9600);
    Serial.print("Enabled IRin.! pwm_step=");
    Serial.println(sizeof(pwm_step)/sizeof(int));
  #endif
	irrecv.enableIRIn(); // Start the receiver
	delay(100);
	digitalWrite(LED_BOARD, LOW);
}

void loop() {
	if (irrecv.decode(&results)) {
		#ifdef DBG
      Serial.print(pwm_index);
      Serial.print(". VAL=");
      Serial.print(results.value, HEX);
      Serial.print(". woot=");
      Serial.println(woot);
    #endif
		if(results.value==IR_BLUE){
      #ifdef DBG
        Serial.println("BLUE");
      #endif
			digitalWrite(LED_BOARD, HIGH);
			if (pwm_index<((sizeof(pwm_step)/sizeof(int))-1)/2) {
				pwm_index=((sizeof(pwm_step)/sizeof(int))-1)/2;
			} else {
				pwm_index=(sizeof(pwm_step)/sizeof(int))-1;
			}
		} else if(results.value==IR_RED){
      #ifdef DBG
        Serial.println("RED");
      #endif
			digitalWrite(LED_BOARD, HIGH);
			pwm_index=0;
		} else if(results.value==IR_YELL){
      #ifdef DBG
        Serial.println("YELL");
      #endif
			digitalWrite(LED_BOARD, HIGH);
			if(pwm_index<(sizeof(pwm_step)/sizeof(int))-1)
				pwm_index++;
		} else if(results.value==IR_GREEN){
      #ifdef DBG
        Serial.println("GREEN");
      #endif
			digitalWrite(LED_BOARD, HIGH);
			if(pwm_index>0)
				pwm_index--;
		} else if (results.value==IR_SIMPLX) {	//WOOT!!!
      #ifdef DBG
        Serial.println("XX");
      #endif
			woot=1;
		} else {
      #ifdef DBG
        Serial.println("NOOO");
      #endif
			woot=0;
		}
    analogWrite(LED,pwm_step[pwm_index]);
		//digitalWrite(LED, HIGH);
		//Serial.print("pwm_step[");
		//Serial.print(pwm_index);
		//Serial.print("]=");
		//Serial.println(pwm_step[pwm_index]);
		irrecv.resume(); // Receive the next value
	}
  if(woot!=0){
   if (woot==1) {
      if (pwm_index>=(sizeof(pwm_step)/sizeof(int))-2) {
        woot=2;
      }
      pwm_index++;
    } else if (woot==2) {
      if (pwm_index<=1) {
        woot=1;
      }
      pwm_index--;
    }
    analogWrite(LED,pwm_step[pwm_index]);
    //delay(100);
  }
	delay(100-(woot!=0)*60);
	digitalWrite(LED_BOARD, LOW);
}
