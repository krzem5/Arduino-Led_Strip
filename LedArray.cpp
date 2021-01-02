#include <Arduino.h>
#include <LedArray.h>



LedArray::LedArray(uint16_t l,uint16_t p){
	this->_l=l;
	this->_p=p;
	this->_px=malloc(l*3*sizeof(uint8_t));
}



LedArray::~LedArray(void){
	free(this->_px);
	pinMode(this->_p,INPUT);
}



void LedArray::init(void){
	pinMode(this->_p,OUTPUT);
    digitalWrite(this->_p,LOW);
    this->_pt=portOutputRegister(digitalPinToPort(this->_p));
    this->_pm=digitalPinToBitMask(this->_p);
}



void LedArray::reset(void){
	memset(this->_px,0,this->_l*3*sizeof(uint8_t));
}



uint16_t LedArray::length(void){
	return this->_l;
}



void LedArray::pixel(uint16_t i,uint32_t c){
	*(this->_px+i*3)=(c>>8)&0xff;
	*(this->_px+i*3+1)=c>>16;
	*(this->_px+i*3+2)=c&0xff;
}



void LedArray::update(void){
	static uint32_t endTime=0;
	if (endTime > micros()) {
		endTime = micros();
	}
	while ((micros() - endTime) < 300L){
		if (endTime > micros()) {
			endTime = micros();
		}
	}
	noInterrupts();
	volatile uint16_t i=this->_l*3;
	volatile uint8_t* ptr=this->_px,b=*ptr++,hi,lo;
	volatile uint8_t next, bit;
	hi   = *this->_pt |  this->_pm;
	lo   = *this->_pt & ~this->_pm;
	next = lo;
	bit  = 8;

	asm volatile(
	"head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
	"st   %a[port],  %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
	"sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
	"mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
	"dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
	"st   %a[port],  %[next]"  "\n\t" // 2    PORT = next   (T =  7)
	"mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
	"breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
	"rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
	"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
	"nop"                      "\n\t" // 1    nop           (T = 13)
	"st   %a[port],  %[lo]"    "\n\t" // 2    PORT = lo     (T = 15)
	"nop"                      "\n\t" // 1    nop           (T = 16)
	"rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
	"rjmp head20"              "\n\t" // 2    -> head20 (next bit out)
	"nextbyte20:"               "\n\t" //                    (T = 10)
	"ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
	"ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
	"st   %a[port], %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
	"nop"                      "\n\t" // 1    nop           (T = 16)
	"sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
	"brne head20"             "\n"   // 2    if(i != 0) -> (next byte)
	: [port]  "+e" (this->_pt),
	[byte]  "+r" (b),
	[bit]   "+r" (bit),
	[next]  "+r" (next),
	[count] "+w" (i)
	: [ptr]    "e" (ptr),
	[hi]     "r" (hi),
	[lo]     "r" (lo));




	interrupts();

	endTime = micros(); // Save EOD time for latch on next call
}
