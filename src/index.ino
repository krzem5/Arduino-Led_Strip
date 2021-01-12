#include <include/led_array.h>



LedArray la(256,6);



uint32_t hue2rgb(uint32_t h){
	if (h<85){
		return ((255-h*3)<<16)|(h*3);
	}
	if (h<170){
		h-=85;
		return ((h*3)<<8)|(255-h*3);
	}
	h-=170;
	return ((h*3)<<16)|((255-h*3)<<8);
}



void setup(){
	la.init();
	la.reset();
	la.update();
	Serial.begin(9600);
}



void loop(){
	static uint32_t tm=0;
	static uint8_t i=0;
	if (Serial&&Serial.available()>0){
		Serial.read();
		tm=millis()+1000;
	}
	if (tm!=0){
		if (tm<millis()){
			tm=0;
			la.reset();
		}
		else{
			for (uint16_t j=0;j<la.length();j++){
				la.pixel(j,hue2rgb((i+j)&0xff),16);
			}
		}
		la.update();
		i++;
		delay(0.1);
	}
}
