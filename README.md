# TI_LM3S

Generic implementation of Arduino for TI-LM3s board

## Installation - Developers

1. Download the latest version from [https://github.com/huaweiwx/TI_LM3S](https://github.com/huaweiwx/TI_LM3S)
2. Unzip it into [Arduino]/hardware folder,Unzip tools/TI.rar in tools.
3. Install Arduino Zero or Arduino Sam from the board manager for the arm-none-eabi-gcc compiler
4. Download the [latest GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).(above 5.4.0-2016q2)
   move to default position:  C:\Users\Administrator\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\
5. Create a folder c:\Tmp
6. Modify environment variables TEMP and TMP as: 
   TEMP=c:\Tmp
   TMP=c:\Tmp
   Because of the Windows command line length limit, we need to modify the environment variables to compile more files (Such as uCGUI)
