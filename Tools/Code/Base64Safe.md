# base64.c
```C
#ifndef _BASE64_C_
#define _BASE64_C_

#include <string.h>

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char * base64_encode(const unsigned char * bindata, char * base64, int binlength){
    int i, j;
    unsigned char current;
    for(i = 0, j = 0; i < binlength; i += 3){
        
    }
}



#endif
```