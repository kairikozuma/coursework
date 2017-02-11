#include "mbed.h"

Serial pc(USBTX, USBRX);

int main() {
    while(1) {
        pc.putc(pc.getc());
    }
}