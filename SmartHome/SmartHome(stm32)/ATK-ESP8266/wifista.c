#include "common.h"
#include "stdlib.h"
#include "ds18b20.h"
#include "rc522_function.h"
#include "rc522_config.h"
#include "string.h"
#include "led.h"
#include "beep.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI STA驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 netpro=0;	//网络模式
u8 atk_8266_wifista_test(void)
{ u8 cnt=0;
	//u8 netpro=0;	//网络模式
	u8 i ;
	short temp=0 ;
	char *a ;
	char *shake="shake";
  char cStr[30] ;
  u8 flag = 0;
	u8 flag1 = 0;
  u8 ucArray_ID [ 4 ];                                                                                             //????IC?????UID(IC????)
	u8 ucStatusReturn;   
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP

	netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//选择网络模式

	     //TCP
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240); 
				Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
				atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
				sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
				atk_8266_send_cmd(p,"OK",50);   


		  LCD_Clear(WHITE);
			POINT_COLOR=RED;
			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);			
			LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
			Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
			LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
			sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
			Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
			Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
			Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
			Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
			Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
			atk_8266_wificonf_show(30,180,"请设置路由器无线参数为:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
			POINT_COLOR=BLUE;
			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
			USART3_RX_STA=0;
			while(1)
			{
			       cnt ++  ;
			      temp=DS18B20_Get_Temp();
			    	if(SHAKE==0)
				   {
				    atk_8266_send_cmd("AT+CIPSEND=0,5","OK",200);  //发送指定长度的数据
					 
						atk_8266_send_data(shake,"OK",100);  //发送指定长度的数据
				   }
					  if((temp/10)>=30)
            {
						    BEEP=1 ;
						}
						else
						{
						   BEEP= 0;
						}
				    sprintf((char*)p,"t:%02d.%dC",temp/10,temp%10);//测试数据
				    // sprintf((char*)p,"1111111111111111111\n");
						Show_Str(30+54,100,200,12,p,12,0);
						atk_8266_send_cmd("AT+CIPSEND=0,7","OK",200);  //发送指定长度的数据
					 
						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
						timex=100;
					
				if(SHAKE==0)
				{
				    sprintf((char*)p,"t:%02d.%dC",temp/10,temp%10);
				}
	   
				 ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );	
				 if ( ucStatusReturn == MI_OK  )
		{
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //???(???????????????,?????????????????)
			{  
				flag1=1;

				sprintf ( cStr,"ID:%02d%02d%02d%02d%s" ,ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ],p );
			 // 	Show_Str(10,200,200,12,cStr,12,0);
				LCD_ShowString ( 0,42*16,200,16,16,cStr);
				atk_8266_send_cmd("AT+CIPSEND=0,20","OK",200);  //发送指定长度的数据
			//	delay_ms(200);
				atk_8266_send_data((u8 *)cStr,"OK",100);  //发送指定长度的数据
			//	delay_ms(1000);
				}
	  	}
				
				if(flag)  
				{ 
					GPIO_SetBits(GPIOF,GPIO_Pin_11) ;

					 for(i=0;i<10;i++)
				  { 
					  BEEP=!BEEP ;
				    delay_ms(100) ;
					}
					flag = 0 ;
					BEEP = 0;
					cnt = 0;
		 //   GPIO_SetBits(GPIOF,GPIO_Pin_11) ;
				}
				else if (cnt==10)
				{
				   GPIO_ResetBits(GPIOF,GPIO_Pin_11) ;
				}
					
			
				if(timex)timex--;
				if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
				t++;
			
	delay_ms(1000) ;
			   
				if(USART3_RX_STA&0X8000)		//接收到一次数据了
				{  
					a=(char *)(USART3_RX_BUF);
//					printf("///////// %s",USART3_RX_BUF);	//发送到串�
//					printf("*********    %s\n\n",a);
   			
			 	if(match_string(a,"open")== 0)
					{
					   LED0=!LED0 ;
						 
					}
					else{
						LED1=!LED1 ;
					   flag = 1;
					}
					

			    rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
					USART3_RX_BUF[rlen]=0;		//添加结束符 
			//		printf("%s",USART3_RX_BUF);	//发送到串口   
					sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
					LCD_Fill(30+54,115,239,130,WHITE);
					POINT_COLOR=BRED;
					Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
					POINT_COLOR=BLUE;
					LCD_Fill(30,130,239,319,WHITE);
        
					Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据  
					
					USART3_RX_STA=0;
					if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
					else t=0;                   //状态为已经连接了,10秒后再检查
				}  
				if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
				{
//			//		LCD_Fill(30+54,125,239,130,WHITE);
//					LCD_Fill(60,80,120,92,WHITE);
					constate=atk_8266_consta_check();//得到连接状态
					if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
					else Show_Str(30+30,80,200,12,"连接失败",12,0); 	 
					t=0;
				}
				
				if((t%20)==0);
				atk_8266_at_response(1);

			}
	myfree(SRAMIN,p);		//释放内存 
	return res;		
} 



















int match_string( char *ptr, char *mh)
{
        int count=0;
        char *x;
	      char *y;
        int len;
        len=strlen(mh);
        if(ptr==NULL)
                return 0;
        for(;*ptr!='\0';ptr++)
        {
                if(*ptr==*mh)
                {
                        x=ptr;
                        y=mh;
                        count=0;
                        while(*x!='\0'&&*y!='\0')
                        {
                                while((*x++)==(*y++))
                                {
                                        count++;
                                     
                                        if(count==len)
                                                return 1;
                                }
                        }
        }
        }
        return 0;

}



//int Sunday(const char *ptr, const char *mat)
//{
//        int len_ptr=strlen(ptr);
//        int len_mat=strlen(mat);
//        int next[50];
//        int i,j,pos;
//	      int n ;
//        pos=0;
//        for(i=0;i<50;i++)
//        {
//                next[i]=len_mat+1;
//        }

//        for(i=0;i<len_mat;i++)
//        {
//                        next[mat[i]-'a']=len_mat-i;
//        }
//        while(pos<(len_ptr-len_mat+1))
//        {
//                n=pos ;
//                for(j=0;j<len_mat;j++,n++)
//                {
//                        if(ptr[n]!=mat[j])
//                        {
//                                pos += next[ptr[pos+len_mat]-'a'];
//                                break;
//                        }
//                }
//                if(j==len_mat)
//                        return 1;
//        }
//        return 0;
//}







