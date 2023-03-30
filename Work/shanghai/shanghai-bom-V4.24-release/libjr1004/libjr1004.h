//libjr1004.h

int pf_mobile_ticket_device_init(char *inbuf);
int pf_mobile_ticket_para_init(char *inbuf);

int pf_mobile_ticket_para_sha1(char *inbuf);

int pf_mobile_ticket_get_info(char *inbuf,int inbytes,char *outbuf);
int pf_mobile_normal_ticket_entry(char *inbuf,char *outbuf);
int pf_mobile_normal_ticket_exit(char *inbuf,char *outbuf);
int pf_mobile_normal_ticket_update(char *inbuf,char *outbuf);
int pf_mobile_normal_ticket_get_proof(char *inbuf,char *outbuf);

int pf_mobile_staff_ticket_entry(char *inbuf,char *outbuf);
int pf_mobile_staff_ticket_exit(char *inbuf,char *outbuf);
int pf_mobile_staff_ticket_update(char *inbuf,char *outbuf);
int pf_mobile_staff_ticket_get_proof(char *inbuf,char *outbuf);
int pf_mobile_staff_ticket_lock(char *inbuf,char *outbuf);

int pf_mobile_staff_ticket_get_record(char  *inbuf,char  *outbuf);
int pf_mobile_ticket_get_record(char  *inbuf,char  *outbuf);
int pf_mobile_ticket_get_0221(char *outbuf);


int oda_ticket_get_zone(char  *buf);
int oda_zone_ticket_get_pan(char *buf);
int oda_zone_ticket_entry(char  *inbuf,char  *obuf);
int oda_zone_ticket_exit(char  *inbuf,char  *obuf);
int oda_zone_ticket_get_proof(char  *inbuf,char  *obuf);
int oda_zone_ticket_update(char  *inbuf,char  *obuf);
int oda_zone_ticket_enroll(char  *inbuf,char  *obuf);
int oda_zone_ticket_disable(char  *inbuf, char  *obuf); 



