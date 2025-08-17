################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/user/bootload.c \
../Core/user/flash.c \
../Core/user/usart_interface.c 

OBJS += \
./Core/user/bootload.o \
./Core/user/flash.o \
./Core/user/usart_interface.o 

C_DEPS += \
./Core/user/bootload.d \
./Core/user/flash.d \
./Core/user/usart_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user/%.o Core/user/%.su Core/user/%.cyclo: ../Core/user/%.c Core/user/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Core/user -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-user

clean-Core-2f-user:
	-$(RM) ./Core/user/bootload.cyclo ./Core/user/bootload.d ./Core/user/bootload.o ./Core/user/bootload.su ./Core/user/flash.cyclo ./Core/user/flash.d ./Core/user/flash.o ./Core/user/flash.su ./Core/user/usart_interface.cyclo ./Core/user/usart_interface.d ./Core/user/usart_interface.o ./Core/user/usart_interface.su

.PHONY: clean-Core-2f-user

