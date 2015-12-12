
#include "flashWork.h"
#include "led7.h"

static const int size_params = 8;
static const int len = 10;


struct typeFlash dataFlash = {

&saveParams,	
&readParams,	
&updateSumWorkTime
};



double updateSumWorkTime(int flag ){
	
	int i;
	double hour;
	if(!flag){
			FILE *saveFile;
			int dateTime[6];
			
			dateTime[0] = RTC_SEC;
			dateTime[1] = RTC_MIN;
			dateTime[2] = RTC_HOUR;
			dateTime[3] = RTC_DOM;
			dateTime[4] = RTC_MONTH;
			dateTime[5] = RTC_YEAR;
			
			saveFile = fopen("timework", "w");
			
			for(i = 0; i < 6; i++){
					fprintf(saveFile, "%d\n", dateTime[i]);
			}
		
			fclose(saveFile);
}

	if(flag){
		FILE *readFile;
		char read_datetime[4];
		int dateTime[6];
		struct tm loc_time;
		struct tm last_time;
		
		
		readFile = fopen("timework", "r");
				i = 0;
				while( !feof(readFile) ){
				if(fgets(read_datetime, len, readFile));
				 
				 dateTime[i] = atoi( strtok(read_datetime, "\n") );
				 i++;
				}
				fclose(readFile);
		
			last_time.tm_sec		=	dateTime[0];
			last_time.tm_min		=	dateTime[1];
			last_time.tm_hour		=	dateTime[2];
			last_time.tm_mday		=	dateTime[3];
			last_time.tm_mon		=	dateTime[4];
			last_time.tm_year		=	dateTime[5];
		
			loc_time.tm_sec		=	RTC_SEC;
			loc_time.tm_min		=	RTC_MIN;
			loc_time.tm_hour	=	RTC_HOUR;
			loc_time.tm_mday	=	RTC_DOM;
			loc_time.tm_mon		=	RTC_MONTH;
			loc_time.tm_year	=	RTC_YEAR;
		
		  hour = (difftime( mktime( &loc_time ),  mktime( &last_time ) ) );
		  return hour;
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
