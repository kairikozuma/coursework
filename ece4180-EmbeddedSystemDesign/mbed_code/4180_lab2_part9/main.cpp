#include "mbed.h"
#include "SDFileSystem.h"
 
SDFileSystem sd(p11, p12, p13, p8, "sd"); // the pinout on the mbed Cool Components workshop board
Serial pc(USBTX, USBRX);

int main() {
    /* Change directory */
    mkdir("/sd/mydir", 0777);
    pc.printf("Changed directory\n");
    
    /* Write to a file */
    FILE *fp = fopen("/sd/mydir/sdtest.txt", "w");
    if(fp == NULL) {
        error("Could not open file for write\n");
    }
    fprintf(fp, "Hello SD file world");
    fclose(fp); 
    pc.printf("Wrote to SD card\n");
    
    /* Read from file and output to virtual COM port */
    FILE *fpread = fopen("/sd/mydir/sdtest.txt", "r");
    if(fpread == NULL) {
        error("Could not open file for read\n");
    }
    
    char buffer[20];
    fgets(buffer, sizeof(buffer), fpread);
    pc.printf("Read from file: %s\n", buffer);
    fclose(fpread); 
}
