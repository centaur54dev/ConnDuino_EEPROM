# ConnDuino_EEPROM
EEPROM(I2C) library for read/write objects, arrays, strings and fonts 

INSTALLATION 
------------
To download, click the DOWNLOAD ZIP button, uncompress and rename the uncompressed folder to "ConnD_EEPROM". Place the "ConnD_EEPROM" library folder to your arduino environment "<sketch folder>/libraries" subfolder. You can see, and even change, your <sketch folder> in the IDE under the menu File>Preferences. You may need to create the libraries subfolder if this is your first library. Restart the IDE.

HOW TO USE 
------------
1. Include the "ConnD_EEPROM.h" file to your sketch

2. Define a global object of type eepromI2C. At its simplest form this can be accomplished with the following line of code:
`eepromI2C eeprom;`

3. Use the **writeObject** method to write objects of any type. The following code snippet gives some examples:
```
int   i = 1;
float pi = 3.14;
bool  a = true;
MyClass foo;    

uint16_t addr = 1024; //the address where writting will start. 
                      //Each object will be written sequentially after the previous one. 
                      
 addr += eeprom.writeObject(addr, i); //writes the int and increments addr by the number of written bytes
 addr += eeprom.writeObject(addr, pi); //writes a float
 addr += eeprom.writeObject(addr, a);   //writes a boolean
 addr += eeprom.writeObject(addr, foo);   //writes a custom type 

```
4. Use the **readObject** method to read objects of any type from eeprom. The following code snippet reads back the objects written above:
```
int   i;
float pi;
bool  a;;
MyClass foo;    

uint16_t addr = 1024; //the address where reading will start. 
                      
 addr += eeprom.readObject(addr, i); //reads the int and increments addr by the number of read bytes
 addr += eeprom.readObject(addr, pi);   //reads a float
 addr += eeprom.readObject(addr, a);    //reads a boolean
 addr += eeprom.readObject(addr, foo);  //reads a custom type

```


Saving and reading back strings
-------------------------------
1. **writeString** method
``` const char* str1 = "abcdef";
    const char* str2 = "xyz";

uint16_t addr1=1024, addr2;  //the addresses where each string will be written.
addr2 = addr1 + eeprom.writeString(addr1, (uint8_t*)str1);  //write str1 
eeprom.writeString(addr2, (uint8_t*)str2);                  //write str2


2. **readString** method
```
#define BUFSIZE 128
bufsize char buffer[BUFSIZE]; //we need a buffer where read characters will be saved

//The addr1, addr2 variables (uint16_t type) should hold 
//the addresses, where each string was written to. 

eeprom.readString(addr1,  (uint8_t*)buffer,  (uint16_t)BUFSIZE); 
Serial.print(buffer);

eeprom.readString(addr2,  (uint8_t*)buffer,  (uint16_t)BUFSIZE); 
Serial.print(buffer);
```


