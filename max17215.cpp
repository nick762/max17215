#include "max17215.h"
#include <OneWire.h>
#include "Arduino.h"

OneWire _max(A0);

uint8_t MAX17215::_scan(){
	uint8_t addr[8];
	_max.reset_search();
	if(!_max.search(addr)){
		_max.reset_search();
		return 0;
	}else{
		return 1;
	}
}			

uint8_t MAX17215::_cellNum(){
  	uint8_t num = lowByte(_readReg(0xb5, 0x01));
	num = num<<4;
	if(num == 16){
		return 1;
	}else if(num == 32){
		return 2;
	}else if(num == 48){
		return 3;
	}else if(num ==64){
		return 4;
	}else{
		return 0;
	}
}	
		
uint16_t MAX17215::_readReg(uint8_t lsb, uint8_t msb)
{
		boolean present;
		uint16_t vol;
		present = _max.reset();
		_max.skip();
		if(present){
			_max.write(0x69);
			_max.write(lsb);
			_max.write(msb);
			vol = _max.read();
			vol |= _max.read()<<8;
			return vol;
		}else{
			return 0;
		}
}

void MAX17215::_writeData(uint8_t lsb, uint8_t msb, uint8_t* data){
	boolean present;
	present = _max.reset();
	_max.skip();
	if(present){
		_max.write(0x6c);
		_max.write(lsb);
		_max.write(msb,1); //,1
		_max.write(data[0]);
		_max.write(data[1],1);
	}
}

void MAX17215::_fReset()
{ 
	byte dat1[] = {0x00,0x00};
	byte dat2[] = {0x0f,0x00};
	byte dat3[] = {0x01,0x00};
	_writeData(0x80,0x00, dat1);
	delay(10);
	_writeData(0x60,0x00, dat2);
	delay(10);
	_writeData(0xBB,0x00, dat3);
	delay(10);
}
	

uint16_t MAX17215::_readCapacity(){
		uint16_t capacity = _readReg(OW_REG_CAPACITY, OW_REG_MSB);
		return capacity / 256; 
}

uint8_t MAX17215::_readType()
{	
	uint8_t type = lowByte(_readReg(OW_REG_TYPE, OW_REG_MSB));
	type = type<<4;
	//Serial.println(type,HEX);
	return type;
}
	
uint16_t MAX17215::_readFCap(){
	uint16_t capacity = (int16_t)_readReg(OW_REG_FCAP, OW_REG_MSB);
	uint16_t rSense = _readReg(0xcf, 0x01);
	if(rSense == 500){		
		return capacity * 1.0F;
	}else if(rSense == 1000){
		return capacity * 0.5F;
	}else if(rSense == 2000){
		return capacity * 0.25F;
	}else{
		return -1;		
	}
}
	
uint16_t MAX17215::_readVoltage(){
		uint16_t voltage = _readReg(OW_REG_VOLTAGE, OW_REG_MSB);
		return voltage * 1.25F;
}

int MAX17215::_readCurrent(){
	uint16_t rSense = _readReg(0xcf, 0x01);
	int current = (int16_t)_readReg(OW_REG_CURRENT, OW_REG_MSB);
	if(rSense == 500){		
		return current * 31.25F / 100;
	}else if(rSense == 1000){
		return current * 15.625F / 100;
	}else if(rSense == 2000){
		return current * 7.8125F / 100;
	}else{
		return -1;		
	}
}

int MAX17215::_readTemperature(){
	int temperature = (int16_t)_readReg(OW_REG_TEMPERATURE, OW_REG_MSB);
	return temperature / 256;
}

uint16_t MAX17215::_readCell1(){
	uint16_t cell = _readReg(OW_REG_C1V ,OW_REG_MSB);
	return cell * 0.078125F;
}

uint16_t MAX17215::_readCell2(){
	uint16_t cell = _readReg(OW_REG_C2V ,OW_REG_MSB);
	return cell * 0.078125F;
}

uint16_t MAX17215::_readCell3(){
	uint16_t cell = _readReg(OW_REG_C3V ,OW_REG_MSB);
	return cell * 0.078125F;
}

uint16_t MAX17215::_readCell4(){
	uint16_t cell = _readReg(OW_REG_C4V ,OW_REG_MSB);
	return cell * 0.078125F;
}

uint8_t MAX17215::_readSerial(){
	/*char *str;
	uint16_t sn;
	for(int i=0; i<6; i++){
		sn = _readReg(0xd8+i, 0x01);
		sn |= _readReg(0xd8+i, 0x01)<<8;
		*str++ = sn;
	}	
	return str;*/
	uint8_t addr[8];
	_max.reset_search();
	if(!_max.search(addr)){
		_max.reset_search();
		return 0;
	}else{
		//for(int i=0; i<8; i++);
		return addr[7];
	}
}



String MAX17215::GetData()
{	
	String comm = ",";
	String data = "";
	uint16_t voltage,c1v,c2v,c3v,c4v,fCap,capacity;
	uint8_t ser;
	int temperature, current, stat, date;
	uint8_t num = _cellNum();

	voltage = _readVoltage();
	current = _readCurrent();
	capacity = _readCapacity();
	fCap = _readFCap();
	temperature = _readTemperature();
	ser = _readSerial();
	stat = 0;
	date = 0;
	if(num == 1){
		c1v = _readCell1();
		c2v = 0;
		c3v = 0;
		c4v = 0;
	}else if(num ==2){
		c1v = _readCell1();
		c2v = _readCell2();
		c3v = 0;
		c4v = 0;
	}else if(num == 3){
		c1v = _readCell1();
		c2v = _readCell2();
		c3v = _readCell3();
		c4v = 0;
	}else if(num == 4){
		c1v = _readCell1();
		c2v = _readCell2();
		c3v = _readCell3();
		c4v = _readCell4();	
	}
	{
		data = voltage+comm+current+comm+temperature+comm+capacity+comm+c1v+comm+c2v+comm+c3v+comm+c4v+comm+ser+comm+stat+comm+date+comm+fCap;
	}
	return data;
}
