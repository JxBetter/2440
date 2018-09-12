int nand_select_chip(void);
int nand_deselect_chip(void);
int nand_command(unsigned char cmd);
int nand_addr(unsigned char addr);
unsigned char nand_data(void);
int nandflash_init(void);
int nand_read_id(void);
int read_nandflash(unsigned int addr, unsigned char *buff, unsigned int len);
int nandflash_test(void);
