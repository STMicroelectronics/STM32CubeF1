# STM32CubeF1 MCU Firmware Package 

**STM32Cube** is an STMicroelectronics original initiative to ease the developers life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform (this repo), delivered for each series (such as the STM32CubeF1 for the STM32F1 series).
   * The CMSIS modules (core and device) corresponding to the ARM-tm core implemented in this STM32 product
   * The STM32 HAL-LL drivers : an abstraction drivers layer, the API ensuring maximized portability across the STM32 portfolio
   * The BSP Drivers of each evaluation or demonstration board provided by this STM32 series
   * A consistent set of middlewares components such as RTOS, USB, FatFS, LwIP, Graphics ...
   * A full set of software projects (basic examples, applications or demonstrations) for each board provided by this STM32 series

The **STM32CubeF1 MCU Package** projects are directly running on the STM32F1 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples).

In this FW Package, the module **Middlewares/ST/STemWin** is not directly accessible. It must be downloaded from a ST server, the URL is available in a readme.txt file inside the module.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Release_Notes.html).

## Boards available
  * STM32F1
    * [STM3210C-EVAL](https://www.st.com/en/evaluation-tools/stm3210c-eval.html)
    * [STM3210E-EVAL](https://www.st.com/en/evaluation-tools/stm3210e-eval.html)
    * [STM32VLDISCOVERY](https://www.st.com/en/evaluation-tools/stm32vldiscovery.html)
    * [NUCLEO-F103RB](https://www.st.com/en/evaluation-tools/nucleo-f103rb.html)

## Troubleshooting

**Caution** : The **Issues** requests are strictly limited to submit problems or suggestions related to the software delivered in this repo 

**For any other question** related to the STM32F1 product, the hardware performance, the hardware characteristics, the tools, the environment, you can submit a topic on the [ST Community/STM32 MCUs forum](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus)