################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/UART/uart.c 

OBJS += \
./BSP/UART/uart.o 

C_DEPS += \
./BSP/UART/uart.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/UART/%.o BSP/UART/%.su BSP/UART/%.cyclo: ../BSP/UART/%.c BSP/UART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xE -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STworkspace/PIANO_GETTER/Dev/FLASH" -I"D:/STworkspace/PIANO_GETTER/Dev/TTP229" -I"D:/STworkspace/PIANO_GETTER/BSP/UART" -I"D:/STworkspace/PIANO_GETTER/Dev/SONG" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-BSP-2f-UART

clean-BSP-2f-UART:
	-$(RM) ./BSP/UART/uart.cyclo ./BSP/UART/uart.d ./BSP/UART/uart.o ./BSP/UART/uart.su

.PHONY: clean-BSP-2f-UART

