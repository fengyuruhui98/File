#ifndef SM_C
#define SM_C


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ABORT do { 	fflush(stdout); 	fprintf(stderr, "%s:%d: ABORT\n", __FILE__, __LINE__); 	ERR_print_errors_fp(stderr); 	exit(1); } while (0)

unsigned char sm3_a[32];
unsigned char sm3_b[32];
unsigned char sm3_Gx[32];
unsigned char sm3_Gy[32];
unsigned char sm3_hash[32];

//国密标准推荐参数
//p:FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
//a:FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
//b:28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
//Gx:32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
//Gy:BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0
//n:FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123

//ECC推荐参数：256k1
//p=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
//a=0000000000000000000000000000000000000000000000000000000000000000
//b=0000000000000000000000000000000000000000000000000000000000000007
//Gx=79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
//Gy=483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
//n=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141

//ECC推荐参数：256r1
//p=FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF
//a=FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC
//b=5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B
//Gx=6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
//Gy=4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5
//n=FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551

//ECC 160R1
//p=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF
//a=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC
//b=1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45
//Gx=4A96B5688EF573284664698968C38BB913CBFC82
//Gy=23A628553168947D59DCC912042351377AC5FB32
//n=0100000000000000000001F4C8F927AED3CA752257

//ECC 192R1
//p=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF
//a=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC
//b=64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1
//Gx=188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012
//Gy=07192B95FFC8DA78631011ED6B24CDD573F977A11E794811
//n=FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831

//ECC 224R1
//p=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001
//a=FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE
//b=B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4
//Gx=B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21
//Gy=BD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34
//n=FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D

