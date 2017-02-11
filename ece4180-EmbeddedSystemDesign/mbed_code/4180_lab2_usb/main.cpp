#include "mbed.h"
#include "MSCFileSystem.h"

MSCFileSystem fs ("fs");

Serial pc(USBTX, USBRX);

int main() {
    /* Change directory */
    pc.printf("Changed directory\n");
    
    /* Write to a file */
    FILE *fp = fopen("/fs/sdtest.txt", "w");
    if(fp == NULL) {
        error("Could not open file for write\n");
    }
    fprintf(fp, "Hello USB file world");
    fclose(fp); 
    pc.printf("Wrote to USB card\n");
    
    /* Read from file and output to virtual COM port */
    FILE *fpread = fopen("/fs/sdtest.txt", "r");
    if(fpread == NULL) {
        error("Could not open file for read\n");
    }
    
    char buffer[22];
    fgets(buffer, sizeof(buffer), fpread);
    pc.printf("Read from file: %s\n", buffer);
    fclose(fpread); 

}