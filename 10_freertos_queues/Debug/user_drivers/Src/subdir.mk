################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_drivers/Src/keypad_4x4.c \
../user_drivers/Src/tm1638_led.c 

OBJS += \
./user_drivers/Src/keypad_4x4.o \
./user_drivers/Src/tm1638_led.o 

C_DEPS += \
./user_drivers/Src/keypad_4x4.d \
./user_drivers/Src/tm1638_led.d 


# Each subdirectory must supply rules for building sources it contributes
user_drivers/Src/%.o: ../user_drivers/Src/%.c user_drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/include" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/FreeRTOS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/CONFIG" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/OS" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/SEGGER" -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/thirdparty/SEGGER VIEW/REC" -I../Core/Inc -I"C:/Users/hoang/OneDrive - ut.edu.vn/Desktop/PROJECT/sample code/FREERTOS/workspace/09_freertos_6tasks/user_drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-user_drivers-2f-Src

clean-user_drivers-2f-Src:
	-$(RM) ./user_drivers/Src/keypad_4x4.d ./user_drivers/Src/keypad_4x4.o ./user_drivers/Src/tm1638_led.d ./user_drivers/Src/tm1638_led.o

.PHONY: clean-user_drivers-2f-Src

