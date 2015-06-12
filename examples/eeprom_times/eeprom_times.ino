#include <ConnD_EEPROM.h>
#include <Wire.h>


eepromI2C ee;                //define an eepromI2C object for eeprom operations
#define MEM_ADDR 3458        //the memory address wher writing/reading starts


					


//This is the structure we write and read from EEPROM
class MyData{
private:
	long result;   
        
public:
	float  f1;
	float  f2;
	double f3;
	int    i1;
	long   i2;
	long   i3;
	bool  negative;
	
	long getResult(){
		return result;
	}
	long calcResult(){
		//makes a calculation with its data
		//and stores the result to a private variable
		long    i = (i2+i3)/i1;
		double  f = f3/(f1+f2);
		result = f/i;
		//round off
		result += result>0 ? 0.5 : -0.5;
		if (negative) result=-result;
		return result;
	}	
	MyData() : result(0){	}
};
MyData data[5];      //the original data to be written
MyData readback[5];  //the read back from eeprom data


void setup(){
	Serial.begin(9600);
	Wire.begin();
    
   Serial.println ("LIBRARY ConnD_EEPROM");
   Serial.println ("EEPROM BENCHMARK USING BLOCK AND BYTE OPERATIONS");
   Serial.println();
   Serial.println();

        
	//data[0] - result should be 20
	data[0].i1 = 10;
	data[0].i2 = 150000;
	data[0].i3 = 350000;
	
	data[0].f1 = 0.5;
	data[0].f2 = 2.5;
	data[0].f3 = 3.0E+6;
	data[0].negative = false;

	//data[1] - result should be -40
	data[1].i1 = data[0].i1;
	data[1].i2 = data[0].i2;
	data[1].i3 = data[0].i3;
	data[1].f1 = data[0].f1;
	data[1].f2 = data[0].f2;
	data[1].f3 = 2*data[0].f3;
	data[1].negative = true;
	
	//data[2] - result should be 80
	data[2].i1 = data[0].i1;
	data[2].i2 = data[0].i2;
	data[2].i3 = data[0].i3;
	data[2].f1 = data[0].f1;
	data[2].f2 = data[0].f2;
	data[2].f3 = 4*data[0].f3;
	data[2].negative = false;

	//data[3] - result should be -20
	data[3].i1 = data[0].i1;
	data[3].i2 = -data[0].i2;
	data[3].i3 = -data[0].i3;
	data[3].f1 = data[0].f1;
	data[3].f2 = data[0].f2;
	data[3].f3 = data[0].f3;
	data[3].negative = false;

	//data[4] - result should be -2
	data[4].i1 = data[0].i1;
	data[4].i2 = -data[0].i2;
	data[4].i3 = -data[0].i3;
	data[4].f1 = data[0].f1*10;
	data[4].f2 = data[0].f2*10;
	data[4].f3 = -data[0].f3;
	data[4].negative = true;

	Serial.println ("Data before written to eeprom");
	for (int i=0; i<5; i++){
		data[i].calcResult();
		Serial.print("data-"); Serial.print(i); Serial.print(": result = ");
			Serial.println(data[i].getResult());
	}

	Serial.print("Size of each object: "); Serial.println(sizeof(MyData)) ;
	Serial.println();
	
        writeUsingBlocks();
	delay(100);
        readUsingBlocks();
        checkReadData();
        
        writeUsingBytes();
	delay(100);
        readUsingBytes();
        checkReadData();
}

void loop() { 
}


void writeUsingBlocks(){
  Serial.println ("Writing data to eeprom using blocks");
  int memaddr = MEM_ADDR;
  long t0 = millis();
  for (int i=0; i<5; i++){
  	memaddr += ee.writeObject(memaddr, data[i]);
  }
  long t1 = millis();
  Serial.print ("...writing finished. Elapsed time: "); 
  Serial.println(t1-t0);
  Serial.println();
}

void writeUsingBytes(){
  Serial.println ("Writing data to eeprom using bytes");
  int memaddr = MEM_ADDR;
  long t0 = millis();
  for (int i=0; i<5; i++){
  	memaddr += ee.writeObjectSimple(memaddr, data[i]);
  }
  long t1 = millis();
  Serial.print ("...writing finished. Elapsed time: "); 
  Serial.println(t1-t0);
  Serial.println();
}

void readUsingBlocks(){
  Serial.println ("Reading data from eeprom using blocks");
  long t0 = millis();
  int memaddr = MEM_ADDR;
  for (int i=0; i<5; i++){
	memaddr += ee.readObject(memaddr, readback[i]);
  }
  long t1 = millis();
  Serial.print ("Time to read: "); 
  Serial.println(t1-t0);
  Serial.println();
}

void readUsingBytes(){
  Serial.println ("Reading data from eeprom using bytes");
  long t0 = millis();
  int memaddr = MEM_ADDR;
  for (int i=0; i<5; i++){
	memaddr += ee.readObjectSimple(memaddr, readback[i]);
  }
  long t1 = millis();
  Serial.print ("Time to read: "); 
  Serial.println(t1-t0);
  Serial.println();
}


void checkReadData(){
  for (int i=0; i<5; i++){
    Serial.print ("Read object: #"); Serial.println(i); 
    long storedResult = readback[i].getResult();
    Serial.print ("  >stored result = ");	Serial.println(storedResult);
    long newResult = readback[i].calcResult();
    Serial.print ("  >recalculated result = ");	Serial.println(newResult);
    
    if (storedResult== newResult && storedResult==data[i].getResult() )
	Serial.println ("SUCCESS"); 
    else {
	Serial.println ("FAIL");
    }		
  }
  Serial.println();
  Serial.println();
}


