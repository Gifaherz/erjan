#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_ADAPTER     "/dev/i2c-0"


int read_buffer(int fd)
{
        struct i2c_rdwr_ioctl_data data;
        struct i2c_msg messages[3];
        unsigned char write_cmd[1] = {0x00};
		unsigned char write_buf[25] = {0xAE, 0xD5, 0x80, 0xA8, 0x1F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0x20, 0x00,  0xA0 | 0x01, 0xC8, 0xDA, 0x00, 0x81, 0xCF, 0xD9,0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF}, read_buf[1] = {0x00};
        unsigned char init[22] = {0x00, 0xAE, 0xA6, 0x20, 0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07, 0xA1, 0xC8, 0xA8, 0x1F, 0xDA, 0x02, 0x81, 0x7F, 0x8D, 0x14, 0xAF};
        unsigned char write[200];
unsigned char pix[10] = {0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        /* 
         * .addr - Адрес устройства (датчика)
         * .flags - операция чтения или записи (0 - w, 1 - r)
         * .len - кол-во передаваемых/принимаемых сообщений
         * .buf - буфер на чтение или запись
         */
        messages[0].addr = 0x3C;
        messages[0].flags = 0;
        messages[0].len = 22;
        messages[0].buf = init;
		

        messages[1].addr = 0x3C;
        messages[1].flags = 0;
        messages[1].len = 19;
        messages[1].buf = init;



	    messages[2].addr = 0x3C;
        messages[2].flags = 0;
        messages[2].len = 10;
		messages[2].buf = pix;
 
        data.msgs = messages;
        data.nmsgs = 1;
 
        if (ioctl(fd, I2C_RDWR, &data) < 0)
                printf("Cant send data!\n");
        else
                printf("ID = 0x%x\n", read_buf[0]);

}


void clear(int fd)
{
        struct i2c_rdwr_ioctl_data data;
        struct i2c_msg messages[1];
        unsigned char write_cmd[1] = {0x40};
       unsigned char pix[9] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
       // unsigned char pix[9] = {0x40, 0xFF, 0xF0, 0xFF, 0xF0, 0xFF, 0xF0, 0xFF, 0xF0};
        messages[0].addr = 0x3C;
        messages[0].flags = 0;
        messages[0].len = 9;
        messages[0].buf = pix;

        data.msgs = messages;
        data.nmsgs = 1;

        for(int i=0; i < 16*4; i++)
        {
                   if (ioctl(fd, I2C_RDWR, &data) < 0)
                           printf("Cant send data!\n");
                           else  printf("ok! %d \n", i);

        }

}

int main(int argc, char **argv)
{
        int fd;

         /*
          * Open I2C file descriptor.
          */
         fd = open(I2C_ADAPTER, O_RDWR);

         if (fd < 0) {
                 printf("Unable to open i2c file\n");
                 return 0;
         }

         read_buffer(fd);
         clear(fd);

         return 0;
 }
