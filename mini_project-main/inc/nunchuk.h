#ifndef NUNCHUK_H
#define NUNCHUK_H

#include <stdint.h>

#define MID_VAL_X 131
#define DEAD_ZONE 10

extern int flg_mv_right;
extern int flg_mv_left;
extern int flg_v;
extern int flg_c;

// I2C Helper Functions
void init_i2c(void);
void i2c_waitidle(void);
void i2c_start(uint32_t devaddr, uint8_t size, uint8_t dir);
void i2c_stop(void);
int i2c_checknack(void);
void i2c_clearnack(void);
int i2c_senddata(uint8_t devaddr, const void *data, uint8_t size);
int i2c_recvdata(uint8_t devaddr, void *data, uint8_t size);

// Nunchuk Functions
void init_nunchuk(void);
void read_nunchuk(uint8_t *buffer); // buffer must be length 8
void print_nunchuk_xy(int x, int y); // x and y location to print
void update_flags(void);
void print_flags(int x, int y);

#endif
