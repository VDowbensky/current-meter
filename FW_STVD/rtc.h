#ifndef _RTC_H_
#define _RTC_H_

#define RTC_INTERRUPT 4

extern void RTC_init (void); 
extern void RTC_setDateTime(BYTE y, BYTE mo, BYTE w, BYTE d, BYTE h, BYTE m, BYTE s);
extern void RTC_readDateTime(void);
extern void RTC_unlock(void);
extern void RTC_lock(void);

extern bit RTC_Flag;


#endif