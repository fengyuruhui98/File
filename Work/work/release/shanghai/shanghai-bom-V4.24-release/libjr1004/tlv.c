#include "stdio.h"

#include "tlv.h"
#include "time_tools.h"

//void Construct(unsigned char* buffer, unsigned int bufferLength, TLVEntity* tlvEntity, 
//			unsigned int& entityLength, unsigned int status)
//{
//int currentTLVIndex = 0;
//int currentIndex = 0;
//int currentStatus = 'T'; //״̬�ַ�
//unsigned long valueSize = 0;
//
//	while(currentIndex < bufferLength)
//	{
//		switch(currentStatus)
//		{
//		case 'T':
//			valueSize = 0;
//			//�ж��Ƿ�һ�ṹ
//			if((status == 1 && buffer[currentIndex] & 0x20) != 0x20)
//			{
//				tlvEntity[currentTLVIndex].Sub_TLVEntity = NULL; //��һ�ṹʱ����Tag�ÿ�
//				//�ж��Ƿ���ֽ�Tag
//				if((buffer[currentIndex] & 0x1f) == 0x1f)
//				{
//					int endTagIndex = currentIndex;
//					while((buffer[++endTagIndex] & 0x80) == 0x80); //�жϵڶ����ֽڵ����λ�Ƿ�Ϊ1
//					int tagSize = endTagIndex - currentIndex + 1; //����Tag���������ֽ�
//					tlvEntity[currentTLVIndex].Tag = new unsigned char[tagSize];
//					memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, tagSize); 
//					tlvEntity[currentTLVIndex].Tag[tagSize] = 0;
//					tlvEntity[currentTLVIndex].TagSize = tagSize;
//					currentIndex += tagSize;
//				}
//				else
//				{
//					tlvEntity[currentTLVIndex].Tag = new unsigned char[1];
//					memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, 1);
//					tlvEntity[currentTLVIndex].Tag[1] = 0;
//					tlvEntity[currentTLVIndex].TagSize = 1;
//					currentIndex += 1;
//				}
//			}
//			else
//			{
//				//�ж��Ƿ���ֽ�Tag
//				if((buffer[currentIndex] & 0x1f) == 0x1f)
//				{
//					int endTagIndex = currentIndex;
//					while((buffer[++endTagIndex] & 0x80) == 0x80); //�жϵڶ����ֽڵ����λ�Ƿ�Ϊ1
//					int tagSize = endTagIndex - currentIndex + 1; //����Tag���������ֽ�
//					tlvEntity[currentTLVIndex].Tag = new unsigned char[tagSize];
//					memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, tagSize); 
//					tlvEntity[currentTLVIndex].Tag[tagSize] = 0;
//					tlvEntity[currentTLVIndex].TagSize = tagSize;
//					currentIndex += tagSize;
//				}
//				else
//				{
//					tlvEntity[currentTLVIndex].Tag = new unsigned char[1];
//					memcpy(tlvEntity[currentTLVIndex].Tag, buffer + currentIndex, 1);
//					tlvEntity[currentTLVIndex].Tag[1] = 0;
//					tlvEntity[currentTLVIndex].TagSize = 1;
//					currentIndex += 1; 
//				}
//				//����SubTag
//				int subLength = 0; 
//				unsigned char* temp;
//				if((buffer[currentIndex] & 0x80) == 0x80)
//				{
//					for (int index = 0; index < 2; index++)
//					{
//						subLength += buffer[currentIndex + 1 + index] << (index * 8); //����Length��ĳ���
//					}
//					temp = new unsigned char[subLength];
//					memcpy(temp, buffer + currentIndex + 3, subLength);
//				}
//				else
//				{
//					subLength = buffer[currentIndex];
//					temp = new unsigned char[subLength];
//					memcpy(temp, buffer + currentIndex + 1, subLength);
//				}
//				temp[subLength] = 0;
//				//memcpy(temp, buffer + currentIndex + 1, subLength);
//				unsigned int oLength;
//				tlvEntity[currentTLVIndex].Sub_TLVEntity = new TLVEntity[1];
//				//Construct(temp, subLength, tlvEntity[currentTLVIndex].Sub_TLVEntity, oLength);
//			}
//			currentStatus = 'L';
//			break;
//		case 'L': 
//			//�жϳ����ֽڵ����λ�Ƿ�Ϊ1�����Ϊ1������ֽ�Ϊ������չ�ֽڣ�����һ���ֽڿ�ʼ��������
//			if((buffer[currentIndex] & 0x80) != 0x80)
//			{
//				tlvEntity[currentTLVIndex].Length = new unsigned char[1];
//				memcpy(tlvEntity[currentTLVIndex].Length, buffer + currentIndex, 1);
//				tlvEntity[currentTLVIndex].Length[1] = 0;
//				tlvEntity[currentTLVIndex].LengthSize = 1;
//				valueSize = tlvEntity[currentTLVIndex].Length[0];
//				currentIndex += 1;
//			}
//			else
//			{
//				//Ϊ1�����
//				unsigned int lengthSize = buffer[currentIndex] & 0x7f;
//				currentIndex += 1; //����һ���ֽڿ�ʼ��Length��
//				for (int index = 0; index < lengthSize; index++)
//				{
//					valueSize += buffer[currentIndex + index] << (index * 8); //����Length��ĳ���
//				}
//				tlvEntity[currentTLVIndex].Length = new unsigned char[lengthSize];
//				memcpy(tlvEntity[currentTLVIndex].Length, buffer + currentIndex, lengthSize);
//				tlvEntity[currentTLVIndex].Length[lengthSize] = 0;
//				tlvEntity[currentTLVIndex].LengthSize = lengthSize;
//				currentIndex += lengthSize;
//			}
//			currentStatus = 'V';
//			break;
//		case 'V':
//			tlvEntity[currentTLVIndex].Value = new unsigned char[valueSize];
//			memcpy(tlvEntity[currentTLVIndex].Value, buffer + currentIndex, valueSize);
//			tlvEntity[currentTLVIndex].Value[valueSize] = 0;
//			currentIndex += valueSize;
//			//������һ��TLV����ѭ��
//			currentTLVIndex += 1;
//			currentStatus = 'T';
//			break;
//		default:
//			return;
//		}
//	}
//	entityLength = currentTLVIndex;
//}
//
//
//
////// ����TLV
//void Parse(TLVEntity* tlvEntity, unsigned int entityLength, unsigned char* buffer, unsigned long & bufferLength)
//{
//int currentIndex = 0;
//int currentTLVIndex = 0;
//unsigned long valueSize = 0;
//
//	while(currentTLVIndex < entityLength)
//	{
//		valueSize = 0;
//		TLVEntity entity = tlvEntity[currentTLVIndex];
//		memcpy(buffer + currentIndex, entity.Tag, entity.TagSize); //����Tag
//		currentIndex += entity.TagSize;
//		for (int index = 0; index < entity.LengthSize; index++)
//		{
//			valueSize += entity.Length[index] << (index * 8); //����Length��ĳ���
//		}
//		if(valueSize > 127)
//		{
//			buffer[currentIndex] = 0x80 | entity.LengthSize;
//			currentIndex += 1;
//		}
//		memcpy(buffer + currentIndex, entity.Length, entity.LengthSize); //����Length
//		currentIndex += entity.LengthSize;
//		//�ж��Ƿ������Ƕ��TLV
//		if(entity.Sub_TLVEntity == NULL)
//		{
//			memcpy(buffer + currentIndex, entity.Value, valueSize); //����Value
//			currentIndex += valueSize;
//		}
//		else
//		{
//			unsigned long oLength;
//			Parse(entity.Sub_TLVEntity, 1, buffer + currentIndex, oLength); //������Ƕ��TLV
//			currentIndex += oLength;
//		}
//		currentTLVIndex++;
//	}
//	buffer[currentIndex] = 0;
//	bufferLength = currentIndex;
//}



