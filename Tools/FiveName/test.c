/*输入出生年月日时，返回生辰八字五行*/
#include<stdio.h>
#include<string.h>
//#include<windows.h>
char *tiang[]={"癸","甲","乙","丙","丁","戊","己","庚","辛","壬"};
char *dizhi[]={"亥","子","丑","寅","卯","辰","巳","午","未","申","酉","戌"};
char *wuxing[]={"木","火","土","金","水"};
char *wxnayin[]={"海中金","炉中火","大林木","路旁土","剑锋金","山头火","涧下水","城头土","白蜡金","杨柳木","泉中水","屋上土","霹雳火","松柏木","长流水",
                 "沙中金","山下火","平地木","壁上土","金箔金","覆灯火","天河水","大驿土","钗钏金","桑柘木","大溪水","沙中土","天上火","石榴木","大海水"};

int ytian(int year)  /*计算年对应的天干*/
{ int i;
  i=year-3;
  return (i%10);
 }
int ydiz(int year)  /*计算年对应的地支*/
{
  int i;
  i=year-3;
  return (i%12);
 }
int mtian(int t1,int t2)  /*计算月对应的天干*/
{ int i;
  i=t1%5*2+1+t2-2;
  i=i%10;
  return (i);
 }
int dtian(int y,int m,int d)  /*计算日对应的天干*/
{ int g,c,j;
  if(m<=2)m+=12;
  c=y/100;
  y=y%100;
  g=4*c+c/4+5*y+y/4+3*(m+1)/5+d-3;
  j=g%10;
  return (j);
 }
int ddi(int y,int m,int d)  /*计算日对应的地支*/
{ int z,c,i,j;
  if(m<=2)m+=12;
  if(m%2)i=6;else i=0;
  c=y/100;
  y=y%100;
  z=8*c+c/4+5*y+y/4+3*(m+1)/5+d+7+i;
  j=z%12+6;
  if(j>7)j=j%12;
  return (j);
 }
int ttian(int t1,int t2)  /*计算时对应的天干*/
{ int i;
//  if(t1%5==0)i=9;
 // else
  i=(t1*2-1)+(t2/100+1)/2;
  i=i%10;
  return (i);
 }
int ttd(int t)
{ int tem;
  t=t/100;
  if(t>=1&&t<23)tem=(t+1)/2+1;else tem==1;
  return (tem%12);
}
int tgwx(int n)/*天干对应五行*/
{
	return ((n+1)/2-1);
}
int dzwx(int n)/*地支对应五行*/
{
	if(n==3||n==4)return 0;
	else if(n==6||n==7)return 1;
	else if(n==9||n==10)return 3;
	else if(n==0||n==1)return 4;
	else return 2;
}
int pr1(int n)/*天干输出*/
{
	printf("%s",*(tiang+n));
}
int pr2(int n)/*地支输出*/
{
	printf("%s",*(dizhi+n));
}
int pr3(int n)/*五行输出*/
{
	printf("%s",*(wuxing+n));
}
/*void meanu()
{
	system("title 八字五行查询  Powered by ： RedMaple");
	system("mode con cols=50 lines=30");
	system("color 74") ;
}*/
int main()
{ 
  //system("cls");
  //meanu();
  int year,month,date,time;
  printf("\n————欢迎使用 生辰八字五行查询 小程序————\n\n\t");
  printf("请输入阳历出生年、月、日、时：\n\n\t");
  printf("举例：1980 9 25 1702\n\n");
  printf("\t*********************************\n\t");
  scanf("%d%d%d%d",&year,&month,&date,&time);
  if(month>12||month<1||date<1||date>31||time>2400||year<1924)
  {printf("\t您的输入错误，重新进入程序\n\n");main();}
  else 
  {
  int tg1,tg2,tg3,tg4,dz1,dz2,dz3,dz4;
  tg1=ytian(year);
  dz1=ydiz(year); 
  tg2=mtian(tg1,month);
  if(month+1>=11)dz2=month-11;else dz2=month+1;
  tg3=dtian(year,month,date);
  dz3=ddi(year,month,date);
  tg4=ttian(tg3,time);
  dz4=ttd(time);
  printf("\n\t*********************************\n");
  printf("\n\t您查询生辰的八字五行如下：\n\n");
  printf("\t 年\t 月\t 日\t 时\n\n");
  printf("\t");
  pr1(tg1);pr2(dz1);printf("\t");
  pr1(tg2);pr2(dz2);printf("\t");
  pr1(tg3);pr2(dz3);printf("\t");
  pr1(tg4);pr2(dz4);printf("\n\n");
  printf("\t");
  pr3(tgwx(tg1));pr3(dzwx(dz1));printf("\t");
  pr3(tgwx(tg2));pr3(dzwx(dz2));printf("\t");
  pr3(tgwx(tg3));pr3(dzwx(dz3));printf("\t");
  pr3(tgwx(tg4));pr3(dzwx(dz4));printf("\n\n");
  printf("\n\t五行纳音为：");
  printf("\%s\n\n",*(wxnayin+(((year-1924)%60)/2)));
  printf("\t*********************************\n");
  printf("\t【0】退出   【】任意键继续查询\n\n\t");
  getchar();
  if(getchar()=='0')
   {
		//system("cls");
	    printf("\n\n————感谢使用 生辰八字五行查询 小程序————\n\n");
	  	printf("\t\t\t再见！\n");
		return 0; 
	}
  else  {printf("\n\n");main(); }
   }
  
}
