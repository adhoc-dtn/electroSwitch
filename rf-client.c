#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    //char dest[18] = "34:DE:1A:59:B4:6E";
    char dest[18] = "C4:21:C8:C9:BF:55";
    //char dest[18] = "FF:FF:FF:FF:FF:FF";
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );
    printf("connecting...\n");
    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    
    // send a message
    if( status == 0 ) {
        printf("connection SUCCESS!!!\n");
        status = write(s, "hello!", 6);
    } else {
        printf("connection FAILED\n");
    }

    if( status < 0 ) {
        perror("uh oh");
    }else {
        printf("write success\n");
    }

    close(s);
    return 0;
}
