#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include <mraa.h>

mraa_gpio_context out;
volatile int mutex = 0;

void interrupt_in(void *arg)
{
    mraa_gpio_context dev = (mraa_gpio_context)arg;
    if(mutex == 0){
        mutex = 1;
       system("clear"); 
       if(mraa_gpio_read(dev) == 1){
            mraa_gpio_write(out, 1);
            printf("Released\r\n");
        }else{
            mraa_gpio_write(out, 0);
            printf("Pressed\r\n");
        }
        mutex = 0;
    }
}

int main(int argc, char *argv[])
{
    mraa_init();

    fprintf(stdout, "Hello mraa.\nVersion: %s\n", mraa_get_version());

    // LED
    out = mraa_gpio_init(20);
    if(out == NULL){
        printf("Error: init out.\r\n");
        return 1;
    }
    mraa_gpio_dir(out, MRAA_GPIO_OUT);

    // Switch
    mraa_gpio_context in = mraa_gpio_init(14);
    if(in == NULL){
        printf("Error: init in.\r\n");
        return 1;
    }
    mraa_gpio_dir(in, MRAA_GPIO_IN);
    mraa_gpio_mode(in, MRAA_GPIO_PULLUP);

    // 割込関数の登録
    mraa_gpio_isr(in, MRAA_GPIO_EDGE_BOTH, interrupt_in, (void *)in);

    while(1) {}




    mraa_gpio_isr_exit(in);
    mraa_gpio_close(in);
    mraa_gpio_close(out);

    mraa_deinit();

    return 0;
}
