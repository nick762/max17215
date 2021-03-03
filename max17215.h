#ifndef MAX17215_h
#define MAX17215_h

#include <OneWire.h>

#define OW_REG_MSB					0x00
#define OW_REG_CAPACITY				0x06
#define OW_REG_VOLTAGE				0xda
#define OW_REG_CURRENT				0x0a
#define OW_REG_TEMPERATURE			0x08
#define	OW_REG_C1V					0xd8
#define	OW_REG_C2V					0xd7
#define	OW_REG_C3V					0xd6
#define	OW_REG_C4V					0xd5
#define	OW_REG_FCAP					0x05
#define OW_REG_TYPE					0x21

class MAX17215{
	public:
		uint16_t _readCapacity();
		uint16_t _readVoltage();
		uint16_t _readCell1();
		uint16_t _readCell2();
		uint16_t _readCell3();
		uint16_t _readCell4();
		uint16_t _readFCap();
		uint8_t _readType();
		int _readCurrent();
		int _readTemperature();
		String GetData(void);
		uint8_t _scan();
		void _writeData(uint8_t, uint8_t, uint8_t*);
		uint8_t _readSerial();
		uint8_t _cellNum();
	private:
		uint16_t _readReg(uint8_t, uint8_t);
};

class MAX17320{
	public:
		uint16_t _readCapacity();
		uint16_t _readVoltage();
		uint16_t _readCell1();
		uint16_t _readCell2();
		uint16_t _readCell3();
		uint16_t _readCell4();
		uint16_t _readFCap();
		uint8_t _readType();
		int _readCurrent();
		int _readTemperature();
		String GetData(void);
		uint8_t _scan();
		void _writeData(uint8_t, uint8_t, uint8_t*);
		uint8_t _readSerial();
		uint8_t _cellNum();
	private:
		uint16_t _readReg(uint8_t, uint8_t);
};
#endif
		
	