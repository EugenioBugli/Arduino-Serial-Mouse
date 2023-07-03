#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

#define BAUD_RATE B19200

int main() {
    int serial_fd = open("/dev/ttyACM0", O_RDONLY);
    if(serial_fd  <= 0) {
        printf("Error (Open)\n");
        return 0;
    }
    usleep(2000000);
    tcflush(serial_fd, TCIFLUSH);
    struct termios tty;
    tcgetattr(serial_fd, &tty); // setting stored to tty
    cfsetispeed(&tty, BAUD_RATE); // set baudrate
    tcsetattr(serial_fd, TCSANOW, &tty); // apply tty to serial port
    

    char buf[256];
    ssize_t num_bytes;
    int buf_idx = 0;
    int complete = 0;
    printf("Serial Port /dev/ttyACM0\n");
    printf("------------------------\n");
    while(1) {
        //read one byte at time
        num_bytes = read(serial_fd, &buf[buf_idx],1);
        if(num_bytes <= -1) printf("Error (Read)");
        if(num_bytes > 0) {
            //check if the message is complete (message ends with \n\n)
            if(buf[buf_idx-1] == '\n' && buf[buf_idx] == '\n') {
                buf[buf_idx] = '\0';
                printf("%s", buf);
                complete = 1;
            }
            else buf_idx++;
        }

        if (complete) {
            complete = 0;
            buf_idx = 0;
        }
        
    }

    close(serial_fd);
    return 0;
}





