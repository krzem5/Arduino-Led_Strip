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



void LedArray::pixel(uint16_t i,uint32_t c,uint8_t v){
	*(this->_px+i*3)=(c>>8)&0xff;
	*(this->_px+i*3+1)=c>>16;
	*(this->_px+i*3+2)=c&0xff;
}



void LedArray::update(void){
	noInterrupts();
	volatile uint8_t e=*this->_px;
	volatile uint8_t n=(*this->_pt)&(~this->_pm);
	volatile uint8_t b=8;
	volatile uint16_t i=this->_l*3;
	__asm__ volatile("_l:\nst %a[p],%[h]\nsbrc %[e],7\nmov %[n],%[h]\ndec %[bi]\nst %a[p],%[n]\nmov %[n],%[l]\nbreq _nb\nrol %[e]\nrjmp .+0\nnop\nst %a[p],%[l]\nnop\nrjmp .+0\nrjmp _l\n_nb:\nldi %[bi],8\nld %[e],%a[bp]+\nst %a[p],%[l]\nnop\nsbiw %[i],1\nbrne _l":[p]"+e"(this->_pt),[e]"+r"(e),[n]"+r"(n),[bi]"+r"(b),[i]"+w"(i):[bp]"e"(this->_px+1),[h]"r"((*this->_pt)|this->_pm),[l]"r"(n));
	interrupts();
}
