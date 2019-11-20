#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define SAMPLE_SIZE (sizeof(short)) 			// bytes per sample

#include <pthread.h>
typedef struct {
	int numSamples;
	short *pData;
} wavedata_t;

#define THRESHOLD 150
#define FAN_FILE "/mnt/remote/project/turnOnFan"
#define FAN_FILE_NAME_LENGTH 30
#define WAVE_FILE_TO_READ_FROM "/mnt/remote/project/test.wav"
#define WAVE_FILE_NAME_LENGTH 28

#define OFF_VALUE 0
#define ON_VALUE 1

void AudioMixer_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound);

unsigned long long  audioAverager_getAverage(wavedata_t *pSound);

void processAverage(unsigned long long sum);

void printToFile(int VALUE_TO_WRITE);

void removeWaveFile();

int main(){

	wavedata_t hiHatFile;
	AudioMixer_readWaveFileIntoMemory(WAVE_FILE_TO_READ_FROM, &hiHatFile);

	processAverage(audioAverager_getAverage(&hiHatFile));

	removeWaveFile();

	return 0;
}


unsigned long long  audioAverager_getAverage(wavedata_t *pSound)
{
	// Ensure we are only being asked to play "good" sounds:
	assert(pSound->numSamples > 0);
	assert(pSound->pData);
	unsigned long long sum = 0;
	for (long sampleIndex = 0; sampleIndex <pSound->numSamples; sampleIndex++ ){
		sum += pSound->pData[sampleIndex];
	}

	return sum / pSound->numSamples;
}

void processAverage(unsigned long long sum){
	if (sum > THRESHOLD){
		printToFile(ON_VALUE);
	}else{
		printToFile(OFF_VALUE);
	}
}


// Client code must call AudioMixer_freeWaveFileData to free dynamically allocated data.
void AudioMixer_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound)
{
	assert(pSound);

	// The PCM data in a wave file starts after the header:
	const int PCM_DATA_OFFSET = 44;

	// Open the wave file
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Unable to open file %s.\n", fileName);
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
	}

	// Read PCM data from wave file into memory
	int samplesRead = fread(pSound->pData, SAMPLE_SIZE, pSound->numSamples, file);
	if (samplesRead != pSound->numSamples) {
		fprintf(stderr, "ERROR: Unable to read %d samples from file %s (read %d).\n",
				pSound->numSamples, fileName, samplesRead);
		exit(EXIT_FAILURE);
	}


}

void removeWaveFile(){
	char * remove_command;
	remove_command = malloc (sizeof(*remove_command) * (WAVE_FILE_NAME_LENGTH + 3 +1) );
	snprintf(remove_command, WAVE_FILE_NAME_LENGTH + 3 +1, "rm %s", WAVE_FILE_TO_READ_FROM);
	printf("executing command {%s}\n", remove_command);
	system(remove_command);
	free(remove_command);
	remove_command = NULL;
}


void printToFile(int VALUE_TO_WRITE){

	char * fanFilename;
	fanFilename = malloc(sizeof(*fanFilename) * (FAN_FILE_NAME_LENGTH+1));
	snprintf(fanFilename, FAN_FILE_NAME_LENGTH, "%s", FAN_FILE);


	FILE * brightnessInput = fopen(fanFilename, "w");
	if (brightnessInput == NULL){
		printf("ERROR opening %s\n", fanFilename);
		exit(1);
	}

	printf("writing {%d} to {%s}\n", VALUE_TO_WRITE, fanFilename);

	fprintf(brightnessInput, "%d", VALUE_TO_WRITE );
	fclose(brightnessInput);
}
