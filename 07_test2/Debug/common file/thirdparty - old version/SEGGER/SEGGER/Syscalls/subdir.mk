################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c 

OBJS += \
./common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o 

C_DEPS += \
./common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d 


# Each subdirectory must supply rules for building sources it contributes
common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o: C:/Users/hoang/OneDrive\ -\ ut.edu.vn/Desktop/PROJECT/sample\ code/FREERTOS/workspace/common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/common file/thirdparty - old version/FreeRTOS/include" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/common file/thirdparty - old version/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/common file/thirdparty - old version/SEGGER/Config" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/common file/thirdparty - old version/SEGGER/OS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/common file/thirdparty - old version/SEGGER/SEGGER" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/common file/thirdparty - old version/SEGGER/Rec" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"common file/thirdparty - old version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-common-20-file-2f-thirdparty-20--2d--20-old-20-version-2f-SEGGER-2f-SEGGER-2f-Syscalls

clean-common-20-file-2f-thirdparty-20--2d--20-old-20-version-2f-SEGGER-2f-SEGGER-2f-Syscalls:
	-$(RM) ./common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d ./common\ file/thirdparty\ -\ old\ version/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o

.PHONY: clean-common-20-file-2f-thirdparty-20--2d--20-old-20-version-2f-SEGGER-2f-SEGGER-2f-Syscalls

