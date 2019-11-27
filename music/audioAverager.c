#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define SAMPLE_SIZE (sizeof(short)) 			// bytes per sample


typedef struct {
	int numSamples;
	short *pData;
} wavedata_t;

#define SOUND_RECORD_COMMAND "arecord -f cd -D plughw:1,0 -d 1 test.wav"

#define THRESHOLD 500
#define FAN_FILE "/mnt/remote/myApp/turnOnFan"
#define FAN_FILE_NAME_LENGTH 30
#define OFF_VALUE 0
#define ON_VALUE 1
void averager_processAverageForFan(unsigned long long sum);
void averager_printToFanFile(int VALUE_TO_WRITE);
unsigned long long  averager_getAverageForFan(wavedata_t *pSound);


#define WAVE_FILE_TO_READ_FROM "/mnt/remote/myApp/test.wav"
#define WAVE_FILE_NAME_LENGTH 28
int averager_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound);


static const char * visualizerFile;
void averager_removeWaveFile();
void averager_createVisualizerFile(wavedata_t *pSound);
void averager_printToVisualizerFile(int VALUE_TO_WRITE);


#define STATUS_FILE_LOCATION "/mnt/remote/myApp/status"
#define PROGRAM_ON '1'
static const char * statusFile;
void averager_createStatusFile(int VALUE_TO_WRITE);
char averager_readStatusFile();



#define NUMBER_OF_BUCKETS 8
#define THRESHOLD_2 50
#define THRESHOLD_3 200
#define THRESHOLD_4 300
#define THRESHOLD_5 400
#define THRESHOLD_6 500
#define THRESHOLD_7 600
#define THRESHOLD_8 800
 
//#include <limits.h>

void print_sound(wavedata_t *pSound);

int main(){
	visualizerFile = getenv("RGB_SCREENCAPTURE_FILE");
	statusFile = STATUS_FILE_LOCATION;

	if ( visualizerFile == NULL){
		printf("the environment variable \"RGB_SCREENCAPTURE_FILE\" has not been exported. Please read through README again\n.");
		return -1;
	}

	averager_createStatusFile(1);


	while(averager_readStatusFile() == PROGRAM_ON){
		system(SOUND_RECORD_COMMAND);
		wavedata_t hiHatFile;
		if (averager_readWaveFileIntoMemory(WAVE_FILE_TO_READ_FROM, &hiHatFile) == 0){
			
			//print_sound(&hiHatFile);
			
			averager_processAverageForFan(averager_getAverageForFan(&hiHatFile));
			averager_createVisualizerFile(&hiHatFile);
			
			averager_removeWaveFile();
		}
	}
	return 0;
}


unsigned long long  averager_getAverageForFan(wavedata_t *pSound){
	// Ensure we are only being asked to play "good" sounds:
	assert(pSound->numSamples > 0);
	assert(pSound->pData);
	unsigned long long sum = 0;
	for (long sampleIndex = 0; sampleIndex <pSound->numSamples; sampleIndex++ ){
		sum += pSound->pData[sampleIndex];
	}

	return sum / pSound->numSamples;
}

void averager_processAverageForFan(unsigned long long sum){
	if (sum > THRESHOLD){
		averager_printToFanFile(ON_VALUE);
	}else{
		averager_printToFanFile(OFF_VALUE);
	}
}


// Client code must call AudioMixer_freeWaveFileData to free dynamically allocated data.
int averager_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound){
	assert(pSound);

	// The PCM data in a wave file starts after the header:
	const int PCM_DATA_OFFSET = 44;

	// Open the wave file
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Unable to open file %s.\n", fileName);
		return EXIT_FAILURE;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	int sizeInBytes = ftell(file) - PCM_DATA_OFFSET;
	pSound->numSamples = sizeInBytes / SAMPLE_SIZE;

	// Search to the start of the data in the file
	fseek(file, PCM_DATA_OFFSET, SEEK_SET);

	// Allocate space to hold all PCM data
	pSound->pData = malloc(sizeInBytes);
	if (pSound->pData == 0) {
		fprintf(stderr, "ERROR: Unable to allocate %d bytes for file %s.\n",
		sizeInBytes, fileName);
		return EXIT_FAILURE;
	}

	// Read PCM data from wave file into memory
	int samplesRead = fread(pSound->pData, SAMPLE_SIZE, pSound->numSamples, file);
	if (samplesRead != pSound->numSamples) {
		fprintf(stderr, "ERROR: Unable to read %d samples from file %s (read %d).\n",
		pSound->numSamples, fileName, samplesRead);
		return EXIT_FAILURE;
	}

	return 0;
}

