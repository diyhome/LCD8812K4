 /*********************************************************
 * 本驱动修改成驱动：LCD8812K4 蓝屏 液晶显示屏 液晶屏 工业级 
 *
 * 地址：https://item.taobao.com/item.htm?spm=a1z09.2.0.0.zWdSkD&id=17760386568&_u=p2f9tros8e02
 * 	 http://www.ndiy.cn/thread-30754-1-1.html
 * 上半部分为段划部分，主控是HT1621：
 * 右下角小数字用“innumber”函数写入，两个8字地址依次为2,4
 * 大数字段划部分用“bignum”函数写入，
 * 数字所处的地址依次为：2,4,5,7,10,13,16,18,21,23,25,28
 * 其余为符号地址
 *
 * 下半部分类似于1601：
 *
********************************************************/

// - - 定义函数 

#define uchar unsigned char
#define uint unsigned int

#define BIAS 0x52 					//0b1000 0101 0010 1/3duty 4com
#define SYSDIS 0X00 					//0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN 0X02 					//0b1000 0000 0010 打开系统振荡器
#define LCDOFF 0X04 					//0b1000 0000 0100 关LCD偏压
#define LCDON 0X06 					//0b1000 0000 0110 打开LCD偏压
#define XTAL 0x28 					//0b1000 0010 1000 外部接时钟
#define RC256 0X30 					//0b1000 0011 0000 内部时钟
#define WDTDIS 0X0A 					//0b1000 0000 1010 禁止看门狗

// - - 定义接口 段划
// - - HT1621控制位（液晶模块接口定义，根据自已的需要更改）
sbit HT1621_CS=P0^4; 					// - - HT1621使能引脚
sbit HT1621_WR=P0^5; 					// - - HT1621时钟引脚
sbit HT1621_DAT=P0^6; 					// - - HT1621数据引脚

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - 初始化接口 lcd
# define LCD_DB P2 						// - - P2 = DB0～DB7
sbit LCD_RS=P0^0;						// - - p0.0 = RS
sbit LCD_RW=P0^1; 						// - - p0.1 = RW
sbit LCD_E=P0^2; 						// - - p0.2 = E
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - 定义变量,数组
uchar code Ht1621Tab[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00};
uchar code numbb[]={5,7,10,13,16,18,21,23,25,28};
uchar code litnum[]={2,4};

void Ht1621_Init(void); 				// - - 初始化 HT1621
void Ht1621Wr_Data(uchar Data,uchar cnt); 		// - - HT1621 写入数据函数
void Ht1621WrCmd(uchar Cmd);			 	// - - HT1621 写入命令函数
void Ht1621WrOneData(uchar Addr,uchar Data); 		// - - HT1621 在指定地址写入数据函数
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt); 	// - - HT1621 连续写入数据函数
void innumber(uchar addres,uchar num);			// - - 右下角小数字写入，地址为2,4
void bignum(uchar addres,uchar num);			// - - 大数字写入
void teshu(uchar dat);

void LCD_init(void); 					// - - 初始化LCD1602函数
void LCD_write_H4bit_command(uchar dat);
void LCD_write_L4bit_command(uchar dat);
void LCD_write_4bit_command(uchar command); 		// - - 向LCD1602写指令函数
void LCD_write_4bit_data(uchar dat);		 	// - - 向LCD1602写数据函数
void LCD_set_xy(uchar x,uchar y); 				// - - 设置LCD1602显示位置 X（0-16),y(1-2)
void LCD_disp_char(uchar x,uchar y,uchar dat); 		// - - 在LCD1602上显示一个字符
void LCD_disp_string(uchar X,uchar Y,uchar *s); 		// - - 在LCD1602上显示一个字符串

void delayms(uint ms);
void delayus_10(uint n); 

/********************************************************
函数名称：void Ht1621_Init(void)
功能描述: HT1621初始化
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
说 明：初始化后，液晶屏所有字段均显示
********************************************************/
void Ht1621_Init(void)
{
	HT1621_CS=1;
	HT1621_WR=1;
	HT1621_DAT=1;
	delayms(500); 				// - - 延时使LCD工作电压稳定
	Ht1621WrCmd(BIAS);
	Ht1621WrCmd(RC256); 				// - - 使用内部振荡器
	Ht1621WrCmd(SYSDIS); 				// - - 关振系统荡器和LCD偏压发生器
	Ht1621WrCmd(WDTDIS); 				// - - 禁止看门狗
	Ht1621WrCmd(SYSEN); 				// - - 打开系统振荡器
	Ht1621WrCmd(LCDON); 				// - - 打开声音输出
}

