#include <stdio.h>
#include <malloc.h>
#include <math.h>

// -------------------------------------------------- [ Section: Endianness ] -
int isBigEndian() {
    int test = 1;
    char *p = (char*)&test;

    return p[0] == 0;
}
void reverseEndianness(const long long int size, void* value){
    int i;
    char result[32];
    for( i=0; i<size; i+=1 ){
        result[i] = ((char*)value)[size-i-1];
    }
    for( i=0; i<size; i+=1 ){
        ((char*)value)[i] = result[i];
    }
}
void toBigEndian(const long long int size, void* value){
    char needsFix = !( (1 && isBigEndian()) || (0 && !isBigEndian()) );
    if( needsFix ){
        reverseEndianness(size,value);
    }
}
void toLittleEndian(const long long int size, void* value){
    char needsFix = !( (0 && isBigEndian()) || (1 && !isBigEndian()) );
    if( needsFix ){
        reverseEndianness(size,value);
    }
}
typedef struct WaveHeader {
    // Riff Wave Header
    char chunkId[4];
    int  chunkSize;
    char format[4];

    // Format Subchunk
    char subChunk1Id[4];
    int  subChunk1Size;
    short int audioFormat;
    short int numChannels;
    int sampleRate;
    int byteRate;
    short int blockAlign;
    short int bitsPerSample;
    //short int extraParamSize;

    // Data Subchunk
    char subChunk2Id[4];
    int  subChunk2Size;

} WaveHeader;

WaveHeader* metadata=(WaveHeader*)malloc(sizeof(WaveHeader));


void main(int argc, char* argv[])
{
  FILE *fp= fopen(argv[1],"r+");
  //read the header
  fread(metadata,sizeof(WaveHeader),1,fp);
  //from metadata get the info about the wave file
  int numChannels,numSamples,bytesPerSample,bytesInChannel;
  int  subChunk2Size;
  
  if(metadata)
  {
    numChannels= metadata->numChannels;
    subChunk2Size= metadata->subChunk2Size;
    bytesPerSample=metadata->bitsPerSample/8;
    bytesInChannel= bytesPerSample/numChannels;
    numSamples=(subChunk2Size)/(numChannels*bytesPerSample);
  }
  float* audioIn= new float[numSamples* numChannels];
  //reading sample by sample
  char buffer[4];
  int offset_main = 0;
  for(int i=0;i<numSamples;i++)
  {
    fread(buffer,bytesPerSample,1,fp);
    int offset=0;
    for(int xchannel=0;xchannel<numChannels;xchannel++)
    {
      if(bytesPerSample==2)
      {
       //*(audioIn+(numSamples*xchannel)+offset_main)=buffer&(0x000f<<xchannel);
        *(audioIn+(numSamples*xchannel)+offset_main)=data_buffer[offset] & 0x00ff;     

      }
      if(bytesPerSample==4)
      {
        //*(audioIn+(numSamples*xchannel)+offset)=buffer&(0x00ff<<xchannel);
         *(audioIn+(numSamples*xchannel)+offset_main)=(data_buffer[offset + 1] << 8)|(data_buffer[offset] & 0x00ff);
                                               

      }
      if(bytesPerSample==8)
      {
       // *(audioIn+(numSamples*xchannel)+offset)=buffer&(0xffff<<xchannel);
        *(audioIn+(numSamples*xchannel)+offset_main)=(data_buffer[offset + 3]<<24)|((data_buffer[offset + 2] & 0x00ff) <<16)| ((data_buffer[offset + 1] & 0x00ff) <<8) | (data_buffer[offset] & 0x00ff) ;
      }
      offset+=bytesInChannel;
    }
    offset_main+=1;
  }
  
  
  


