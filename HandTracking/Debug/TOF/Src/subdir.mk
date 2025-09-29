################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TOF/Src/app_tof.c 

OBJS += \
./TOF/Src/app_tof.o 

C_DEPS += \
./TOF/Src/app_tof.d 


# Each subdirectory must supply rules for building sources it contributes
TOF/Src/app_tof.o: ../TOF/Src/app_tof.c TOF/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/STM32F4xx_Nucleo" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components/vl53l5cx" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components/vl53l5cx/modules" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components/vl53l5cx/porting" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/TOF" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/TOF/Inc" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/TOF/Src" -O0 -ffunction-sections -fdata-sections -Wall "-u _printf_float" -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TOF-2f-Src

clean-TOF-2f-Src:
	-$(RM) ./TOF/Src/app_tof.cyclo ./TOF/Src/app_tof.d ./TOF/Src/app_tof.o ./TOF/Src/app_tof.su

.PHONY: clean-TOF-2f-Src

