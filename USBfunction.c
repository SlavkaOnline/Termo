#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"


#include "USBfunction.h"

U8 InReport[64];                                /* HID Input Report    */
                                            /*   Bit0   : Buttons  */
                                            /*   Bit1..7: Reserved */
U8 OutReport[63]; 

U8 flagReport;

void GetInReport (void) {
	
	switch(flagReport){
   	
		case 0x02:
			sendDateTime();
			flagReport = 0x00;
		break;
		
		default:
			getConstParam();
		break;
	}
}


/*------------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *------------------------------------------------------------------------------*/
void ChangeConstParam (void) {

	int i;
	struct typePCtoMCnumber datasend;
	datasend = *((struct typePCtoMCnumber *)&OutReport[1]);
	
	data.mode[0] = datasend.threshold[0];
	data.mode[1] = datasend.threshold[1];
	data.mode[2] = datasend.threshold[2];
	data.mode[3] = datasend.threshold[3];
	data.mode[4] = datasend.threshold[4];
	data.THdelay = datasend.THdelay;
	data.UTH = datasend.UTH;
	
	setMode(datasend.currentMode);
	
	while(dataFlash.saveParams());	
}

void getConstParam(){
	
		unsigned char* message = (unsigned char*)&data;
		memcpy(&InReport, message, sizeof(data));
	
}

void sendDateTime(void){

	struct typeInfo info;
	struct typeDateTimeSend alarm;
	unsigned char* message = (unsigned char*)&info;
		info.dt.hh = RTC_HOUR;
		info.dt.mm = RTC_MIN;
		info.dt.ss = RTC_SEC;
		info.dt.dm = RTC_DOM;
		info.dt.mh = RTC_MONTH;
		info.dt.yy = RTC_YEAR;
	
	info.alarm = dataFlash.getAlarmDateTime();
	info.workTime = 0; //dataFlash.SumWorkTime(READHOUR);
	info.dd = 11;
	info.mm = 12;
	info.yyyy = 2015;

  memcpy(&InReport, message, sizeof(info));
	led7.setNumLed7(InReport[2]);
	
}

void setDateTime(void){
	
	struct typeDateTimeSend DateTime;
	struct tm loc_time;
	DateTime = *((struct typeDateTimeSend*)&OutReport[1]);
	
	led7.setNumLed7(DateTime.dm);
	
	loc_time.tm_sec		=	DateTime.ss;
	loc_time.tm_min		=	DateTime.mm;
	loc_time.tm_hour	=	DateTime.hh;
	loc_time.tm_mday	=	DateTime.dm;
	loc_time.tm_mon		=	DateTime.mh;
	loc_time.tm_year	=	DateTime.yy;

	correct_time_struct(&loc_time);
	correct_data_struct(&loc_time);

}

void SetOutReport (void) {
			

	
		switch(OutReport[0]){
			
			case 0x01 : ChangeConstParam (); break;
			case 0x02 : flagReport = 0x02; break;
			case 0x03 : setDateTime(); break;
			case 0xd0 : disableAlarm(0,1); break;
			case 0xd1 : disableAlarm(1,1); break;
			case 0xd2 : disableAlarm(2,1); break;
			
		}
	
}


