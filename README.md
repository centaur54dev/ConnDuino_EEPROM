# ConnDuino_EEPROM
EEPROM(I2C) library for read/write objects, arrays, strings and fonts 

INSTALLATION 
------------
To download, click the DOWNLOAD ZIP button, uncompress and rename the uncompressed folder to "ConnD_EEPROM". Place the "ConnD_EEPROM" library folder to your arduino environment "<sketch folder>/libraries" subfolder. You can see, and even change, your <sketch folder> in the IDE under the menu File>Preferences. You may need to create the libraries subfolder if this is your first library. Restart the IDE.

HOW TO USE 
------------
1. Include the "Wire.h" and "ConnD_EEPROM.h" file to your sketch.

2. Define a global object of type eepromI2C. At its simplest form this can be accomplished with the following line of code:
`eepromI2C eeprom;`

3. Use the **writeObject** method to write objects of any type. It returns the number of written bytes. The following code snippet gives some examples:
```
//The next objects will be saved to eeprom
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
4. Use the **readObject** method to read objects of any type from eeprom. It returns the number of read bytes. The following code snippet reads back the objects written previously:
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


Writting and reading strings
-------------------------------
1. The **writeString** method writes a null terminated string to eeprom and returns the number of written bytes (this include the null character too). Here is an example:
``` 
const char* str1 = "abcdef";
const char* str2 = "xyz";

uint16_t addr1=1024, addr2;  //the addresses where each string will be written.
addr2 = addr1 + eeprom.writeString(addr1, str1);  //write str1 
eeprom.writeString(addr2, str2);                  //write str2
```

2. The **readString** method reads a string from eeprom and returns the number of read chars (not including the null character). Here is an example:
```
#define BUFSIZE 128
bufsize char buffer[BUFSIZE]; //we need a buffer where read characters will be saved

//The addr1, addr2 variables (uint16_t type) should hold 
//the addresses, where each string was written to. 

eeprom.readString(addr1,  buffer,  BUFSIZE); //read str1
Serial.print(buffer); 

eeprom.readString(addr2,  buffer,  BUFSIZE); //read str2
Serial.print(buffer);
```

Arrays
------
The **writeByteArray** method writes a byte array to eeprom and returns the number of written bytes. It is quite useful for writing monochrome bitmap images that are converted to byte arrays (using the LCDAssistant software for instance).
```
#define ARRLEN 8
uint8_t arr[ARRLEN] = {1,2,4,8,16,32,64,128};

uint16_t addr=1024; //the address where the first array element will be written
eeprom.writeByteArray(addr, arr, ARRLEN);  //writes the array
```

The **readByteArray** method reads a byte array from eeprom and returns the number of read bytes. 
```
#define ARRLEN 8
uint8_t arr[ARRLEN];

uint16_t addr=1024; //the address where the first array element was written
eeprom.readByteArray(addr, arr, ARRLEN);  //writes the array
```

Fonts
-----
The **writeFont** method, makes possible to write proportional font bitmap characters to EEPROM. The widths of every character as well as their monochrome bitmaps should reside in PROGMEM arrays. Here is a basic example, using this method:
```
PROGMEM uint8_t FONT_BITMAPS_ARR[] = {...}
PROGMEM uint8_t FONT_WIDTHS_ARR[] = {...}

uint8_t byteHeight = 2;      //character height = 2*8=16 pixels
const char firstChar = ' ';  //space 
const char lastChar = '~';   //all ascii characters between this and firstChar are included.
                             // this means lastChar-firstChar+1   number of characters.
                             //the PROGMEM should match this number. 

uint16_t addr=8192;  //the memory address where writing will begin. 
addr += eeprom.writeFont(addr, byteHeight, firstchar, lastchar, 
                          FONT_WIDTHS_ARR, 
                          FONT_BITMAPS_ARR);

```

Customizing eeprom object
-------------------------
### I2C address
By default, the eepromI2C class assumes that the I2C address of the EEPROM device is 0x50. This can be altered though during construction of the eepromI2C global object. For a device having I2C address 0x52, we may use:
`eepromI2C eeprom(0x52);`

### Read block size
Reading from EEPROM is perfomed in blocks of bytes. By default, the eepromI2C class uses blocks of 30 bytes which is the maximum size supported by the #Wire# library, without modifying it. For performance reasons (i.e. reading of many small objects) it is possible to specify a different block size. For example, the next line of code creates an eepromI2C object, using I2C address 0x50 and block size equal to 4:
`eepromI2C eeprom(0x50, 4);`

### Other options
Hacking into the "ConnD_EEPROM.h" header file, the following defines may be customized:
```
EEPROM_DEFAULT_WRITE_LEN	16    //Block size for writing. 
                                //It should divide perfectly the page size (normally 64)
                                
EEPROM_WRITE_DELAY  5           //Delay in msecs after each write operation. 
                                //Should be obtained from the device datasheet

```


