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
 addr += eeprom.writeObject(addr, pi);
 addr += eeprom.writeObject(addr, a);
 addr += eeprom.writeObject(addr, foo);

```

