#include <IRremote.h>

const int RECV_PIN = 7;
const int R_PIN = 2, G_PIN = 3, B_PIN = 4, W_PIN = 5; //Pins of the colours

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long old_signal = 0;
int R = 0, G = 0, B = 0, W = 0; //intensity of colours
int R_out = 0, G_out = 0, B_out = 0, W_out = 0; //Output pipe for colour intensity
int R_toggled=0, G_toggled=0, B_toggled=0;
int mode = 1; //operation mode
int i = 10;
int blinkInterval=1000;
unsigned long currentTime = 0; //all times in milliseconds
unsigned long previousTime = 0; 
int fadeAmount=5, fadeVar=5;
int rb_colour;
int rand_R = 0, rand_G = 0, rand_B = 0; //Random colour vars
int RAND_PORT=10;

void setup(){
  Serial.begin(9600);
  
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");  irrecv.blink13(true);
  randomSeed(analogRead(RAND_PORT));
  
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(W_PIN, OUTPUT);
}

void loop(){
  if (irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    if (results.value == 0XFFFFFFFF) results.value = old_signal;
    
    switch(results.value){
          case 0xFFA25D: //decrease Red
          Serial.println("CH-");
          R=R-i;
          break;
          
          case 0xFF629D: //increase Red
          Serial.println("CH");
          R=R+i;
          break;
          
          case 0xFFE21D: //Turn colours off
          Serial.println("CH+");
          R=0;
          G=0;
          B=0;
          W=0;
          break;
          
          case 0xFF22DD: //decrease Green
          Serial.println("|<<");
          G=G-i;
          break;
          
          case 0xFF02FD: //increase Green
          Serial.println(">>|");
          G=G+i;
          break;
          
          case 0xFFC23D: //max brightness
          Serial.println(">||");
          R=255;
          G=255;
          B=255;
          W=255;
          break;
          
          case 0xFFE01F: //decrease Blue
          Serial.println("-");
          B=B-i;
          break;
          
          case 0xFFA857: //increase Blue
          Serial.println("+");
          B=B+i;
          break;
          
          case 0xFF906F: //faster
          Serial.println("EQ");
          if (mode==2){
            blinkInterval=blinkInterval*0.9;
            }
          else if ((mode==3)||(mode==4)){
            fadeAmount=fadeAmount+1;
            }
          Serial.println("fading speed:");
          Serial.println(fadeAmount);
          break;
          
          case 0xFF6897: //decrease White
          Serial.println("0");
          W=W-i;
          break;
          
          case 0xFF9867: //increase White
          Serial.println("100+");
          W=W+1;
          break;
          
          case 0xFFB04F: //slower
          Serial.println("200+");
          if (mode==2){
            blinkInterval=blinkInterval*1.1;
            }
          else if ((mode==3)||(mode==4)){
            fadeAmount=fadeAmount-1;
            if (fadeAmount<1)fadeAmount=1;
            }
          Serial.println("fading speed:");
          Serial.println(fadeAmount);
          break;
          
          case 0xFF30CF: //Solid Mode
          Serial.println("1");
          mode = 1;
          break;
          
          case 0xFF18E7: //Blinking Mode
          Serial.println("2");
          mode = 2;
          break;
          
          case 0xFF7A85: //Primary Fade
          Serial.println("3");
          mode = 3;
          break;
          
          case 0xFF10EF: //Rainbow Fade          
          Serial.println("4");
          mode = 4;
          rb_colour=1;
          R_out=255;
          break;int R_toggled=0, G_toggled=0, B_toggled=0;

          
          case 0xFF38C7:
          Serial.println("5");
          mode = 5;
          break;
          
          case 0xFF5AA5:
          Serial.println("6");
          mode = 6;
          break;
          
          case 0xFF42BD:
          Serial.println("7");
          mode = 7;
          break;
          
          case 0xFF4AB5:
          Serial.println("8");
          break;
          
          case 0xFF52AD:
          Serial.println("9");
          break;
        }
    
    old_signal = results.value;
    irrecv.resume(); // Receive the next signal
  }
  switch(mode){
    case 1:
    R_out=R;
    G_out=G;
    B_out=B;
    W_out=W;
    break;
    
    case 2:      
    currentTime = millis();
    
    if (currentTime - previousTime >= blinkInterval) {
      // if the LED is off turn it on and vice-versa:
      if ((R==R_out) && (G==G_out) && (B==B_out) && (W==W_out)) {
        R_out=0; G_out=0; B_out=0; W_out=0;
      } else {R_out=R; G_out=G; B_out=B; W_out=W;}
      previousTime = currentTime; // save the last time you blinked the LED
    }
    break;
    
    case 3: //Primary Fade
    if ((R_out>0) || (R_toggled==0)){
      R_out=R_out+fadeVar;
      if (R_out>255) R_out=255;
      else if (R_out<0) R_out=0;
      if (R_out <= 0) {
        fadeVar = fadeAmount;
      }      
      else if (R_out >= 255){
        fadeVar = -fadeAmount;
        R_toggled=1;
        }
    }
    else if ((G_out>0) || (G_toggled==0)){
      G_out=G_out+fadeVar;
      if (G_out>255) G_out=255;
      else if (G_out<0) G_out=0;
      if (G_out <= 0) {
        fadeVar = fadeAmount;
      }
      else if (G_out >= 255){
        fadeVar = -fadeAmount;
        G_toggled=1;
        }
    }
    else if ((B_out>0) || (B_toggled==0)){
      B_out=B_out+fadeVar;
      if (B_out>255) B_out=255;
      else if (B_out<0) B_out=0;
      if (B_out <= 0) {
        fadeVar = fadeAmount;
      }
      else if (B_out >= 255){
        fadeVar = -fadeAmount;
        B_toggled=1;
        }
    } else {
      R_toggled=0;
      G_toggled=0;
      B_toggled=0;
      }      
            
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
    break;
    
    case 4: //Rainbow Fade
    if (rb_colour==1){
      R_out=R_out-fadeAmount;
      if (R_out<0) R_out=0;
      G_out=G_out+fadeAmount;
      if (G_out>255) G_out=255;
      if ((R_out==0) && (G_out==255)) rb_colour=2;
    }
    else if (rb_colour==2){
      G_out=G_out-fadeAmount;
      if (G_out<0) G_out=0;
      B_out=B_out+fadeAmount;
      if (B_out>255) B_out=255;
      if ((G_out==0) && (B_out==255)) rb_colour=3;
    }
    else if (rb_colour==3){
      B_out=B_out-fadeAmount;
      if (B_out<0) B_out=0;
      R_out=R_out+fadeAmount;
      if (R_out>255) R_out=255;
      if ((B_out==0) && (R_out==255)) rb_colour=1;
    }
    delay(50);
    break;
    
    case 5: //Random Colour
    randomSeed(analogRead(RAND_PORT));
    R_out = random(255);
    randomSeed(analogRead(RAND_PORT));
    G_out = random(255);
    randomSeed(analogRead(RAND_PORT));
    B_out = random(255);
    W_out = 0;
    break;          

    case 6: //Warm White
    R_out = 0;
    G_out = 0;          
    B_out = 0;
    W_out = 255;
    break;
  }
 
  analogWrite(R_PIN, R_out);
  analogWrite(G_PIN, G_out);
  analogWrite(B_PIN, B_out);
  analogWrite(W_PIN, W_out);
}

//Feature PAD:
//Fading initialization properly
//Random colour
//save favourite colour
//Warm White Mode
