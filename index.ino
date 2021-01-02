#include <LedArray.h>



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
}



void loop(){
	static uint8_t i=0;
	for (uint16_t j=0;j<la.length();j++){
		la.pixel(j,hue2rgb((i+j)&0xff));
	}
	la.update();
	i++;
	delay(0.1);
}
