#include "LSM9DS1.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);

int main() {
    //LSM9DS1 lol(p9, p10, 0x6B, 0x1E);
    LSM9DS1 lol(p9, p10, 0xD6, 0x3C);
    lol.begin();
    if (!lol.begin()) {
        pc.printf("Failed to communicate with LSM9DS1.\n");
    }
    lol.calibrate();
    while(1) {
        lol.readTemp();
        lol.readMag();
        lol.readGyro();
        
        //pc.printf("%d %d %d %d %d %d %d %d %d\n\r", lol.calcGyro(lol.gx), lol.calcGyro(lol.gy), lol.calcGyro(lol.gz), lol.ax, lol.ay, lol.az, lol.mx, lol.my, lol.mz);
        //pc.printf("%d %d %d\n\r", lol.calcGyro(lol.gx), lol.calcGyro(lol.gy), lol.calcGyro(lol.gz));
        pc.printf("gyro: %d %d %d\n\r", lol.gx, lol.gy, lol.gz);
        pc.printf("accel: %d %d %d\n\r", lol.ax, lol.ay, lol.az);
        pc.printf("mag: %d %d %d\n\n\r", lol.mx, lol.my, lol.mz);
        myled = 1;
        wait(.5);
        myled = 0;
        wait(.5);
    }
}