void construct_tlv(unsigned char *in_buf, long in_len, struct TLVEntity *tlv, unsigned char value)
{
long curIndex, curTLVArray, curTagIndex;

	curIndex = curTLVArray = curTagIndex = 0;

	while(curIndex < in_len)
	{
		//
		//tlv->UpArray += 1;
		//TAG
		curTagIndex = curIndex;
		if((in_buf[curIndex] & 0x1F) == 0x1F)
		{//double bytes TAG
			tlv[curTLVArray].Tag = (in_buf[curIndex] << 8) + in_buf[curIndex + 1];
			curIndex += 1;
		}else
			tlv[curTLVArray].Tag = in_buf[curIndex];
		curIndex += 1;
		//LENGTH
		if((in_buf[curIndex] & 0x80) == 0x80)
		{//next bytes is the real length but one bytes enough for PBOC
			curIndex += 1;
			tlv[curTLVArray].Length = in_buf[curIndex];
		}else
			tlv[curTLVArray].Length = in_buf[curIndex];
		curIndex += 1;
		//VALUE
		tlv[curTLVArray].Value = &in_buf[curIndex];
		//MUST be the TAG
		if(((in_buf[curTagIndex] & 0x20) == 0x20) || (tlv[curTLVArray].Tag == 0x9f38))
		{//constructed data object
			if(tlv[curTLVArray].Sub_TLVEntity == NULL)
			{//
				tlv[curTLVArray].Sub_TLVEntity = (struct TLVEntity *)malloc(sizeof(struct TLVEntity) * 20);
				memset(tlv[curTLVArray].Sub_TLVEntity, 0x00, sizeof(struct TLVEntity) * 20);
			}
			else
			{
				//tlv[curTLVArray].Sub_TLVEntity = (struct TLVEntity *)realloc(tlv[curTLVArray].Sub_TLVEntity, sizeof(struct TLVEntity) * (tlv[0].UpArray + 1));
				//tlv[curTLVArray].Sub_TLVEntity[tlv[0].ArraySize].Sub_TLVEntity = NULL;
			}
			tlv[curTLVArray].Sub_TLVEntity[0].UpArray += 1;
			if(tlv[curTLVArray].Tag == 0x9f38)
				construct_tlv(tlv[curTLVArray].Value, tlv[curTLVArray].Length, tlv[curTLVArray].Sub_TLVEntity, 0);
			else
				construct_tlv(tlv[curTLVArray].Value, tlv[curTLVArray].Length, tlv[curTLVArray].Sub_TLVEntity, 1);
			tlv[curTLVArray].ArraySize = tlv[curTLVArray].Sub_TLVEntity[0].UpArray;

			curIndex += tlv[curTLVArray].Length;
			curTLVArray += 1;
			if(curIndex < in_len)
			{
				tlv[0].UpArray += 1;
				//tlv = (struct TLVEntity *)realloc(tlv, sizeof(struct TLVEntity) * (tlv[0].UpArray));
				//memset(&tlv[curTLVArray], 0x00, sizeof(struct TLVEntity));
			}
		}else
		{//primitive data object
			tlv[curTLVArray].Sub_TLVEntity = NULL;
			tlv[curTLVArray].ArraySize += 1;
			if(value)
				curIndex += tlv[curTLVArray].Length;
			curTagIndex = curIndex;
			curTLVArray += 1;
			//have multi TLV for the same level
			if(curIndex < in_len)
			{
				tlv[0].UpArray += 1;
				//tlv = (struct TLVEntity *)realloc(tlv, sizeof(struct TLVEntity) * (tlv[0].UpArray));
				//memset(&tlv[curTLVArray], 0x00, sizeof(struct TLVEntity));
			}
		}
	}
	return ;
}


