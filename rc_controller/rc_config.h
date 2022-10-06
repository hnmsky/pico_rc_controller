//
// Created by hnmsky on 10/6/22.
//

#ifndef RC_CONTROLLER_RC_CONFIG_H
#define RC_CONTROLLER_RC_CONFIG_H


#define DEBUG 0 //Set 1 to print out RC data for two channel
#define CH0_PIN 16 //Channel 0 pin
#define CH1_PIN 17 //Channel 1pin

/* Set channel max, centre and min value. Unit is (2000000/SM_CLK) us. When SM_CLK is 4000000, 4000 means 2000us. */
#define CH0_MAX 4000
#define CH0_MIN 2278
#define CH0_CENTRE  3274

#define CH1_MAX 4000
#define CH1_MIN 2000
#define CH1_CENTRE  3006



/* State machine clock */
#define SM_CLK 4000000
#endif //RC_CONTROLLER_RC_CONFIG_H
