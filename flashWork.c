#include "flashWork.h"


static const int size_params = 6;
static const int len = 10;

struct typeFlash dataFlash = {

&saveParams,	
&readParams,	

};


void readParams(){
	
	int i;
	FILE *readFile;
	char read_params[len];
	int p[size_params];
	char *tmp_str;
	
	readFile = fopen("params.txt", "w");
	
	i = 0;
	while( !feof(readFile) ){
		if(fgets(read_params, len, readFile));
		 //strtok(read_params, "\n");
		 p[i] = atoi( strtok(read_params, "\n") );
		 i++;
	}
	
	fclose(readFile);
	
}


void saveParams(){

	int params[size_params];
	int i;
	FILE *saveFile;
	
	params[0] = pr_data->mode[0];
	params[1] = pr_data->mode[1];
	params[2] = pr_data->mode[2];
	params[3] = pr_data->mode[4];
	params[4] = pr_data->THdelay;
	params[5] = pr_data->UTH;
	
	
	saveFile = fopen("params.txt", "w");
	
	for(i = 0; i < size_params; i++){
		fprintf(saveFile, "%d\n", params[i]);
	}
	
	fclose(saveFile);
	


}