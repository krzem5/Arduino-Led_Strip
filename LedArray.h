#ifndef __LED_ARRAY_H__
#define __LED_ARRAY_H__



class LedArray{
	public:
		LedArray(uint16_t l,uint16_t p);
		~LedArray(void);
		void init(void);
		void reset(void);
		uint16_t length(void);
		void pixel(uint16_t i,uint32_t c);
		void update(void);
	private:
		uint16_t _l;
		uint16_t _p;
		uint8_t* _px;
		volatile uint8_t* _pt;
		uint8_t _pm;
};



#endif
