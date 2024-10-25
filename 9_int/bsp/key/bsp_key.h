#ifndef __BSP_KEY_H
#define __BSP_KEY_H

typedef enum {
    KEY_NONE = 0,
    KEY0_PRES = 1,
    KEY1_PRES = 2,
    WKUP_PRES = 3,
} KeyValue;

void key_init(void);
KeyValue key_getValue(void);

#endif // __BSP_KEY_H