/******************************************************
函数名称：void Ht1621Wr_Data(uchar Data,uchar cnt)
功能描述: HT1621数据写入函数
全局变量：无
参数说明：Data为数据，cnt为数据位数
返回说明：无
说 明：写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
	uchar i;
	for (i=0;i<cnt;i++)
	{
		HT1621_WR=0;
		delayus_10(1);
		HT1621_DAT=Data&0x80;
		delayus_10(1);
		HT1621_WR=1;
		delayus_10(1);
		Data<<=1;
	}
}

/********************************************************
函数名称：void Ht1621WrCmd(uchar Cmd)
功能描述: HT1621命令写入函数
全局变量：无
参数说明：Cmd为写入命令数据
返回说明：无
说 明：写入命令标识位100
********************************************************/
void Ht1621WrCmd(uchar Cmd)
{
	HT1621_CS=0;
	delayus_10(1);
	Ht1621Wr_Data(0x80,4); 				// - - 写入命令标志100
	Ht1621Wr_Data(Cmd,8); 				// - - 写入命令数据
	HT1621_CS=1;
	delayus_10(1);
}

/********************************************************
函数名称：void Ht1621WrOneData(uchar Addr,uchar Data)
功能描述: HT1621在指定地址写入数据函数
全局变量：无
参数说明：Addr为写入初始地址，Data为写入数据
返回说明：无
说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
********************************************************/
void Ht1621WrOneData(uchar Addr,uchar Data)
{
	HT1621_CS=0;
	Ht1621Wr_Data(0xa0,3); 					// - - 写入数据标志101
	Ht1621Wr_Data(Addr<<2,6); 				// - - 写入地址数据
	Ht1621Wr_Data(Data<<4,4); 				// - - 写入数据
	HT1621_CS=1;
	delayus_10(1);
}

/********************************************************
函数名称：void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
功能描述: HT1621连续写入方式函数
全局变量：无
参数说明：Addr为写入初始地址，*p为连续写入数据指针，
cnt为写入数据总数
返回说明：无
说 明：HT1621的数据位4位，此处每次数据为8位，写入数据
总数按8位计算
********************************************************/
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
{
	uchar i;
	HT1621_CS=0;
	Ht1621Wr_Data(0xa0,3); 					// - - 写入数据标志101
	Ht1621Wr_Data(Addr<<2,6); 				// - - 写入地址数据
	for (i=0;i<cnt;i++)
	{
		Ht1621Wr_Data(*p,8);			 	// - - 写入数据
		p++;
	}
	HT1621_CS=1;
	delayus_10(1);
}

void innumber(uchar addres,uchar num)
{
	switch (num) {
	    case 0:
	      Ht1621WrOneData(litnum[addres]-1,0x05);
	      Ht1621WrOneData(litnum[addres],0x0f);
	      break;
	    case 1:
	      Ht1621WrOneData(litnum[addres]-1,0x05);
	      break;
	    case 2:
	      Ht1621WrOneData(litnum[addres]-1,0x03);
	      Ht1621WrOneData(litnum[addres],0x0d);
	      break;
	    case 3:
	      Ht1621WrOneData(litnum[addres]-1,0x02);
	      Ht1621WrOneData(litnum[addres],0x0f);
	      break;
	    case 4:
	      Ht1621WrOneData(litnum[addres]-1,0x06);
	      Ht1621WrOneData(litnum[addres],0x06);
	      break;
	    case 5:
	      Ht1621WrOneData(litnum[addres]-1,0x06);
	      Ht1621WrOneData(litnum[addres],0x0b);
	      break;
     	    case 6:
	      Ht1621WrOneData(litnum[addres]-1,0x07);
	      Ht1621WrOneData(litnum[addres],0x0b);
	      break;
	     case 7:
	      Ht1621WrOneData(litnum[addres],0x0e);
	      break;
	    case 8:
	      Ht1621WrOneData(litnum[addres]-1,0x07);
	      Ht1621WrOneData(litnum[addres],0x0f);
	      break;
	    case 9:
	      Ht1621WrOneData(litnum[addres]-1,0x06);
	      Ht1621WrOneData(litnum[addres],0x0f);
	      break;
	}
}

