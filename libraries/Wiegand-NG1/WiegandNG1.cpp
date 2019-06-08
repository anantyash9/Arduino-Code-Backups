#include <WiegandNG1.h>

// pcintbranch

volatile unsigned long	WiegandNG1::_lastPulseTime;	// time last bit pulse received
volatile unsigned int	WiegandNG1::_bitCounted;		// number of bits arrived at Interrupt pins
volatile unsigned char	*WiegandNG1::_buffer;		// buffer for data retention
unsigned int			WiegandNG1::_bufferSize;		// memory (bytes) allocated for buffer


void shift_left1(volatile unsigned char *ar, int size, int shift)
{
	while (shift--) {								// for each bit to shift ...
		int carry = 0;								// clear the initial carry bit.
		int lastElement = size-1;
		for (int i = 0; i < size; i++) {			// for each element of the array, from low byte to high byte
			if (i!=lastElement) {
				// condition ? valueIfTrue : valueIfFalse
				carry = (ar[i+1] & 0x80) ? 1 : 0;
				ar[i] = carry | (ar[i]<<1);
			}
			else {
				ar[i] <<=1;
			}
		}   
	}
}  

void WiegandNG1::clear() {							// reset variables to start new capture
	_bitCounted=0;
	_lastPulseTime = millis();
	memset((unsigned char *)_buffer,0,_bufferSize);
	interrupts();									// allow interrupt
}

void WiegandNG1::pause() {
	noInterrupts();									// disable interrupt so that user can process data 
}

volatile unsigned char * WiegandNG1::getRawData() {
	return _buffer;									// return pointer of the buffer
}

unsigned int WiegandNG1::getPacketGap() {
	return _packetGap;
}

unsigned int WiegandNG1::getBitAllocated() {
	return _bitAllocated;
}

unsigned int WiegandNG1::getBitCounted() {
	return _bitCounted;
}

unsigned int WiegandNG1::getBufferSize() {
	return _bufferSize;
}

bool WiegandNG1::available() {
	bool ret=false;
	noInterrupts();
	unsigned long tempLastPulseTime = _lastPulseTime;
	interrupts();

	unsigned long sysTick = millis();
	//	if ((sysTick - _lastPulseTime) > _packetGap) {	// _packetGap (ms) laps
	if ((sysTick - tempLastPulseTime) > _packetGap) {	// _packetGap (ms) laps
		if(_bitCounted>0) {							// bits found, must have data, return true
			if(_bitCounted<8) {
				Serial.print(_bitCounted);
				Serial.print(", ");
				Serial.print(sysTick);
				Serial.print(", ");
				Serial.print(_lastPulseTime);
				Serial.print(",");
				Serial.println(tempLastPulseTime);
			}
			ret=true;
		}
		else
		{
			_lastPulseTime = millis();
		}
	}
	return ret;
}

void WiegandNG1::ReadD0 () {
	_bitCounted++;									// increment bit count for Interrupt connected to D0
	shift_left1(_buffer,_bufferSize,1);				// shift 0 into buffer
	_lastPulseTime = millis();						// keep track of time last wiegand bit received
}

void WiegandNG1::ReadD1() {
	_bitCounted++;									// increment bit count for Interrupt connected to D1
	if (_bitCounted > (_bufferSize * 8)) {
		_bitCounted=0;								// overflowed, 
	} else {
		shift_left1(_buffer,_bufferSize,1);			// shift 1 into buffer
		_buffer[_bufferSize-1] |=1;					// set last bit 1
		_lastPulseTime = millis();					// keep track of time last wiegand bit received
	}
}

bool WiegandNG1::begin(unsigned int allocateBits, unsigned int packetGap) {
	bool ret;
	// newer versions of Arduino provide pin to interrupt mapping
	ret=begin(2, 3, allocateBits, packetGap);
	return ret;
}

bool WiegandNG1::begin(uint8_t pinD0, uint8_t pinD1, unsigned int allocateBits, unsigned int packetGap) {
	if (_buffer != NULL) {
		delete [] _buffer;
	}
	_packetGap = packetGap;
	_bitAllocated = allocateBits;
	
	_bufferSize=(_bitAllocated/8);						// calculate the number of bytes required to store wiegand bits
	if((_bitAllocated % 8) >0) _bufferSize++;			// add 1 extra byte to cater for bits that are not divisible by 8
	_buffer = new unsigned char [_bufferSize];			// allocate memory for buffer
	if(_buffer == NULL) return false;					// not enough memory, return false

	clear();
	
	pinMode(pinD0, INPUT);								// set D0 pin as input
	pinMode(pinD1, INPUT);								// set D1 pin as input
	attachInterrupt(digitalPinToInterrupt(pinD0), ReadD0, FALLING);			// hardware interrupt - high to low pulse
	attachInterrupt(digitalPinToInterrupt(pinD1), ReadD1, FALLING);			// hardware interrupt - high to low pulse
	return true;
}

WiegandNG1::WiegandNG1() {

}

WiegandNG1::~WiegandNG1() {
	if (_buffer != NULL) {
		delete [] _buffer;
	}
}


