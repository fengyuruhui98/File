#ifndef		__TLV_H_
#define		__TLV_H_


// TLV�ṹ��
//struct TLVEntity {
//	unsigned char* Tag; //���
//	unsigned char* Length; //���ݳ���
//	unsigned char* Value; //����
//	unsigned int TagSize; //���ռ���ֽ���
//	unsigned int LengthSize; //���ݳ���ռ���ֽ���
//	TLVEntity* Sub_TLVEntity; //��Ƕ��TLVʵ��
//};

// TLV�ṹ��
typedef struct TLVEntity{
	unsigned short	Tag; 		//���
	unsigned long	Length; 	//���ݳ���
	unsigned char	*Value; 	//����
	unsigned long 	UpArray; 	//���ռ���ֽ���
	unsigned long	LengthSize; 	//���ݳ���ռ���ֽ���
	unsigned long	ArraySize;	//
	struct TLVEntity	*Sub_TLVEntity; 	//��Ƕ��TLVʵ��
	//struct TLVEntity		Sub_TLVEntity[20]; 	//��Ƕ��TLVʵ��
}TLVEntity_t;


void construct_tlv(unsigned char *in_buf, long in_len, struct TLVEntity *tlv, unsigned char value);
TLVEntity_t * search_tlv(unsigned short *tag_buf, long tag_len, struct TLVEntity *tlv);
char init_gpo(unsigned char *cur_timebcd, unsigned long tranamount, unsigned char cappflag, unsigned char *out_buf, struct TLVEntity *tlv);

#endif