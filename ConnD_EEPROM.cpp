

#include <Wire.h>
#include <ConnD_EEPROM.h>


void
eepromI2C::setMemAddr(uint16_t memAddr){
	Wire.beginTransmission(_i2cAddr);
	Wire.write((int)(memAddr >> 8));	// MSB
	Wire.write((int)(memAddr & 0xFF));  // LSB
}


uint16_t
eepromI2C::writeFont(uint16_t memaddr, uint8_t byteHeight, uint8_t c0, uint8_t clast,
					uint8_t* widths, uint8_t* bitmaps){

	uint16_t addr = memaddr;
	addr += writeObjectSimple(addr, byteHeight);
	addr += writeObjectSimple(addr, c0);
	addr += writeObjectSimple(addr, clast);

	uint16_t numChars = clast - c0 + 1; 
	for (uint16_t i = 0; i<numChars; i++){
		uint8_t b = pgm_read_byte(widths + i);
		addr += writeObjectSimple(addr, b);
	}

	uint16_t index = 0;
	for (int i = 0; i < numChars; i++){
		uint8_t bytes = byteHeight * pgm_read_byte(widths + i);
		for (int k = 0; k < bytes; k++){
			addr += writeObjectSimple(addr, pgm_read_byte(bitmaps + index));
			index++;
		}
	}

	return   addr - memaddr;
}



uint16_t 
eepromI2C::writeByteArray(uint16_t addr, const uint8_t* arr, uint16_t size){
	
	uint8_t blockBytes		= _blockBytesWrite(addr);
	uint8_t blockBytesIni	=  blockBytes;

	setMemAddr(addr);
	
	int i;	
	for (i=0; i<size; i++){	
		if (blockBytes==0){
			Wire.endTransmission();
			delay(EEPROM_WRITE_DELAY);
			addr += blockBytesIni;
			blockBytes = blockBytesIni = EEPROM_DEFAULT_WRITE_LEN;	
			Wire.beginTransmission(_i2cAddr);
			Wire.write((int)(addr >> 8));		// MSB
			Wire.write((int)(addr & 0xFF));		// LSB
		}
		Wire.write(*arr);
		arr++;
		blockBytes--;
	}
	Wire.endTransmission();
	delay(EEPROM_WRITE_DELAY);
	return i;
}




uint16_t 
eepromI2C::readByteArray(uint16_t addr, uint8_t* arr, uint16_t size){
	
	uint8_t blockBytes = 0;		//in loop: the number of bytes up to block end.
	uint16_t  i;
	
	setMemAddr(addr);
	Wire.endTransmission();
		
	for (i=0; i<size; i++){
		if (blockBytes==0){
           //we need a new block
           blockBytes = _blockSizeRead;
           if (size < blockBytes) blockBytes = size;
           //get the new block
            Wire.requestFrom(_i2cAddr, blockBytes);
        }
		if(Wire.available()){
              //read a byte from buffer
              *arr = Wire.read();
			  arr++;
              blockBytes--;
        }
	}
	return i;
}





uint16_t 
eepromI2C::writeString(uint16_t addr, const char* str, uint16_t strLen){
		
	int i = writeByteArray(addr, (const uint8_t*)str, strLen);
	addr += i;

	//write null termination 
	setMemAddr(addr);
			Wire.write((uint8_t) 0);
			i++;
	Wire.endTransmission();
	delay(EEPROM_WRITE_DELAY);
	return i;
}

uint16_t 
eepromI2C::writeString(uint16_t addr, const char* str){
	//find the end of string
	uint16_t strLen=0;
	for (uint16_t i=0; i< EEPROM_MAX_STRING_WRITE; i++){
		if (*(str+i)==0) break;
		strLen++;
	}
	return writeString(addr, str, strLen);
}

uint16_t 
eepromI2C::readString(uint16_t addr, char* str, uint16_t bufferSize){
	uint8_t blockBytes = 0;		//in loop: the number of bytes up to block end.
	uint16_t  i;
	
	setMemAddr(addr);
	Wire.endTransmission();
	
	for (i=0; i<bufferSize; i++){
		if (blockBytes==0){
           //we need a new block
           blockBytes = _blockSizeRead;
		   if (bufferSize < blockBytes) blockBytes = bufferSize;
           Wire.requestFrom(_i2cAddr, blockBytes);
        }
		if(Wire.available()){
              //read a byte from buffer 
			  *str = Wire.read();
			  if (*str == 0) break;	//null termination found
			  str++;
              blockBytes--;
        }
	}
	*str = 0;		//if breaking from (*str==0) harmless overwritting is done
	return i;

}
#include <Wire.h>

