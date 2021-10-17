/* 
 *  FIR FILTER NACH QUELLE:
 *  Grundlagen der digitalen Kommunikationstechnik
 *  Übertragungstechnik – Signalverarbeitung – Netze 
 *  Carsten Roppel
 *  E-Mail: c.roppel@fh-sm.de
 *  Fachbuchverlag Leipzig, 2006
 */

/* Anzahl der Filterkoeffizienten */
const uint8_t nc = 31;

static uint8_t zeiger;

int16_t circular_buffer[nc];
/* Berechnung der 31 Taps mit folgendem Webtool: https://wirelesslibrary.labs.b-com.com/FIRfilterdesigner/ FILTERPARAMETER siehe FIRfilter.txt*/
int16_t coeff[]={4518,-2154,-1807,-1581,-1445,-1373,-1342,-1342,-1360,-1387,-1418,-1450,-1479,-1500,-1513,32767,-1513,-1500,-1479,-1450,-1418,-1387,-1360,-1342,-1342,-1373,-1445,-1581,-1807,-2154,4518}; //HPF 0.5Hz fs=16

/* Multiplikation zweier Q15-Zahlen */
int16_t mult_q15(int16_t factor1, int16_t factor2) 
{ 
 return (int16_t)(((int32_t)factor1 * (int32_t)factor2) >> 15);
}

/* FIR Filter Initialisierung */
void fir_init()
{
  for(uint8_t i = 0; i < nc; i++) circular_buffer[i] = 0; 
}

/* FIR Filter Job */
int16_t fir_filt(int16_t new_sample)
{
  circular_buffer[zeiger] = new_sample; 
  zeiger = (zeiger + 1) % nc;  
  int16_t y = 0; 
  for(uint8_t i = 0; i < nc; i++) 
  { 
    uint8_t k = (zeiger + i) % nc; 
    y += mult_q15(coeff[i], circular_buffer[k]); 
  }
  return(y);
}
