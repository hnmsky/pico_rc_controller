#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/pio.h"
#include "rc_controller.pio.h"
#include "rc_controller.h"
#include "rc_config.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

// GPIO defines
// Example uses GPIO 2
#define GPIO 2



static PIO pio_0 = pio0;
static uint sm_0 = 0;
static PIO pio_1 = pio1;
static uint sm_1 = 0;
static volatile uint32_t tm0 = CH0_CENTRE;
static volatile uint32_t tm1= CH1_CENTRE;
extern void hid_init(void);
extern void hid_loop(void);
void __isr rc_ch0_handler() {
    pio_interrupt_clear(pio_0, sm_0);
    if (!pio_sm_is_rx_fifo_empty(pio_0, sm_0)) {
        uint32_t val = pio_sm_get(pio_0, sm_0);
        tm0 = 0xFFFFFFFF - val;
    }
    //printf("irq0\n");
}
void __isr rc_ch1_handler() {
    pio_interrupt_clear(pio_1, sm_1);
    if (!pio_sm_is_rx_fifo_empty(pio_1, sm_1)) {
        uint32_t val = pio_sm_get(pio_1, sm_1);
        tm1 = 0xFFFFFFFF - val;
    }
    //printf("irq1\n");
}
void rc_init()
{
    // Clear IRQ flag before starting, and make sure flag doesn't actually
    // assert a system-level interrupt (we're using it as a status flag)
    pio_set_irq0_source_enabled(pio_0, pis_interrupt0 + sm_0, true);
    pio_interrupt_clear(pio_0, sm_0);

    pio_set_irq0_source_enabled(pio_1, pis_interrupt0 + sm_1, true);
    pio_interrupt_clear(pio_1, sm_1);


    irq_set_exclusive_handler(PIO0_IRQ_0, rc_ch0_handler);
    irq_set_enabled(PIO0_IRQ_0, true);
    irq_set_exclusive_handler(PIO1_IRQ_0, rc_ch1_handler);
    irq_set_enabled(PIO1_IRQ_0, true);
}

int16_t get_ch_val(int ch) {
    int32_t ch_range;
    int32_t tm = 0;
    int32_t max, min, centre;
    if (ch == 0) {
        max = CH0_MAX;
        min = CH0_MIN;
        centre = CH0_CENTRE;
        if (tm0 > CH0_MAX)
            tm0 = CH0_MAX;
        if (tm0 < CH0_MIN)
            tm0 = CH0_MIN;
        tm = tm0;
    } else if (ch == 1) {
        max = CH1_MAX;
        min = CH1_MIN;
        centre = CH1_CENTRE;
        if (tm1 > CH1_MAX)
            tm1 = CH1_MAX;
        if (tm1 < CH1_MIN)
            tm1 = CH1_MIN;
        tm = tm1;
    } else {
        return 0;
    }
    

    int32_t ch_val;
    if (tm > centre)
        ch_val = (tm - centre)*128/abs(max-centre);
    else
        ch_val = (tm - centre)*128/abs(centre-min);
    if (ch_val > 127)
        ch_val = 127;
    if (ch_val < -127)
        ch_val = -127;
    if (abs(ch_val) <= 1)
        ch_val = 0;
    printf("ch%d val:%d tm:%d\n",ch, ch_val, tm);
    return ch_val;
}
int main()
{
    stdio_init_all();

#if 0
    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
#endif

    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    gpio_init(GPIO);
    gpio_set_dir(GPIO, GPIO_IN);
    gpio_pull_up(GPIO);
    

    // Example of using the HW divider. The pico_divider library provides a more user friendly set of APIs 
    // over the divider (and support for 64 bit divides), and of course by default regular C language integer
    // divisions are redirected thru that library, meaning you can just use C level `/` and `%` operators and
    // gain the benefits of the fast hardware divider.
    int32_t dividend = 123456;
    int32_t divisor = -321;
    // This is the recommended signed fast divider for general use.
    divmod_result_t result = hw_divider_divmod_s32(dividend, divisor);
    printf("%d/%d = %d remainder %d\n", dividend, divisor, to_quotient_s32(result), to_remainder_s32(result));
    // This is the recommended unsigned fast divider for general use.
    int32_t udividend = 123456;
    int32_t udivisor = 321;
    divmod_result_t uresult = hw_divider_divmod_u32(udividend, udivisor);
    printf("%d/%d = %d remainder %d\n", udividend, udivisor, to_quotient_u32(uresult), to_remainder_u32(uresult));




    // Choose which PIO instance to use (there are two instances)
    //PIO pio = pio0;

    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset0 = pio_add_program(pio_0, &rc_ch0_program);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    sm_0 = pio_claim_unused_sm(pio_0, true);
    printf("sm:%d\n", sm_0);
    rc_ch1_program_init(pio_0, sm_0, offset0, CH0_PIN,SM_CLK);

    uint offset1 = pio_add_program(pio_1, &rc_ch1_program);
    sm_1 = pio_claim_unused_sm(pio_1, true);
    printf("sm:%d\n", sm_1);
    rc_ch1_program_init(pio_1, sm_1, offset1, CH1_PIN,SM_CLK);
    rc_init();

    hid_init();
    while (true) {
        // Blink
        //sleep_ms(1000);
#if DEBUG
        sleep_ms(10);
        printf("ch0:%d ch1:%d\n", tm0,tm1);
#endif
        hid_loop();
    }


    return 0;
}



