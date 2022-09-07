#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatype.h"
#include "utilities.h"

static void parse_tlv_tag(pBytes tag, BYTE *src)
{
	if (0x1f == (src[0]&0x1f))
		tag->size = 2;
	else
		tag->size = 1;

	tag->data = src;

	return;
}

static int parse_tlv_length(pBytes tlv, BYTE *src)
{
	BYTE *t;

	if (0 == src[0])
	{
		tlv->size = 0;
		tlv->data = NULL;
	}
	else if (0x80 > src[0])
	{
		tlv->size = (int)src[0];
		tlv->data = &src[1];
	}
	else if (0x81 == src[0])
	{
		tlv->size = (int)src[1];
		tlv->data = &src[2];
	}
	else if (0x82 == src[0])
	{
		t = (BYTE*)&(tlv->size);
		t[0] = src[2];
		t[1] = src[1];
		t[2] = 0x00;
		t[3] = 0x00;
		tlv->data = &src[3];
	}
	else if (0x83 == src[0])
	{
		t = (BYTE*)&(tlv->size);
		t[0] = src[3];
		t[1] = src[2];
		t[2] = src[1];
		t[3] = 0x00;
		tlv->data = &src[4];
	}
	else
	{
		return -1;
	}

	return 0;
}

//parse the first tlv data
static int tlv_parser(pTlvData tlv, BYTE *src, int size)
{
	int   rv;
	int   offset;
	BYTE *ptr  = src;
	BYTE *head = src;
	
	//Parse Tlv Data Tag
	parse_tlv_tag(&(tlv->tag), ptr);
	//ptr point to the head of the length data of tlv data
	ptr = &(tlv->tag.data[tlv->tag.size]);
	//Parse Tlv Data Length
	rv  = parse_tlv_length(&(tlv->val), ptr);
	if (rv != 0)
	{
		return -1;
	}
	//ptr point to the head of the next tlv data
	if (tlv->val.data==NULL && tlv->val.size==0)
		ptr = &(tlv->tag.data[tlv->tag.size+1]);
	else
		ptr = &(tlv->val.data[tlv->val.size]);
	//check format
	offset = (int)(ptr - head);
	
	return offset<=size ? 0 : -1;
}


//计算tlv对象的个数
int count_tlv_substring(BYTE *src, int size)
{
	int rv;
	int count  = 0;	
	int offset = 0;
	
	BYTE *last;
	BYTE *next = src;
	
	TlvData tlv;

	while(size > 0)
	{
		rv = tlv_parser(&tlv, next, size);
		if (rv != 0)
			return -1;

		last = next;
		if (tlv.val.data==NULL && tlv.val.size==0)
			next = &(tlv.tag.data[tlv.tag.size+1]);	//skip Length
		else
			next = &(tlv.val.data[tlv.val.size]);
		offset = (int)(next - last);
		size  -= offset;
		++count;
	}

	return count;
}

//解析出所有的tlv对象
int unpack_tlv_data(pTlvData *dst, BYTE *src, int size)
{
	int    i;
	int    rv;
	int	   offset;
	int    count;
	BYTE   *last;
	BYTE   *next = src;
	pTlvData ptr = NULL;
	
	count = count_tlv_substring(src, size);
	if (count < 0)
		return -1;

	//Assign memory for dst
	*dst  = (pTlvData)malloc(sizeof(TlvData)*count);
	if (NULL == *dst)
		return -2;

	i = 0;
	ptr = *dst;
	while(i < count)
	{
		//parse tlv data
		rv = tlv_parser(&(ptr[i]), next, size);
		if (rv != 0)
		{
			free(*dst);
			*dst = NULL;
			return -3;
		}
		
		last = next;
		if (ptr[i].val.data==NULL && ptr[i].val.size==0)
			next = &(ptr[i].tag.data[ptr[i].tag.size+1]);
		else
			next = &(ptr[i].val.data[ptr[i].val.size]);
		offset = (int)(next - last);
		size  -= offset;
		++i;
	}

	return count;
}

