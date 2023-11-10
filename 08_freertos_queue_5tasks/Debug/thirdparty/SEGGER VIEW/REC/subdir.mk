################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/REC/segger_uart.c 

OBJS += \
./thirdparty/SEGGER\ VIEW/REC/segger_uart.o 

C_DEPS += \
./thirdparty/SEGGER\ VIEW/REC/segger_uart.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/SEGGER\ VIEW/REC/segger_uart.o: C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/REC/segger_uart.c thirdparty/SEGGER\ VIEW/REC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/include" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/CONFIG" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/OS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/SEGGER" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/REC" -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"thirdparty/SEGGER VIEW/REC/segger_uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-thirdparty-2f-SEGGER-20-VIEW-2f-REC

clean-thirdparty-2f-SEGGER-20-VIEW-2f-REC:
	-$(RM) ./thirdparty/SEGGER\ VIEW/REC/segger_uart.d ./thirdparty/SEGGER\ VIEW/REC/segger_uart.o

.PHONY: clean-thirdparty-2f-SEGGER-20-VIEW-2f-REC

