@echo off
echo AXF to BIN file generation ...

"C:\Program Files\Keil\ARM\ARMCC\bin\fromelf.exe" --bin --output ..\MDK-ARM\STM3210E_EVAL\STM3210E_EVAL_SysTick.bin ..\MDK-ARM\STM3210E_EVAL\STM3210E_EVAL.axf 

echo AXF to BIN file generation completed.

