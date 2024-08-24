################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lis3dh-pid/lis3dh_reg.c 

OBJS += \
./Drivers/lis3dh-pid/lis3dh_reg.o 

C_DEPS += \
./Drivers/lis3dh-pid/lis3dh_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lis3dh-pid/%.o Drivers/lis3dh-pid/%.su Drivers/lis3dh-pid/%.cyclo: ../Drivers/lis3dh-pid/%.c Drivers/lis3dh-pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I/Users/elliottmiller/Projects/six_speed_display/stm32/Drivers/lis3dh-pid -I/Users/elliottmiller/Projects/six_speed_display/stm32/Drivers/ST7789-STM32/ST7789 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lis3dh-2d-pid

clean-Drivers-2f-lis3dh-2d-pid:
	-$(RM) ./Drivers/lis3dh-pid/lis3dh_reg.cyclo ./Drivers/lis3dh-pid/lis3dh_reg.d ./Drivers/lis3dh-pid/lis3dh_reg.o ./Drivers/lis3dh-pid/lis3dh_reg.su

.PHONY: clean-Drivers-2f-lis3dh-2d-pid