struct TLVEntity * search_tlv(unsigned short *tag_buf, long tag_len, struct TLVEntity *tlv)
{
long curIndex, tagIndex;
TLVEntity_t *temptlv;

	if(tag_len <= 0)
		return NULL;
	tagIndex = 0;
	for(curIndex = 0; curIndex < tlv->UpArray; curIndex++)
	{
		//TAG
		if(tlv[curIndex].Tag == tag_buf[tagIndex])
		{
			tagIndex += 1;
			if(tagIndex == tag_len)
				return &tlv[curIndex];
			temptlv = search_tlv(&tag_buf[tagIndex], tag_len - tagIndex, tlv[curIndex].Sub_TLVEntity);
			return temptlv;
		}
	}
	return NULL;
}


char init_gpo(unsigned char *cur_timebcd, unsigned long tranamount, unsigned char cappflag, unsigned char *out_buf, struct TLVEntity *tlv)
{
unsigned char curIndex, tagIndex;

	//9f6604 9f0206 9f0306 9f1a02 9505 5f2a02 9a03 9c01 9f3704 df6001 
	tagIndex = 0;
	for(curIndex = 0; curIndex < tlv->ArraySize; curIndex++)
	{
		switch(tlv->Sub_TLVEntity[curIndex].Tag)
		{
		case 0x9f66:	//�ն˽�������
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], "\x28\x00\x00\x80", 4);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x9f02:	//��Ȩ���
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			out_buf[tagIndex + 1] = bin2bcd((tranamount / 100000000) % 100);
			out_buf[tagIndex + 2] = bin2bcd((tranamount / 1000000) % 100);
			out_buf[tagIndex + 3] = bin2bcd((tranamount / 10000) % 100);
			out_buf[tagIndex + 4] = bin2bcd((tranamount / 100) % 100);
			out_buf[tagIndex + 5] = bin2bcd((tranamount / 1) % 100);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x9f03:	//�������
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x9f1a:	//�ն˹��Ҵ���
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], "\x01\x56", 2);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x95:		//�ն���֤���
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], "\x00\x00\x00\x00\x00", 5);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x5f2a:	//���׻��Ҵ���
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], "\x01\x56", 2);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x9a:		//��������
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], &cur_timebcd[1], 3);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x9c:		//��������
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], "\x00", 1);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0x9f37:	//�ն������
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], "\x11\x22\x33\x44", 4);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		case 0xdf60:	//CAPP����ָʾλ
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			memcpy(&out_buf[tagIndex], &cappflag, 1);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		default:		//
			memset(&out_buf[tagIndex], 0x00, tlv->Sub_TLVEntity[curIndex].Length);
			tagIndex += tlv->Sub_TLVEntity[curIndex].Length;
			break;
		}
	}
	return tagIndex;
}

void free_tlv(struct TLVEntity *tlv)
{
long curIndex, tagIndex;

	tagIndex = 0;
	for(curIndex = 0; curIndex < tlv->ArraySize; curIndex++)
	{
		//TAG
		if(tlv->Sub_TLVEntity == NULL)
		{
			continue;
		}
		free_tlv(&(tlv->Sub_TLVEntity[curIndex]));
	}
	if(tlv->Sub_TLVEntity != NULL)
		free(tlv->Sub_TLVEntity);
	tlv->Sub_TLVEntity = NULL;
}
