#include <ConnD_EEPROM.h>
#include <Wire.h>

#define MEM_ADDR 2344        //the memory address where writing/reading starts

eepromI2C ee;                //define an eepromI2C object for eeprom operations
const int MAX_STR_LEN = 50;  


//These are the 5 strings that will be written and read back from eeprom
const char* phrase[5] = {
   "The quick brown fox jumps over the lazy dog.",
   "1234567890",
   "1+1*3 = -[(-4)/(1^2)]",
   "",
   "While making deep excavations we found some quaint bronze jewelry."  //this one exceeds buffer size
};
int address[5];   				//the start memory address for each string
char readback[MAX_STR_LEN*5];    //where the strings will be stored after read from eeprom







void setup(){
   Serial.begin(9600);
   Wire.begin();

   Serial.println ("LIBRARY ConnD_EEPROM");
   Serial.println ("EEPROM STRING READ/WRITE TEST");
 
   Serial.println();
   Serial.println();

   //----Write strings----
   Serial.println ("Writing strings to eeprom");
   long t0 = millis();
   int mem_addr = MEM_ADDR;
      
   for (int i=0; i<5; i++){
     address[i] = mem_addr;
     mem_addr += ee.writeString(mem_addr, (uint8_t*)phrase[i]);   
   }
   long t1 = millis();
   Serial.print ("...writing finished. Elapsed time: "); Serial.println(t1-t0);
   Serial.println();
   delay(100);
   
   //----Read strings back----
   Serial.println ("Reading strings from eeprom");
   t0 = millis();
    
   for (int i=0; i<5; i++){
	   ee.readString(address[i], (uint8_t*)&readback[i*MAX_STR_LEN], MAX_STR_LEN);
   }
   t1 = millis();
   Serial.print ("...reading finished. Elapsed time: "); Serial.println(t1-t0);
   Serial.println();

   //----Print read strings----
   Serial.println ("These are the read strings:"); 
  
   for (int i = 0; i < 5; i++){
	   Serial.print(i+1);
	   Serial.print(": ");
	   Serial.println((const char*)&readback[i*MAX_STR_LEN]);
   }
  }	


void loop(){}



