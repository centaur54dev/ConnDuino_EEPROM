#ifndef _CONNDUINO_EEPROM_I2C___
#define _CONNDUINO_EEPROM_I2C___




#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Wire.h>


#define EEPROM_I2C_DEFAULT_ADDRESS 0x50		///default device i2c address
#define EEPROM_DEFAULT_PAGE_LEN		64		///eeprom page size in bytes
#define EEPROM_DEFAULT_WRITE_LEN	16		///bytes of blocks the library uses for write
#define EEPROM_DEFAULT_READ_LEN		30		///bytes of blocks the library uses for read (limited by Wire libraru buffer)
#define EEPROM_WRITE_DELAY			5		///delay after a write operation in msec					
#define EEPROM_MAX_STRING_WRITE		512		///an upper limit for safe looping required by the writeString routine




class eepromI2C{
	const uint8_t _i2cAddr;			///the i2c address of the eeprom module
	const uint8_t _blockSizeRead;	///size of read block size

	uint8_t _blockBytesWrite(uint16_t addr){
		//return the number of bytes up to block border starting from given address
		return (addr / EEPROM_DEFAULT_WRITE_LEN + 1)*EEPROM_DEFAULT_WRITE_LEN - addr;
	}

public:
	
	//Constructor 
	eepromI2C(uint16_t i2c_address = EEPROM_I2C_DEFAULT_ADDRESS,
				uint8_t blockSizeRead = EEPROM_DEFAULT_READ_LEN)
		: _i2cAddr(i2c_address), 
			_blockSizeRead(blockSizeRead){}


	uint8_t getI2CAddr() {		return _i2cAddr;	}
	void setMemAddr(uint16_t memaddr);

	/*!Writes a proportional font to device
		* The widths and bitmaps byte arrays should be declared with PROGMEM.
		* Their size should be equal to the number of characters in font.
		* All the ASCII characters between c0 and clast are written.
		> memaddr: 	  the start memory address for he write
		> byteHeight: the number of bytes(=8 pixels) per character height
		> c0:		  the first character of the font
		> clast:	  the last character of the font
		> widths:	  array with the pixel width of each character.
		> bitmaps:	  array with the monochrome pixel values of each character
	 */
	uint16_t  writeFont(uint16_t memaddr, uint8_t byteHeight, uint8_t c0, uint8_t clast,
						uint8_t* widths, uint8_t* bitmaps);


	/*!Writes a byte array to device. 
		* Returns the number of written bytes.
		> memaddr: 	the memory address for the 1st byte to be written.	
		> arr:		the array of bytes.
		> size:		the size of arr.
	*/
	uint16_t writeByteArray(uint16_t memaddr, const uint8_t* arr, uint16_t size);


	/*!Writes a byte array from progmem to device. 
		* Returns the number of written bytes.
		> memaddr: 	the memory address for the 1st byte to be written.	
		> arr:		the array of bytes.
		> size:		the size of arr.
	*/
	uint16_t writeByteArrayPROGMEM(uint16_t memaddr, const uint8_t* arr, uint16_t size);


	/*!Reads a byte array from the device. 
		* Returns the number of read bytes. 
		> memaddr: 	the memory address for the 1st byte to be read	
		> arr:		an array of bytes where the read bytes will be saved.
		> size:		the size of arr and the requested number of read bytes.
	*/
	uint16_t readByteArray(uint16_t memaddr, uint8_t* arr, uint16_t size);


	/*!Writes a string of fixed length to device. 
		* A null termination is appended after all chars have been written.
		* Returns the number of written chars including the null termination.
		> memaddr: 	the memory address for the 1st char to be written.
		> str:		the array of chars, representing the string.
		> strLen:	the length of the string  (excluding null termination). 
	*/
	uint16_t writeString(uint16_t memaddr, const char* str, uint16_t strLen);


	/*!Writes a string of arbitrary size (null terminated) to device. 
		* The routine first searches for the null termination and then 
		  calls writeString with the calculated string length.
		* Returns the number of written chars including the null termination.
		> memaddr: 	the memory address for the 1st char to be written.
		> str:		the array of chars, representing the string.
	*/
	uint16_t writeString(uint16_t memaddr, const char* str);

	/*!Reads a string from device. 
		* Reading will stop if buffer is full or char 0 (=null terrmination) is read.
		* Returns the number of bytes read (not including null termination)
		> memaddr: 	  the memory address for the 1st string char to be read.
		> str:		  an array of chars, where the read string will be saved.
		> bufferSize: the size of the str array. If read chars exceed this value
					  reading stops.
	*/
	uint16_t readString(uint16_t memaddr, char* str, uint16_t bufferSize);


	/*!Writes an object of any type to device. 
		* The type of the object is known at compile-time by the template parameter.
		* Internally, the actual writting of data is performed by a call to writeByteArray,
		  therefore no severe program size growth is expected when misusing this function 
		  with many different types.
		* Returns the number of written bytes. 
		> memaddr: 	the start memory address where the object will be written.
		> value:	the object to be written.
	*/
	template <class T> 
	uint16_t writeObject(uint16_t memaddr, const T& value){

		const uint8_t* p = (const uint8_t*)(const void*)&value;
		uint16_t objSize = sizeof(T);		
		return writeByteArray(memaddr, p, objSize);
	}
	


	/*!Reads an object of any type from device. 
		* The type of the object is known at compile-time by the template parameter.
		* Internally, the actual writting of data is performed by a call to readByteArray,
		  therefore no severe program size growth is expected when misusing this function 
		  with many different types.
		* Returns the number of read bytes. 
		> memaddr: 	the start memory address where the object will be read from.
		> value:	the target object where the read data will be saved.
	*/
	template <class T> 
	uint16_t readObject(uint16_t memaddr, T& value){
		uint8_t* p = (uint8_t*)(void*)&value;
		uint16_t objSize = sizeof(T);		
		return readByteArray(memaddr, p, objSize);
	}

	/*!Writes an object of any type to device byte by byte. 
		* For testing or compatibility use. 
		* Normally writting an object byte by byte is much slower except
		  for 1-byte objects. 
		* The type of the object is known at compile-time by the template parameter.
		* Returns the number of written bytes. 
		> memaddr: 	the start memory address where the object will be written.
		> value:    the object to be written.
	*/
	template <class T>
	uint16_t writeObjectSimple(uint16_t memaddr, const T& value){

		const uint8_t* p = (const uint8_t*)(const void*)&value;
		uint16_t i;
		for (i = 0; i < sizeof(value); i++){
			setMemAddr(memaddr);
			Wire.write(*p++);
			Wire.endTransmission();
			memaddr++;
			delay(EEPROM_WRITE_DELAY);
		}

		return i;
	}

	/*!Reads an object of any type from device byte by byte. 
		* For testing or compatibility use. 
		* Normally reading an object byte by byte is much slower except
		  for 1-byte objects. 
		* The type of the object is known at compile-time by the template parameter.
		* Returns the number of read bytes. 
		> memaddr: 	the start memory address where the object will be read from.
		> value:	the target object where the read data will be saved.
	*/
	template <class T>
	uint16_t readObjectSimple(uint16_t memaddr, T& value){

		uint8_t* p = (uint8_t*)(void*)&value;
		uint8_t objSize = sizeof(value);
		uint16_t i;

		for (i = 0; i < objSize; i++){
			setMemAddr(memaddr);
			Wire.endTransmission();
			Wire.requestFrom(_i2cAddr, (uint8_t)1);
			if (Wire.available()){
				*p++ = Wire.read();
			}
			memaddr++;
		}
		return i;
	}
};
	













#endif