void bignum(uchar addres,uchar num)
{
	switch (num) {
	    case 0:
	      Ht1621WrOneData(numbb[addres]+1,0x05);
	      Ht1621WrOneData(numbb[addres],0x0f);
	      break;
	    case 1:
	      Ht1621WrOneData(numbb[addres]+1,0x05);
	      break;
	    case 2:
	      Ht1621WrOneData(numbb[addres]+1,0x03);
	      Ht1621WrOneData(numbb[addres],0x0d);
	      break;
	    case 3:
	      Ht1621WrOneData(numbb[addres]+1,0x02);
	      Ht1621WrOneData(numbb[addres],0x0f);
	      break;
	    case 4:
	      Ht1621WrOneData(numbb[addres]+1,0x06);
	      Ht1621WrOneData(numbb[addres],0x06);
	      break;
	    case 5:
	      Ht1621WrOneData(numbb[addres]+1,0x06);
	      Ht1621WrOneData(numbb[addres],0x0b);
	      break;
     	    case 6:
	      Ht1621WrOneData(numbb[addres]+1,0x07);
	      Ht1621WrOneData(numbb[addres],0x0b);
	      break;
	     case 7:
	      Ht1621WrOneData(numbb[addres],0x0e);
	      break;
	    case 8:
	      Ht1621WrOneData(numbb[addres]+1,0x07);
	      Ht1621WrOneData(numbb[addres],0x0f);
	      break;
	    case 9:
	      Ht1621WrOneData(numbb[addres]+1,0x06);
	      Ht1621WrOneData(numbb[addres],0x0f);
	      break;
	      }
}

void teshu(uchar dat)
{
	switch (dat){
		case ',':
			Ht1621WrOneData(3,0x08);
			break;
		case '%':
			Ht1621WrOneData(14,0x08);
			break;
		case 'F':
			Ht1621WrOneData(6,0x08);
			break;
		case 'S':
			Ht1621WrOneData(11,0x08);
			break;
		case 'M':
			Ht1621WrOneData(15,0x02);
			break;
		case '1':
			Ht1621WrOneData(8,0x08);
			break;
		case '2':
			Ht1621WrOneData(15,0x01);
			break;
		case '+':
			Ht1621WrOneData(0,0x0e);
			break;
		case '3':
			Ht1621WrOneData(0,0x01);
			Ht1621WrOneData(1,0x08);
			break;
		case '-':
			Ht1621WrOneData(0,0x04);
			break;
	}
}


/********************************lcd******************************/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - 初始化LCD1602
void LCD_init(void)
{
LCD_RS=0; // - - 指令
LCD_RW=0; // - - 写入
LCD_E=0; // - - 使能
    LCD_write_L4bit_command(0x03); 				// - - 设置4位格式，2行，5x7
LCD_write_L4bit_command(0x03); 				// - - 设置4位格式，2行，5x7
LCD_write_L4bit_command(0x03);				// - - 设置4位格式，2行，5x7
LCD_write_L4bit_command(0x02); 				// - - 设置4位格式，2行，5x7
LCD_write_4bit_command(0x28); 				// - - 设置4位格式，2行，5x7
LCD_write_4bit_command(0x0c); 				// - - 整体显示，关光标，不闪烁
LCD_write_4bit_command(0x06); 				// - - 设定输入方式，增量不移位
LCD_write_4bit_command(0x01);				// - - 清除屏幕显示
}
//********************************
    // - - 向LCD1602写指令
void LCD_write_4bit_command(uchar dat)
{
LCD_RS=0; // - - 指令
LCD_RW=0; // - - 写入
LCD_write_H4bit_command(dat);
LCD_write_L4bit_command(dat);
}
    // - - 向LCD1602写高四位指令
void LCD_write_H4bit_command(uchar dat)
{
LCD_DB=(LCD_DB&0x0F)|(dat&0xF0);
LCD_E=1; // - - 允许
LCD_E=0;
}
    // - - 向LCD1602写低四位指令
void LCD_write_L4bit_command(uchar dat)
{
dat<<=4;
LCD_DB=(LCD_DB&0x0F)|(dat&0xF0);
LCD_E=1; // - - 允许
LCD_E=0;
}
    // - - 向LCD1602写数据
void LCD_write_4bit_data(uchar dat)
{
LCD_RS=1; // - - 数据
LCD_RW=0; // - - 写入
LCD_write_H4bit_command(dat);
LCD_write_L4bit_command(dat);
}
    // - - 设置显示位置
void LCD_set_xy(uchar x,uchar y)
{
uchar address;
if(y==1)
{
address=0x80+x; // - - 第一行位置
} else {
address=0xc0+x; // - - 第二行位置
}
LCD_write_4bit_command(address);
}
    // - - 显示一个字符函数
void LCD_disp_char(uchar x,uchar y,uchar dat) // - - LCD_disp_char(0,1,0x38); // - - 显示8
{
LCD_set_xy(x,y);
LCD_write_4bit_data(dat);
}
// - - 显示一个字符串函数
void LCD_disp_string(uchar x,uchar y,uchar *s)
{
LCD_set_xy(x,y);
while(*s!='\0')
{
LCD_write_4bit_data(*s);
s++;
}
}

void delayms(uint ms)
{
	uint i;
	while (ms--)
	{
		for(i=0;i<120;i++);
	}
}

void delayus_10(uint n)
{ 
	uint i,j;
	for(i=n;i>0;i--)
	for(j=2;j>0;j--); 
} 
