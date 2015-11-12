#include "flashWork.h"

unsigned char MEM_status;


struct typeFlash dataFlash = {

&saveParams,	
&readParams,	
&MemoryReadStatus
};

unsigned char MemoryReadStatus()
{
   unsigned char temp;
 
   SSP.setSS0(SSP_ON);
	 temp = SSP.sendSSP0(0xD7);
   SSP.setSS0(SSP_OFF);
   return temp;
}




void saveParams(){

int i;	
unsigned char *bdata = (unsigned char*)(&data); 
int len = sizeof(data);


	
// SSP0.setSS(SPI_ON);	
// SSP0.sendSPI(0x55);
// SSP0.sendSPI(0x09);
// SSP0.sendSPI(0x60);
// SSP0.sendSPI(0x00);
// SSP0.setSS(SPI_OFF);	
// 	
do
{
   MEM_status = MemoryReadStatus();
} while (!(MEM_status & 0x80));	
	
	
SSP.setSS0(SSP_ON);
SSP.sendSSP0(0x87);
SSP.sendSSP0(0x00);
SSP.sendSSP0(0x00);
SSP.sendSSP0(0x00);

//SSP0.sendSPI(0x00);


for( i = 0; i < len; i++){
	SSP.sendSSP0(bdata[i]);
}
SSP.setSS0(SSP_OFF);	


SSP.setSS0(SSP_ON);	
SSP.sendSSP0(0x86);
SSP.sendSSP0(0x09);
SSP.sendSSP0(0x60);
SSP.sendSSP0(0x00);

SSP.setSS0(SSP_OFF);	
	
do
{
   MEM_status = MemoryReadStatus();
} while (!(MEM_status & 0x80));	

	
}


void readParams(){
	
	int i;	
  //unsigned char *bdata ;
	int len = sizeof(data);
  unsigned char *bdata; // = (unsigned char*)malloc(sizeof(unsigned char)*len) ;
	struct typeData *data2;

SSP.setSS0(SSP_ON);	
SSP.sendSSP0(0x55);
SSP.sendSSP0(0x00);
SSP.sendSSP0(0x00);
SSP.sendSSP0(0x00);
	
SSP.setSS0(SSP_OFF);	
	
do
{
   MEM_status = MemoryReadStatus();
} while (!(MEM_status & 0x80));	
	
SSP.setSS0(SSP_ON);
SSP.sendSSP0(0xD2);
SSP.sendSSP0(0x09);
SSP.sendSSP0(0x60);
SSP.sendSSP0(0x00);

	SSP.sendSSP0(0xFF);
	SSP.sendSSP0(0xFF);
	SSP.sendSSP0(0xFF);
	SSP.sendSSP0(0xFF);

for( i = 0; i < len; i++){
	bdata[i] = SSP.sendSSP0(0xFF);
   //tmp = SSP0.sendSPI(0xFF);
	 
}
SSP.setSS0(SSP_OFF);

// for( i = 0; i < len; i++){
// 	 bdata--;
// }

data2 = (struct typeData*)(bdata);
led7.setNumLed7(data2->threshold);
}