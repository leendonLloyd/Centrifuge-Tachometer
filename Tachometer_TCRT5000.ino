const int IN_A0 = A0; // analog input
const int IN_D0 = 2; // change to pin 2 and d0 nga pin (necessary for interrupt function)

int LEDblue = 9;
int LEDred=7;

volatile unsigned long rev;     //revolutions value
unsigned long int rpm, maxRPM;  //current and max RPM
unsigned long time;             //time for one rev

int RPMdigit, prevRPM;          //variabe for RPM number
int flag = 0;
long prevTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode (IN_A0, INPUT);
  pinMode (IN_D0, INPUT);
  pinMode (LEDblue, OUTPUT);
  pinMode (LEDred, OUTPUT);
  attachInterrupt(0, RPMCount, RISING);      //interrupt function for recording a pulse at negative edge low to high
  rev = 0;
  rpm = 0;
  time = 0;
  Serial.print("TachoMeter: Ready");
  delay(2000);
}

void loop() {
  long currentTime = millis();    //millis() = built in function for current time after running arduino

  long idleTime = currentTime - prevTime; //function for stopping the measurement

  if(rev >= 5){
    if (flag == 0){
      flag = 1;                           //prevents the loop from executing again if flag = 1
    }

    rpm = 30*1000/(millis() - time)*rev;   //gets mo na ni ah, rpm = (60 secs * 1000 milliseconds)/time)*(rev/2)

    if (rpm > maxRPM) maxRPM = rpm;       //records maximum RPM as long as increasing
      time = millis();
      rev = 0;
     int x= rpm;                          //  calculate number of digits of rpm value
     while(x != 0)
     {
       x = x/10;
       RPMdigit++;
     }
            
    if(RPMdigit != prevRPM)              // If the rpm falls to a number with fewer digits , clear lcd
     {
       prevRPM = RPMdigit;
       flag=0;
       Serial.println("SENSOR MEASURING");
     }
     Serial.print(rpm, DEC);              //current RPM
     Serial.println( " RPM");             
     delay(500);
     prevTime = currentTime;              //reset idletime      
    
  }

  if (idleTime > 5000){                 
    if (flag == 1){                       //clear lcd if no readings after 5 sec
      digitalWrite (LEDred, HIGH);
      delay(1000);
      digitalWrite (LEDred, LOW);
      flag = 0;
    }
     digitalWrite (LEDblue, HIGH);
     Serial.print("Maximum RPM");
     Serial.print(maxRPM,DEC);              //display max RPM
     Serial.println("  RPM");
     delay(2000);
     digitalWrite (LEDblue, LOW);
     Serial.println("IDLE STATE");
     Serial.println("READY TO MEASURE");
     delay(2000);
     prevTime = currentTime;    
  }  
}

void RPMCount (){
  rev++;
}
