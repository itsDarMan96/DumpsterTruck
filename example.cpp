#include <JuceHeader.h>
//#define GUI_DEBUG
#inlcude <AudioFile.h>

extern "C"
{
#include "Spleeter4Stems.h"
}


#include <userenv.h>  // GetUserProfileDirectory() (link with userenv)
const int BUFLEN = 512;
BOOL getCurrentUserDir(char* buf, DWORD buflen)
{
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken))
		return FALSE;
	if (!GetUserProfileDirectory(hToken, buf, &buflen))
		return FALSE;
	CloseHandle(hToken);
	return TRUE;
}
//class MS5_1AI : public AudioProcessor, public AudioProcessorValueTreeState::Listener
Class SpleeterRT 
{
public:
	//==============================================================================
	SpleeterRT()
	{

		int i;

		fs = 44100;
		order = 0;
		char dir[512];
		getCurrentUserDir(dir, 512);
		for (i = 0; i < 4; i++)
			coeffProvPtr[i] = malloc(getCoeffSize());
		// Load coeff
		char file1[17] = "\\drum4stems.dat";
		char file2[17] = "\\bass4stems.dat";
		char file3[26] = "\\accompaniment4stems.dat";
		char file4[17] = "\\vocal4stems.dat";
		size_t len1 = strlen(dir);
		size_t len2 = strlen(file1);
		char *concat = (char*)malloc(len1 + len2 + 1);
		memcpy(concat, dir, len1);
		memcpy(concat + len1, file1, len2 + 1);
		FILE *fp = fopen(concat, "rb");
		fread(coeffProvPtr[0], 1, 39290900, fp);
		fclose(fp);
		free(concat);
		len2 = strlen(file2);
		concat = (char*)malloc(len1 + len2 + 1);
		memcpy(concat, dir, len1);
		memcpy(concat + len1, file2, len2 + 1);
		fp = fopen(concat, "rb");
		fread(coeffProvPtr[1], 1, 39290900, fp);
		fclose(fp);
		free(concat);
		len2 = strlen(file3);
		concat = (char*)malloc(len1 + len2 + 1);
		memcpy(concat, dir, len1);
		memcpy(concat + len1, file3, len2 + 1);
		fp = fopen(concat, "rb");
		fread(coeffProvPtr[2], 1, 39290900, fp);
		fclose(fp);
		free(concat);
		len2 = strlen(file4);
		concat = (char*)malloc(len1 + len2 + 1);
		memcpy(concat, dir, len1);
		memcpy(concat + len1, file4, len2 + 1);
		fp = fopen(concat, "rb");
		fread(coeffProvPtr[3], 1, 39290900, fp);
		fclose(fp);
		free(concat);
		setLatencySamples(16384);
		msr = 0;
	}
	SpleeterRT::~SpleeterRT()
	{
		Spleeter4StemsFree(msr);
		if (coeffProvPtr[0])
		{
			for (int i = 0; i < 4; i++)
				free(coeffProvPtr[i]);
			coeffProvPtr[0] = 0;
		}
	}


	//==============================================================================
	void SpleeterInit(double sampleRate, int maximumExpectedSamplesPerBlock)
	{
		fs = (int)sampleRate;
		if (msr)
		{
			Spleeter4StemsFree(msr);
			free(msr);
		}
		msr = (Spleeter4Stems*)malloc(sizeof(Spleeter4Stems));
		Spleeter4StemsInit(msr, 1536, 256, coeffProvPtr);
	}
	
	/*
	void releaseResources() override
	{
	}*/

	//void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override
	void spleeter_Block(AudioFile *audioFile)
	{

		// number of samples per buffer
		
		//const int n = buffer.getNumSamples();
		const int numSamples = audioFile.getNumSamplesPerChannel();
		
		
		// input channels
		//const float *inputs[2] = { buffer.getReadPointer(0), buffer.getReadPointer(1) };
		
		const float *inputs[2] = { &audioFile.sample[0],&audioFile.sample[1]};
		
		float *tmpPtr1, *tmpPtr2;
		//creating Buffer 
		AudioFile<double>::AudioBuffer buffer;
		//setting to eight channels
		buffer.resize (8);
		//here should set the number of samples per out put channel also
		buffer[0].resize(numSamples);
		buffer[1].resize(numSamples);
		buffer[2].resize(numSamples);
		buffer[3].resize(numSamples);
		buffer[4].resize(numSamples);
		buffer[5].resize(numSamples);
		buffer[6].resize(numSamples);
		buffer[7].resize(numSamples);
		
		/*
		
		float* outputs[8] = { buffer.getWritePointer(0), buffer.getWritePointer(1),
		buffer.getWritePointer(2), buffer.getWritePointer(3), buffer.getWritePointer(4),
		buffer.getWritePointer(5), buffer.getWritePointer(6), buffer.getWritePointer(7) };
		
		*/
		
		float* outputs[8] = { &buffer[0],&buffer[1],&buffer[2],&buffer[3],&buffer[4],&buffer[5],&buffer[6],&buffer[7]}
		
		
		//what is order and where is it assigned thus assigning order =1 for time being
		
		order = 1;
		
		if (order == 1)
		{
			tmpPtr1 = outputs[0];
			tmpPtr2 = outputs[1];
			outputs[0] = outputs[2];
			outputs[1] = outputs[3];
			outputs[2] = tmpPtr1;
			outputs[3] = tmpPtr2;
		}
		else if (order == 2)
		{
			tmpPtr1 = outputs[0];
			tmpPtr2 = outputs[1];
			outputs[0] = outputs[4];
			outputs[1] = outputs[5];
			outputs[4] = tmpPtr1;
			outputs[5] = tmpPtr2;
		}
		else if (order == 3)
		{
			tmpPtr1 = outputs[0];
			tmpPtr2 = outputs[1];
			outputs[0] = outputs[6];
			outputs[1] = outputs[7];
			outputs[6] = tmpPtr1;
			outputs[7] = tmpPtr2;
		}
		// output channels
		int i, offset = 0;
		while (offset < numSamples)
		{
			float *ptr[8] = { outputs[0] + offset, outputs[1] + offset,
			outputs[2] + offset, outputs[3] + offset, outputs[4] + offset,
			outputs[5] + offset, outputs[6] + offset, outputs[7] + offset };
			const int processing = min(n - offset, OVPSIZE); //what is OVPSIZE
			Spleeter4StemsProcessSamples(msr, inputs[0] + offset, inputs[1] + offset, processing, ptr);
			offset += processing;
		}
	}

	//==============================================================================


private:
	//==============================================================================
	void *coeffProvPtr[4];
	Spleeter4Stems *msr;
	int fs;
	int order;
	
};


void main()
{
	
	SpleeterRT *spleeter_rt = new SpleeterRT();
	
	#using the AudioFile instance to load and get the information about the loaded audio.
	
	AudioFile<double> audiofile;
	int sampleRate = audioFile.getSampleRate();
	int bitDepth = audioFile.getBitDepth();
	int numSamples = audioFile.getNumSamplesPerChannel();
	double lengthInSeconds = audioFile.getLengthInSeconds()

	int numChannels = audioFile.getNumChannels();
	bool isMono = audioFile.isMono();
	bool isStereo = audioFile.isStereo();
	
	spleeter_rt -> SpleeterInit(sampleRate,44100);
	spleeter_rt -> spleeter_Block(audioFile);
	
}
	
	
	
	
	

	