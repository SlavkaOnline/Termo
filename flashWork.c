
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
	
	int params[6];
	int i;
	FILE *saveFile;
	

	
	params[0] = RTC_HOUR;
	params[1] = RTC_MIN;
	params[2] = RTC_SEC;
	params[3] = RTC_DOM;
	params[4] = RTC_MONTH;
	params[5] = RTC_YEAR;
	
	
	saveFile = fopen("alarm.txt", "w");
	
	if(!saveFile){
		led7.setNumLed7(63);
		return;
	}
	
	for(i = 0; i < size_params; i++){
		fprintf(saveFile, "%d\n", params[i]);
		//fputs("1\n", saveFile);
	}
	
	fclose(saveFile);	
}


struct typeDateTimeSend getAlarmDateTime(){
	
	int i;
	FILE *readFile;
	char read_params[6];
	int params[size_params];
  struct typeDateTimeSend dt;
	
	readFile = fopen("alarm.txt", "r");
	
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
	
	dt.hh = params[0];
	dt.mm = params[1];
	dt.ss = params[2];
	dt.dm = params[3];
	dt.mh = params[4];
	dt.yy = params[5];
	
	return dt;
	
}




double SumWorkTime(int flag ){
	
	int i;
	if(!flag){
			FILE *saveFile;
		
			
// 		  saveFile = fopen("timework", "r");
// 			if (saveFile){
// 				fclose(saveFile);
// 				return 0;
// 			}
		 
			
			saveFile = fopen("timework", "w");
			fprintf(saveFile, "%d", 0);
			fclose(saveFile);
}

	if(flag == 1){
		FILE *readFile;
		FILE *saveFile;
		char read_datetime[10];
	
	  int hour;
		readFile = fopen("timework", "r");
		   	if(!readFile){
						led7.setNumLed7(63);
						return;
					}
		
			
				while( !feof(readFile) ){
				if(fgets(read_datetime, len, readFile));
				 
				 hour = atoi( strtok(read_datetime, "\n") );
				 
				}
				fclose(readFile);
		
	      if(flag == 2){
					hour++;
					saveFile = fopen("timework", "w");
					saveFile = fopen("timework", "w");
					fprintf(saveFile, "%d", hour);
					fclose(saveFile);
					return hour;
				}
				else{
					
		    return hour;
				}
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
