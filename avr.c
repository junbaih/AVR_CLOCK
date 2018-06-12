#include "avr.h"

void
ini_avr(void)
{
  WDTCR = 15;
}

void
wait_avr(unsigned int mcsec)
{
  TCCR0 = 3;
  while (mcsec--) {
    TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.00001);
    SET_BIT(TIFR, TOV0);
    WDR();
    while (!GET_BIT(TIFR, TOV0));
  }
  TCCR0 = 0;
}

void wait_avrm(unsigned int msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}