int SM2_Operation(const unsigned char *id, unsigned char *id_len, unsigned char y_bit, unsigned char *Xa, unsigned char *Ya, 
				  unsigned char *message, unsigned int msgLen, unsigned char *r, unsigned char *s)
{
#ifdef DEBUG_QR		//为了兼容老读写器，老读写器暂不支持sm2算法

BN_CTX *ctx = NULL;
BIGNUM *p, *a, *b;
EC_GROUP *group;
EC_POINT *P, *Q, *R;
BIGNUM *x, *y, *z;
EC_KEY	*eckey = NULL;
const unsigned char 	*pSignature;
unsigned char	digest[32] = "\xB5\x24\xF5\x52\xCD\x82\xB8\xB0\x28\x47\x6E\x00\x5C\x37\x7F\xB1\x9A\x87\xE6\xFC\x68\x2D\x48\xBB\x5D\x42\xE3\xD9\xB9\xEF\xFE\x76"; 
int	sig_len;
BIGNUM *kinv, *rp,*order; 

unsigned char sm3_buf[2048], rs[100], pubkeyX[200], pubkeyY[200];
int sm3_buf_len;
int i;
unsigned short sh_id_len;
unsigned char Ya_bak[32];

	CRYPTO_set_mem_debug_functions(0, 0, 0, 0, 0);
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
	ERR_load_crypto_strings();
	//RAND_seed(rnd_seed, sizeof rnd_seed); /* or BN_generate_prime may fail */
	
	ctx = BN_CTX_new();
	//if (!ctx) ABORT;
	if(!ctx)
		return 10;

	/* Curve SM2 (Chinese National Algorithm) */
	//http://www.oscca.gov.cn/News/201012/News_1197.htm
	p = BN_new();
	a = BN_new();
	b = BN_new();
	//if (!p || !a || !b) ABORT;
	if (!p || !a || !b) return 11;
	group = EC_GROUP_new(EC_GFp_mont_method()); /* applications should use EC_GROUP_new_curve_GFp
 	                                             * so that the library gets to choose the EC_METHOD */
	//if (!group) ABORT;
	if (!group) return 12;
	//固定参数值：p/a/b
	//if (!BN_hex2bn(&p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF")) ABORT;
	if (!BN_hex2bn(&p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF")) return 13;

	if (1 != BN_is_prime_ex(p, BN_prime_checks, ctx, NULL)) ABORT;
	if (!BN_hex2bn(&a, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC")) return 14;
	BN_bn2bin(a, sm3_a);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_a", sm3_a, 32);
#endif	

	if (!BN_hex2bn(&b, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93")) return 15;
	BN_bn2bin(b, sm3_b);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_b", sm3_b, 32);
#endif
	
	if (!EC_GROUP_set_curve_GFp(group, p, a, b, ctx)) return 16;

	P = EC_POINT_new(group);
	if (!P) return 17;

	x = BN_new();
	y = BN_new();
	z = BN_new();
	if (!x || !y || !z) return 18;
	//固定参数值：x/z(n)
	if (!BN_hex2bn(&x, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7")) return 19;
	BN_bn2bin(x, sm3_Gx);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_Gx", sm3_Gx, 32);
#endif	
	//素数域椭圆曲线，给定压缩坐标x和y_bit参数，设置point的几何坐标；用于将Octet-String转化为椭圆曲线上的点；
	if (!EC_POINT_set_compressed_coordinates_GFp(group, P, x, 0, ctx)) return 20;
	//判断一个点point是否在椭圆曲线上
	if (!EC_POINT_is_on_curve(group, P, ctx)) return 21;
	if (!BN_hex2bn(&z, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123")) return 22;
	//设置椭圆曲线的基G；generator、order和cofactor为输入参数；
	if (!EC_GROUP_set_generator(group, P, z, BN_value_one())) return 23;
		
	//计算y（若提供y，则比较y值）
	//获取素数域上椭圆曲线上某个点的x和y的几何坐标；
	if (!EC_POINT_get_affine_coordinates_GFp(group, P, x, y, ctx)) return 24;
	//if (!BN_hex2bn(&z, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0")) ABORT;
	BN_bn2bin(y, sm3_Gy);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_Gy", sm3_Gy, 32);
#endif
	//if (0 != BN_cmp(y, z)) ABORT;
	
	//计算Za=SM3{ENTLa || ID || a || b || Gx || Gy || Xa || Ya]
	sm3_buf_len = 0;
	//ENTLA
	memcpy(&sm3_buf[sm3_buf_len], id_len, 2);
	sm3_buf_len += 2;
	//ID
	sh_id_len = ((id_len[0] << 8) + id_len[1]) / 8;
	memcpy(&sm3_buf[sm3_buf_len], id, sh_id_len);//strlen(id));
	sm3_buf_len += sh_id_len;	//strlen(id);
	//a
	memcpy(&sm3_buf[sm3_buf_len], sm3_a, 32);
	sm3_buf_len += 32;
	//b 
	memcpy(&sm3_buf[sm3_buf_len], sm3_b, 32);
	sm3_buf_len += 32;
	//Gx
	memcpy(&sm3_buf[sm3_buf_len], sm3_Gx, 32);
	sm3_buf_len += 32;
	//Gy
	memcpy(&sm3_buf[sm3_buf_len], sm3_Gy, 32);
	sm3_buf_len += 32;
	//Xa
	//memcpy(&sm3_buf[sm3_buf_len], "\xD5\x54\x8C\x78\x25\xCB\xB5\x61\x50\xA3\x50\x6C\xD5\x74\x64\xAF\x8A\x1A\xE0\x51\x9D\xFA\xF3\xC5\x82\x21\xDC\x81\x0C\xAF\x28\xDD\x92\x10\x73\x76\x8F\xE3\xD5\x9C\xE5\x4E\x79\xA4\x94\x45\xCF\x73\xFE\xD2\x30\x86\x53\x70\x27\x26\x4D\x16\x89\x46\xD4\x79\x53\x3E", 64);
	memcpy(&sm3_buf[sm3_buf_len], Xa, 32);
	sm3_buf_len += 32;
	//Ya
	if( Ya == NULL)
	{
		//计算y（若提供y，则比较y值）
		//获取素数域上椭圆曲线上某个点的x和y的几何坐标；
		//BN_bin2bn(Xa, 32, x);
		//if (!EC_POINT_get_affine_coordinates_GFp(group, P, x, y, ctx)) return 24;
		//BN_bn2bin(y, Ya_bak);//&sm3_buf[sm3_buf_len]);
		//extendsPrintf("Cal Ya 1 ", Ya_bak, 32);
		SM2_Cal_Ya(y_bit, Xa, Ya_bak);
		extendsPrintf("Cal Ya 2", Ya_bak, 32);
	}else
	{
		memcpy(Ya_bak, Ya, 32);
	}
	memcpy(&sm3_buf[sm3_buf_len], Ya_bak, 32);
	sm3_buf_len += 32;
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3 message", sm3_buf, sm3_buf_len);
#endif
	//Za
	sm3(sm3_buf, sm3_buf_len, sm3_hash);
#ifdef	DEBUG_PRINT	
	extendsPrintf("Za", sm3_hash, 32);
#endif	
	//计算报文MSG的HASH值：SM3[Za || MSG]
	sm3_buf_len= 0;
	memcpy(&sm3_buf[sm3_buf_len], sm3_hash, 32);
	sm3_buf_len += 32;
	
	//memcpy(&sm3_buf[sm3_buf_len], "\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x01\x01", 22);
	memcpy(&sm3_buf[sm3_buf_len], message, msgLen);
	sm3_buf_len += msgLen;
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3 message", sm3_buf, sm3_buf_len);
#endif	
	//
	sm3(sm3_buf, sm3_buf_len, sm3_hash);
#ifdef	DEBUG_PRINT
	extendsPrintf("Digist", sm3_hash, 32);
#endif
	//
	if ((eckey = EC_KEY_new()) == NULL)
		return 25;
	if (EC_KEY_set_group(eckey, group) == 0)
	{
#ifdef	DEBUG_PRINT	
		fprintf(stdout," failed\n");
#endif
		return 26;
	}
	//
	for(i = 0; i < 32; i++)
		sprintf(&pubkeyX[i * 2], "%02X", Xa[i]);
	for (i = 0; i < 32; i++)
		sprintf(&pubkeyY[i * 2], "%02X", Ya_bak[i]);
	//if (!BN_hex2bn(&x, "D5548C7825CBB56150A3506CD57464AF8A1AE0519DFAF3C58221DC810CAF28DD")) ABORT;
	//if (!BN_hex2bn(&y, "921073768FE3D59CE54E79A49445CF73FED23086537027264D168946D479533E")) ABORT;
#ifdef	DEBUG_PRINT	
	printf("Xa: %s\n", pubkeyX);
	printf("Ya: %s\n", pubkeyY);
#endif	
	if (!BN_hex2bn(&x, pubkeyX)) return 27;
	if (!BN_hex2bn(&y, pubkeyY)) return 28;
	
	if (!EC_POINT_set_affine_coordinates_GFp(group,P, x, y, ctx)) return 29;
	//EC_KEY_set_public_key(eckey, P);
	EC_KEY_set_public_key_affine_coordinates(eckey, x, y);
	
	//
	memcpy(rs, "\x30\x44\x02\x20", 4);
	//memcpy(&rs[4], "\x8E\xED\x8C\xE5\x3A\x85\x26\x48\xC0\x8F\xA9\xB3\x48\x37\x12\x35\x45\x54\x2E\xE4\x82\xA2\x95\x16\x2B\xB6\x85\xA4\x4A\x23\xED\xCD", 32);
	memcpy(&rs[4], r, 32);
	//memcpy(&rs[4], "\x7E\x31\xC0\xCA\x34\xF8\xF1\x99\x32\xF0\x68\xF3\xD2\xFB\x55\x65\x4E\xE5\x1C\xF4\x59\x2E\x30\xA9\xEA\x64\x93\xFB\x4D\x32\x3F\x36", 32);
	memcpy(&rs[4 + 32], "\x02\x20" ,2);
	memcpy(&rs[4 + 32 + 2], s, 32);
	//memcpy(&rs[4 + 32 + 2], "\xC5\xEC\x31\x41\x2A\x77\x87\xFD\xCB\x2C\x5E\xD8\x74\x7A\xEF\xFF\xD5\xE1\x4D\xB0\x8E\x2C\x11\x43\x5D\x45\xB2\xC2\xC3\x80\x01\x7A", 32);
	//memcpy(&rs[4 + 32 + 2], "\xAC\x81\x7A\x5F\x81\xD2\x0C\x88\xE5\xE2\x9F\x3C\xA7\xC5\x39\x6F\x2E\x78\x86\x7F\xC5\xB1\x04\x64\x06\x02\x9D\xF1\xBD\x2B\x0D\xE9", 32);
	
//	sig = ECDSA_SIG_new();
//  if (!ECDSA_SIG_set0(sig, r, s)) {
//       fprintf(stderr, "error : %s %d\n", __FUNCTION__, __LINE__);
//       goto end;
//  }
//  pp_len = i2d_ECDSA_SIG(sig, &pp);	
	
#ifdef	DEBUG_PRINT	
	extendsPrintf("rs", rs, 70);
#endif
	if( 1 != (i = SM2_verify(1, sm3_hash, 32, rs, 70, eckey)) )
	{
#ifdef	DEBUG_PRINT	
		printf("SM2 Verify failed return %d\n", i);
#endif
		EC_POINT_free(P);
		EC_KEY_free(eckey);
		EC_GROUP_free(group);
		BN_CTX_free(ctx);
		return 1;
	}
#ifdef	DEBUG_PRINT	
	printf("SM2 Verify OK\n");
#endif
	EC_POINT_free(P);
	EC_KEY_free(eckey);
	EC_GROUP_free(group);
	BN_CTX_free(ctx);
#endif	
	return 0;
}


//int main(int argc, char *argv[])
//{
//unsigned char Xa[32], Ya[32];
//unsigned char msg[32];
//unsigned char pubkey[64];
//unsigned char r[32], s[32];
//int i;
//
//	memcpy(Xa, "\xD5\x54\x8C\x78\x25\xCB\xB5\x61\x50\xA3\x50\x6C\xD5\x74\x64\xAF\x8A\x1A\xE0\x51\x9D\xFA\xF3\xC5\x82\x21\xDC\x81\x0C\xAF\x28\xDD", 32);
//	memcpy(Ya, "\x92\x10\x73\x76\x8F\xE3\xD5\x9C\xE5\x4E\x79\xA4\x94\x45\xCF\x73\xFE\xD2\x30\x86\x53\x70\x27\x26\x4D\x16\x89\x46\xD4\x79\x53\x3E", 32);
//	memcpy(msg, "\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x01\x01", 22);
//	
//	memcpy(pubkey, "\xD5\x54\x8C\x78\x25\xCB\xB5\x61\x50\xA3\x50\x6C\xD5\x74\x64\xAF\x8A\x1A\xE0\x51\x9D\xFA\xF3\xC5\x82\x21\xDC\x81\x0C\xAF\x28\xDD\x92\x10\x73\x76\x8F\xE3\xD5\x9C\xE5\x4E\x79\xA4\x94\x45\xCF\x73\xFE\xD2\x30\x86\x53\x70\x27\x26\x4D\x16\x89\x46\xD4\x79\x53\x3E", 64);
//	
//	memcpy(r, "\x8E\xED\x8C\xE5\x3A\x85\x26\x48\xC0\x8F\xA9\xB3\x48\x37\x12\x35\x45\x54\x2E\xE4\x82\xA2\x95\x16\x2B\xB6\x85\xA4\x4A\x23\xED\xCD", 32);
//	memcpy(s, "\xC5\xEC\x31\x41\x2A\x77\x87\xFD\xCB\x2C\x5E\xD8\x74\x7A\xEF\xFF\xD5\xE1\x4D\xB0\x8E\x2C\x11\x43\x5D\x45\xB2\xC2\xC3\x80\x01\x7A", 32);
//
//	for(i = 0; i < 0xFFFFFF; i++)
//	{
//		SM2_Operation(iACC_id, iACC_id_len, Xa, Ya, msg, 22, r, s);
//		printf("finished %d times\n\n", i);
//	}
//	
//	return 0;
//}


int SM2_Cal_Ya(int y_bit, unsigned char *Xa, unsigned char *Ya)
{
#ifdef DEBUG_QR		//为了兼容老读写器，老读写器暂不支持sm2算法

BN_CTX *ctx = NULL;
BIGNUM *p, *a, *b;
EC_GROUP *group;
EC_POINT *P, *Q, *R;
BIGNUM *x, *y, *z;
EC_KEY	*eckey = NULL;
const unsigned char 	*pSignature;
unsigned char	digest[32] = "\xB5\x24\xF5\x52\xCD\x82\xB8\xB0\x28\x47\x6E\x00\x5C\x37\x7F\xB1\x9A\x87\xE6\xFC\x68\x2D\x48\xBB\x5D\x42\xE3\xD9\xB9\xEF\xFE\x76"; 
int	sig_len;
BIGNUM *kinv, *rp,*order; 

unsigned char sm3_buf[2048], rs[100], pubkeyX[200], pubkeyY[200];
int sm3_buf_len;
int i;

	CRYPTO_set_mem_debug_functions(0, 0, 0, 0, 0);
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
	ERR_load_crypto_strings();
	//RAND_seed(rnd_seed, sizeof rnd_seed); /* or BN_generate_prime may fail */
	
	ctx = BN_CTX_new();
	//if (!ctx) ABORT;
	if(!ctx)
		return 10;

	/* Curve SM2 (Chinese National Algorithm) */
	//http://www.oscca.gov.cn/News/201012/News_1197.htm
	p = BN_new();
	a = BN_new();
	b = BN_new();
	//if (!p || !a || !b) ABORT;
	if (!p || !a || !b) return 11;
	group = EC_GROUP_new(EC_GFp_mont_method()); /* applications should use EC_GROUP_new_curve_GFp
 	                                             * so that the library gets to choose the EC_METHOD */
	//if (!group) ABORT;
	if (!group) return 12;
	//固定参数值：p/a/b
	//if (!BN_hex2bn(&p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF")) ABORT;
	if (!BN_hex2bn(&p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF")) return 13;

	if (1 != BN_is_prime_ex(p, BN_prime_checks, ctx, NULL)) ABORT;
	if (!BN_hex2bn(&a, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC")) return 14;
	BN_bn2bin(a, sm3_a);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_a", sm3_a, 32);
#endif	

	if (!BN_hex2bn(&b, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93")) return 15;
	BN_bn2bin(b, sm3_b);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_b", sm3_b, 32);
#endif
	
	if (!EC_GROUP_set_curve_GFp(group, p, a, b, ctx)) return 16;

	P = EC_POINT_new(group);
	if (!P) return 17;

	x = BN_new();
	y = BN_new();
	z = BN_new();
	if (!x || !y || !z) return 18;
	//固定参数值：x/z(n)
	if (!BN_hex2bn(&x, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7")) return 19;
	BN_bn2bin(x, sm3_Gx);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_Gx", sm3_Gx, 32);
#endif	
	//素数域椭圆曲线，给定压缩坐标x和y_bit参数，设置point的几何坐标；用于将Octet-String转化为椭圆曲线上的点；
	if (!EC_POINT_set_compressed_coordinates_GFp(group, P, x, 0, ctx)) return 20;
	//判断一个点point是否在椭圆曲线上
	if (!EC_POINT_is_on_curve(group, P, ctx)) return 21;
	if (!BN_hex2bn(&z, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123")) return 22;
	//设置椭圆曲线的基G；generator、order和cofactor为输入参数；
	if (!EC_GROUP_set_generator(group, P, z, BN_value_one())) return 23;
		
	//计算y（若提供y，则比较y值）
	//获取素数域上椭圆曲线上某个点的x和y的几何坐标；
	if (!EC_POINT_get_affine_coordinates_GFp(group, P, x, y, ctx)) return 24;
	//if (!BN_hex2bn(&z, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0")) ABORT;
	BN_bn2bin(y, sm3_Gy);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_Gy", sm3_Gy, 32);
#endif

	//
	for(i = 0; i < 32; i++)
		sprintf(&pubkeyX[i * 2], "%02X", Xa[i]);
	for (i = 0; i < 32; i++)
		sprintf(&pubkeyY[i * 2], "%02X", Ya[i]);
	//if (!BN_hex2bn(&x, "D5548C7825CBB56150A3506CD57464AF8A1AE0519DFAF3C58221DC810CAF28DD")) ABORT;
	//if (!BN_hex2bn(&y, "921073768FE3D59CE54E79A49445CF73FED23086537027264D168946D479533E")) ABORT;
#ifdef	DEBUG_PRINT	
	printf("Xa: %s\n", pubkeyX);
	printf("Ya: %s\n", pubkeyY);
#endif	
	if (!BN_hex2bn(&x, pubkeyX)) return 27;
	//if (!BN_hex2bn(&y, pubkeyY)) return 28;
	
	if (!EC_GROUP_set_curve_GFp(group, p, a, b, ctx)) return 31;
	Q = EC_POINT_new(group);
	if(!Q) return 30;

	if( (y_bit % 2) == 0)
	{//公钥仅提供Xa坐标，须根据压缩形式确定计算Ya的方法
		if( !EC_POINT_set_compressed_coordinates_GFp(group, Q, x, 0, ctx)) return 32;
		//if (!EC_POINT_set_affine_coordinates_GFp(group,P, x, y, ctx)) return 29;
		if (!EC_POINT_get_affine_coordinates_GFp(group, Q, x, y, ctx)) return 33;
		BN_bn2bin(y, sm3_Gy);
#ifdef	DEBUG_PRINT	
		extendsPrintf("sm3_Gy(0)", sm3_Gy, 32);
#endif
	}else
	{
		if( !EC_POINT_set_compressed_coordinates_GFp(group, Q, x, 1, ctx)) return 32;
		//if (!EC_POINT_set_affine_coordinates_GFp(group,P, x, y, ctx)) return 29;
		if (!EC_POINT_get_affine_coordinates_GFp(group, Q, x, y, ctx)) return 33;
		BN_bn2bin(y, sm3_Gy);
	}
	memcpy(Ya, sm3_Gy, 32);
#ifdef	DEBUG_PRINT	
	extendsPrintf("sm3_Gy(1)", sm3_Gy, 32);
#endif
	EC_POINT_free(P);
	EC_POINT_free(Q);
	EC_KEY_free(eckey);
	EC_GROUP_free(group);
	BN_CTX_free(ctx);
#endif	
	return 0;
}

void extendsPrintf(unsigned char *msg, unsigned char *inbuf, int in_len)
{
int i;

	printf("%s : ", msg);
	for(i = 0; i < in_len; i++)
		printf("%02X", inbuf[i]);	
	printf("\n");
}

#endif