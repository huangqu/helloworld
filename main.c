uint8_t DevID = 1;
uint16_t testindex;		
uint16_t timecnt;		
void cmd_process(uint8_t *recvdata,uint32_t len, uint8_t *senddata, uint32_t *sendlen)
{
	uint8_t id;
	uint32_t i;
	uint32_t j;
	uint8_t type,type1;
	uint16_t Checksum = 0;
	id = recvdata[0];
	if( id  != DevID &&  id!=0)   //不是正确地址，同时也不是广播地址
	{
			return ;      //地址不正确返回错误码
	}
	Checksum=CRC16(recvdata,len-2);    //检查校验和是否正确
	if( (*(recvdata+len-2) !=( Checksum & 0x00FF))||(*(recvdata+len-1)!=(Checksum>>8)))
	{
			return;
	}
	type =  recvdata[1];
	type1 =  recvdata[3];
	switch(type)
	{
		case 0x03:
			switch(type1)
			{
				case 0x00:
					senddata[0] = recvdata[0];
					senddata[1] = recvdata[1];
				  senddata[2] = 0x00;
					senddata[3] = 0x20;
					for(i=0; i<16; i++)
					{
						senddata[4+i*2] = testdata[testindex]>>8;
						senddata[4+i*2+1] = testdata[testindex];
					}
					//memcpy(&senddata[4],testdata[testindex],)
					testindex++;
					if(testindex>=sizeof(testdata))
						testindex = 0;
					
					Checksum=CRC16(senddata,36);    //检查校验和是否正确
					senddata[36] = (uint8_t)Checksum;
					senddata[37] = (uint8_t)(Checksum>>8);
					*sendlen = 38;
					break;
				case 0x20:
					senddata[0] = recvdata[0];
					senddata[1] = recvdata[1];
				  senddata[2] = 0x00;
					senddata[3] = 0x08;
					senddata[4] = 17;
					senddata[5] = 8;
					senddata[6] = 3;
					senddata[7] = 12;
					senddata[8] = 10;
					senddata[9] = 10;
					senddata[10] = 1;
					senddata[11] = 1;					
					Checksum=CRC16(senddata,12);    //检查校验和是否正确
					senddata[12] = (uint8_t)Checksum;
					senddata[13] = (uint8_t)(Checksum>>8);
					*sendlen = 14;
					break;
				case 0x37:
				case 0x38:
					senddata[0] = recvdata[0];
					senddata[1] = recvdata[1];
				  senddata[2] = 0x04;
					senddata[3] = 0x0e;
				  senddata[4] = 0x00;
					senddata[5] = 0x1c;
					for(i=0; i<0x1c; i++)
					{
						senddata[6+i*37 + 0] = 17;
						senddata[6+i*37 + 1] = 8;
						senddata[6+i*37 + 2] = 3;
						senddata[6+i*37 + 3] = timecnt/60;
						senddata[6+i*37 + 4] = timecnt%60;
						
						for(j=0; j<16; j++)
						{
							senddata[6+i*37 + 5+j*2] = testdata[testindex]>>8;
							senddata[6+i*37 + 5+j*2+1] = testdata[testindex];
						}						
						timecnt++;
						if(timecnt>= 1440)
							timecnt = 0;
					}
					
					testindex++;
					if(testindex>=sizeof(testdata))
						testindex = 0;
									
					Checksum=CRC16(senddata,1042);    //检查校验和是否正确
					senddata[1042] = (uint8_t)Checksum;
					senddata[1043] = (uint8_t)(Checksum>>8);
					*sendlen = 1044;					
					break;
			}
			break;
		case 0x10:
		{
			switch(type1)
			{
				case 0x20:
					if(recvdata[5]==0x04 && recvdata[6]==0x08)
					{
						senddata[0] = recvdata[0];
						senddata[1] = recvdata[1];
						senddata[2] = 0x00;
						senddata[3] = 0x20;
						senddata[4] = 0x00;
						senddata[5] = 0x04;
						Checksum=CRC16(senddata,6);    //检查校验和是否正确
						senddata[6] = (uint8_t)Checksum;
						senddata[7] = (uint8_t)(Checksum>>8);
						*sendlen = 8;		
					}
					break;
			}
		}
		break;
			
	}
//	memcpy(senddata,recvdata,len);
//	*sendlen = len;
}
