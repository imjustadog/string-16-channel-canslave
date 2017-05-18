#define SCK LATGbits.LATG13
#define nRST LATGbits.LATG14
#define SDA LATGbits.LATG12

#define ds1302_control_add 0x8e //control address
#define ds1302_charger_add 0x90                      
#define ds1302_clkburst_add 0xbe
#define ds1302_ram_add 0xc0 // ram address



void write8bit(unsigned char input);
void ds1302_write_byte(unsigned char address, unsigned char tmp);
unsigned char read8bit();
unsigned char ds1302_read_byte(unsigned char address);
void ds1302_write_time(unsigned char shift_address, unsigned char time_data);
unsigned char ds1302_bcd2asc(unsigned char temp);
unsigned char ds1302_read_time(unsigned char shift_address);
void ds1302_init(void);
