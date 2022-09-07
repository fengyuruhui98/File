#ifndef _TEST_HARD_H_
#define _TEST_HARD_H_


//º¯Êı
char reader_test_antenna(unsigned char *in_buf, unsigned char *out_buf, unsigned char *out_len);
char reader_test_eeprom(void);
char reader_test_littlesam(char channel, unsigned char *out_buf, unsigned char *out_len);
char reader_test_rtc(char write_control, unsigned char *time_bcd, unsigned char *out_len);
char reader_test_24G(unsigned char *in_buf, unsigned char *out_buf, unsigned char *out_len);

char reader_test_if_24G_exist(void);
char check_24G_module(void);

#endif