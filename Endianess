#include <stdio.h>

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
    if( ! isBigEndian() ){
        reverseEndianness(size,value);
    }
}
void toLittleEndian(const long long int size, void* value){
    if( isBigEndian() ){
        reverseEndianness(size,value);
    }
}

char bigEndianChar(char value){ char copy = value; toBigEndian(sizeof(char),(void*)&copy); return copy; }
char litEndianChar(char value){ char copy = value; toLittleEndian(sizeof(char),(void*)&copy); return copy; }

int bigEndianInt(int value){ int copy = value; toBigEndian(sizeof(int),(void*)&copy); return copy; }
int litEndianInt(int value){ int copy = value; toLittleEndian(sizeof(int),(void*)&copy); return copy; }

short int bigEndianShort(short int value){ short int copy = value; toBigEndian(sizeof(short int),(void*)&copy); return copy; }
short int litEndianShort(short int value){ short int copy = value; toLittleEndian(sizeof(short int),(void*)&copy); return copy; }

long int bigEndianLong(long int value){ long int copy = value; toBigEndian(sizeof(long int),(void*)&copy); return copy; }
long int litEndianLong(long int value){ long int copy = value; toLittleEndian(sizeof(long int),(void*)&copy); return copy; }

float bigEndianFloat(float value){ float copy = value; toBigEndian(sizeof(float),(void*)&copy); return copy; }
float litEndianFloat(float value){ float copy = value; toLittleEndian(sizeof(float),(void*)&copy); return copy; }

double bigEndianDouble(double value){ double copy = value; toBigEndian(sizeof(double),(void*)&copy); return copy; }
double litEndianDouble(double value){ double copy = value; toLittleEndian(sizeof(double),(void*)&copy); return copy; }

int main(){

    char myChar = 'a';
    char myCharBig = bigEndianChar(myChar);
    char myCharLit = litEndianChar(myChar);
    printf("\nSizeof(char): %lu\n",sizeof(char));
    printf("System:%x\n",myChar);
    printf("Big Endian:%x\n",myCharBig);
    printf("Little Endian:%x\n",myCharLit);

    short int myShort = 0x1234;
    short int myShortBig = bigEndianShort(myShort);
    short int myShortLit = litEndianShort(myShort);
    printf("\nSizeof(short int): %lu\n",sizeof(short));
    printf("System:%hx\n",myShort);
    printf("Big Endian:%hx\n",myShortBig);
    printf("Little Endian:%hx\n",myShortLit);

    int myInt = 0x12345678;
    int myIntBig = bigEndianInt(myInt);
    int myIntLit = litEndianInt(myInt);
    printf("\nSizeof(int): %lu\n",sizeof(int));
    printf("System:%x\n",myInt);
    printf("Big Endian:%x\n",myIntBig);
    printf("Little Endian:%x\n",myIntLit);

    long int myLong = 0x0123456789ABCDEF;
    long int myLongBig = bigEndianLong(myLong);
    long int myLongLit = litEndianLong(myLong);
    printf("\nSizeof(long int): %lu\n",sizeof(long int));
    printf("System:%lx\n",myLong);
    printf("Big Endian:%lx\n",myLongBig);
    printf("Little Endian:%lx\n",myLongLit);

    float myFloat = 1.234e-5;
    float myFloatBig = bigEndianFloat(myFloat);
    float myFloatLit = litEndianFloat(myFloat);
    printf("\nSizeof(float): %lu\n",sizeof(float));
    printf("System:%e\n",myFloat);
    printf("Big Endian:%e\n",myFloatBig);
    printf("Little Endian:%e\n",myFloatLit);

    double myDouble = 1.234e-5;
    double myDoubleBig = bigEndianDouble(myDouble);
    double myDoubleLit = litEndianDouble(myDouble);
    printf("\nSizeof(double): %lu\n",sizeof(double));
    printf("System:%e\n",myDouble);
    printf("Big Endian:%e\n",myDoubleBig);
    printf("Little Endian:%e\n",myDoubleLit);

}
