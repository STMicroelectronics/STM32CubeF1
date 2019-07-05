@echo off
echo ELF to BIN file generation ...

"C:\Program Files (x86)\Atollic\TrueSTUDIO for ARM Pro 5.1.1\ARMTools\bin\arm-atollic-eabi-objcopy.exe" -O binary ..\TrueStudio\STM3210C_EVAL\Debug\STM3210C_EVAL.elf ..\TrueStudio\STM3210C_EVAL\STM3210C_EVAL_SysTick.bin

echo ELF to BIN file generation completed.

pause