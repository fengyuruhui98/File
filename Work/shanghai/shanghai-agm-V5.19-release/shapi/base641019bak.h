#ifndef base64__h
#define base64__h


char * base64_encode( const unsigned char * bindata, char * base64, int binlength );
int base64_decode( const char * base64, unsigned char * bindata );
int base64urlsafe_decode(unsigned char * base64, unsigned char * bindata);

#endif