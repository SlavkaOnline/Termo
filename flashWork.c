
#include "flashWork.h"
#include "led7.h"
#include <time.h>

static const int size_params = 8;
static const int len = 10;




struct typeFlash dataFlash = {

&saveParams,	
&readParams,	
&SumWorkTime,
&setAlarmDateTime,
&getAlarmDateTime	
	
};


void setAlarmDateTime(){
	
unsigned int *BRAM = ( unsigned int *)(0xE0084000);
	BRAM[0] = RTC_HOUR;
	BRAM[1] = RTC_MIN;
	BRAM[2] = RTC_SEC;
	BRAM[3] = RTC_DOM;
	BRAM[4] = RTC_MONTH;
	BRAM[5] = RTC_YEAR %100;

}


struct typeDateTimeSend getAlarmDateTime(){
	

  struct typeDateTimeSend dt;
	unsigned int *BRAM = ( unsigned int *)(0xE0084000);


	dt.hh = BRAM[0];
	dt.mm = BRAM[1];
	dt.ss = BRAM[2];
	dt.dm = BRAM[3];
	dt.mh = BRAM[4];
	dt.yy = BRAM[5];
	
	return dt;
	
}




short SumWorkTime(int flag ){
	
	unsigned int *BRAM = ( unsigned int *)(0xE0084000);
	
	switch (flag){
		
	case SETNULLHOUR:
				BRAM[6] = 0;
	break;
	
	case UPDATEHOUR:
			BRAM[6]++;
	break;
	
	case READHOUR:
				return BRAM[6];
	break;
		
		
	}
		
	return 0;
}


int readParams(){
	
	int i;
	FILE *readFile;
	char read_params[len];
	int params[size_params];

	
	readFile = fopen("param.txt", "r");
	
	if(!readFile){
		led7.setNumLed7(63);
		return;
	}
	
	i = 0;
	while( !feof(readFile) ){
		if(fgets(read_params, len, readFile));
		 //strtok(read_params, "\n");
		 params[i] = atoi( strtok(read_params, "\n") );
		 i++;
	}
	
	fclose(readFile);
	
	pr_data->mode[0] = params[0];
	pr_data->mode[1] = params[1];
	pr_data->mode[2] = params[2];
	pr_data->mode[3] = params[3];
	pr_data->mode[4] = params[4];
	pr_data->THdelay = params[5];
	pr_data->UTH = params[6];
	setMode(params[7]);
	
	return 0;
}


int saveParams(){


	int params[size_params];
	int i;
	FILE *saveFile;
	

	
	params[0] = pr_data->mode[0];
	params[1] = pr_data->mode[1];
	params[2] = pr_data->mode[2];
	params[3] = pr_data->mode[3];
	params[4] = pr_data->mode[4];
	params[5] = pr_data->THdelay;
	params[6] = pr_data->UTH;
	params[7] = pr_data->currentMode;
	
	saveFile = fopen("param.txt", "w");
	
	if(!saveFile){
		led7.setNumLed7(63);
		return;
	}
	
	for(i = 0; i < size_params; i++){
		fprintf(saveFile, "%d\n", params[i]);
		//fputs("1\n", saveFile);
	}
	
	fclose(saveFile);	

	return 0;
}
