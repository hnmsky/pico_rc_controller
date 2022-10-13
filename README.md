# pico_rc_controller

使用树莓派PICO连接RC接收器， 接受RC遥控器信号，并作为HID设备通过USB连接电脑或树莓派， 遥控小车运行。可以用在donkeycar项目中。

目前只支持2个通道。

## PICO设置

rc_controller 中是PICO代码。使用PIO来读去RC接收器信号。

rc_config.h中配置：

```
#define DEBUG 0 //设置1打印channel 0和channel 1读数
#define CH0_PIN 16 //Channel 0 pin
#define CH1_PIN 17 //Channel 1 pin

/* 设置CH0 CH1的最大值 最小值 和中间值。单位是(2000000/SM_CLK) us. 
    当SM_CLK 是4000000, 4000 就是 2000us. */
#define CH0_MAX 4000
#define CH0_MIN 2278
#define CH0_CENTRE  3274

#define CH1_MAX 4000
#define CH1_MIN 2000
#define CH1_CENTRE  3006



/* State machine clock */
#define SM_CLK 4000000
```

CMakeLists.txt 中需要设置路径

set(PICO_SDK_PATH "set path for pico-sdk")

## donkeycar设置

mysim中是donkeycar的模拟器的设置。 可以使用RC遥控器来控制模拟器或实际的小车

使用下面命令生成my_joystick.py：

```
donkey createjs
```

在myconfig中设置：

```
CONTROLLER_TYPE = 'custom' 
```

## 延迟

使用Jetson Nano， 信号从RC接收器到Jetson Nano的PWM输出，延迟大概是32ms。

---



Use the Raspberry Pi PICO to connect to the RC receiver, receive the RC remote control signal, and connect it to the computer or Raspberry Pi as a HID device via USB to run the remote control car. Can be used in donkeycar projects.

Currently only 2 channels are supported.

## PICO settings

In rc_controller is the PICO code. Use PIO to read the RC receiver signal.

Configuration in rc_config.h:

```
#define DEBUG 0 //Set 1 to print channel 0 and channel 1 readings
#define CH0_PIN 16 //Channel 0 pin
#define CH1_PIN 17 //Channel 1 pin

/* Set the maximum value, minimum value and middle value of CH0 CH1. The unit is (2000000/SM_CLK) us.
When SM_CLK is 4000000, 4000 is 2000us. */
#define CH0_MAX 4000
#define CH0_MIN 2278
#define CH0_CENTRE 3274

#define CH1_MAX 4000
#define CH1_MIN 2000
#define CH1_CENTRE 3006



/* State machine clock */
#define SM_CLK 4000000
```

The SDK path needs to be set in CMakeLists.txt

set(PICO_SDK_PATH "set path for pico-sdk")

## donkeycar settings

mysim is the settings of the donkeycar simulator. The RC remote can be used to control the simulator or the actual car

Generate my_joystick.py with the following command:

```
donkey createjs
```

Set in myconfig:

```
CONTROLLER_TYPE = 'custom'
```

## Latency

The latency from the RC receiver to the PWM output of the Jetson Nano is about 32ms on Jetson Nano platform.