void print_sound(wavedata_t *pSound){
	
	// print 100 samples
	for(int i = 0; i<100; i++){
		printf("%d ", pSound->pData[i]);
	}

}

void averager_removeWaveFile(){
	char * remove_command;
	remove_command = malloc (sizeof(*remove_command) * (WAVE_FILE_NAME_LENGTH + 3 +1) );
	snprintf(remove_command, WAVE_FILE_NAME_LENGTH + 3 +1, "rm %s", WAVE_FILE_TO_READ_FROM);
	//printf("executing command {%s}\n", remove_command);
	system(remove_command);
	free(remove_command);
	remove_command = NULL;
}


void averager_printToFanFile(int VALUE_TO_WRITE){

	char * fanFilename;
	fanFilename = malloc(sizeof(*fanFilename) * (FAN_FILE_NAME_LENGTH+1));
	snprintf(fanFilename, FAN_FILE_NAME_LENGTH, "%s", FAN_FILE);


	FILE * brightnessInput = fopen(fanFilename, "w");
	if (brightnessInput == NULL){
		printf("ERROR opening %s\n", fanFilename);
		exit(1);
	}

	//printf("writing {%d} to {%s}\n", VALUE_TO_WRITE, fanFilename);

	fprintf(brightnessInput, "%d", VALUE_TO_WRITE );
	fclose(brightnessInput);
}

void averager_createVisualizerFile(wavedata_t *pSound){
	
	double bucket_size = pSound->numSamples;
	bucket_size = bucket_size / NUMBER_OF_BUCKETS;
	double fraction = bucket_size - ((long)bucket_size);
	if (fraction == 0){
		bucket_size++;
	}
	unsigned long int count = 0;
	unsigned long int sum;
	
	char messageToWrite[1024]=""; 

	//averager_printToVisualizerFile(2);
	
	strcat(messageToWrite,"2");
	
	for (long sampleIndex = 0; sampleIndex <pSound->numSamples; sampleIndex++ ){
		if (count == bucket_size){
			
			sum = sum / bucket_size;
			printf("sum == %ld\n", sum);
			if (sum > THRESHOLD_8){
				//averager_printToVisualizerFile(8);
				
				strcat(messageToWrite," 8");
			}else if (sum > THRESHOLD_7){
				//averager_printToVisualizerFile(7);
				strcat(messageToWrite," 7");

			}else if (sum > THRESHOLD_6){
				//averager_printToVisualizerFile(6);
				strcat(messageToWrite," 6");

			}else if (sum > THRESHOLD_5){
				//averager_printToVisualizerFile(5);
				strcat(messageToWrite," 5");

			}else if (sum > THRESHOLD_4){
				//averager_printToVisualizerFile(4);
				strcat(messageToWrite," 4");

			}else if (sum > THRESHOLD_3){
				//averager_printToVisualizerFile(3);
				strcat(messageToWrite," 3");
				
			}else if (sum > THRESHOLD_2){
				//averager_printToVisualizerFile(2);
				strcat(messageToWrite," 2");

			}else{
				//averager_printToVisualizerFile(1);
				strcat(messageToWrite," 1");

			}

			count = 0;
			sum = 0;
		}
		sum += pSound->pData[sampleIndex];
		count++;

	}

	char write_cmd[2048]="" ; 
	
	sprintf(write_cmd,"echo %s | tee %s ",messageToWrite,visualizerFile);
	system(write_cmd);
			


}

void averager_printToVisualizerFile(int VALUE_TO_WRITE){
	

	FILE * brightnessInput = fopen(visualizerFile, "w");
	if (brightnessInput == NULL){
		printf("ERROR opening %s\n", visualizerFile);
		exit(1);
	}

	//printf("writing {%d} to {%s}\n", VALUE_TO_WRITE, visualizerFile);

	fprintf(brightnessInput, "%d ", VALUE_TO_WRITE );
	fclose(brightnessInput);

}

void averager_createStatusFile(int VALUE_TO_WRITE){

	
	FILE * brightnessInput = fopen(statusFile, "w");
	if (brightnessInput == NULL){
		printf("ERROR opening %s\n", statusFile);
		exit(1);
	}

	//printf("writing {%d} to {%s}\n", VALUE_TO_WRITE, statusFile);

	fprintf(brightnessInput, "%d ", VALUE_TO_WRITE );
	fclose(brightnessInput);
}


char averager_readStatusFile(){


	FILE * brightnessInput = fopen(statusFile, "r");
	if (brightnessInput == NULL){
		printf("ERROR opening %s\n", statusFile);
		exit(1);
	}

	//printf("writing {%d} to {%s}\n", VALUE_TO_WRITE, visualizerFile);
	char val = fgetc( brightnessInput );
	fclose(brightnessInput);
	return val;
}
