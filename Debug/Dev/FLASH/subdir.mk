################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dev/FLASH/gd25q16e.c 

OBJS += \
./Dev/FLASH/gd25q16e.o 

C_DEPS += \
./Dev/FLASH/gd25q16e.d 


# Each subdirectory must supply rules for building sources it contributes
Dev/FLASH/%.o Dev/FLASH/%.su Dev/FLASH/%.cyclo: ../Dev/FLASH/%.c Dev/FLASH/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xE -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STworkspace/PIANO_GETTER/Dev/FLASH" -I"D:/STworkspace/PIANO_GETTER/Dev/TTP229" -I"D:/STworkspace/PIANO_GETTER/BSP/UART" -I"D:/STworkspace/PIANO_GETTER/Dev/SONG" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Dev-2f-FLASH

clean-Dev-2f-FLASH:
	-$(RM) ./Dev/FLASH/gd25q16e.cyclo ./Dev/FLASH/gd25q16e.d ./Dev/FLASH/gd25q16e.o ./Dev/FLASH/gd25q16e.su

.PHONY: clean-Dev-2f-FLASH

