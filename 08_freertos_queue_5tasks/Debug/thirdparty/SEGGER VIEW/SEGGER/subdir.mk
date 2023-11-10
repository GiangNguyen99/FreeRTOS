################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.c \
C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.c \
C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.o \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.o \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.d \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.d \
./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.o: C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.c thirdparty/SEGGER\ VIEW/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/include" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/CONFIG" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/OS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/SEGGER" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/REC" -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"thirdparty/SEGGER VIEW/SEGGER/SEGGER_RTT.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.o: C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.S thirdparty/SEGGER\ VIEW/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/CONFIG" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/SEGGER" -x assembler-with-cpp -MMD -MP -MF"thirdparty/SEGGER VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.o: C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.c thirdparty/SEGGER\ VIEW/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/include" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/CONFIG" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/OS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/SEGGER" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/REC" -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"thirdparty/SEGGER VIEW/SEGGER/SEGGER_RTT_printf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.o: C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.c thirdparty/SEGGER\ VIEW/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/include" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/CONFIG" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/OS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/SEGGER" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/REC" -I../Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"thirdparty/SEGGER VIEW/SEGGER/SEGGER_SYSVIEW.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-thirdparty-2f-SEGGER-20-VIEW-2f-SEGGER

clean-thirdparty-2f-SEGGER-20-VIEW-2f-SEGGER:
	-$(RM) ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.d ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT.o ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.d ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_RTT_printf.o ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.d ./thirdparty/SEGGER\ VIEW/SEGGER/SEGGER_SYSVIEW.o

.PHONY: clean-thirdparty-2f-SEGGER-20-VIEW-2f-SEGGER

