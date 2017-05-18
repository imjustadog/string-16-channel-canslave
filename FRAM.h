#define nSS LATGbits.LATG9

void InitSPI();
void WREN();
unsigned char RDSR();
void RDID();
void WRSR(unsigned char value);
unsigned char READ(unsigned char addr1, unsigned char addr2, unsigned char addr3);
