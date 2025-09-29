################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/vl53l5cx/vl53l5cx.c 

OBJS += \
./Drivers/BSP/Components/vl53l5cx/vl53l5cx.o 

C_DEPS += \
./Drivers/BSP/Components/vl53l5cx/vl53l5cx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/vl53l5cx/%.o Drivers/BSP/Components/vl53l5cx/%.su Drivers/BSP/Components/vl53l5cx/%.cyclo: ../Drivers/BSP/Components/vl53l5cx/%.c Drivers/BSP/Components/vl53l5cx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/STM32F4xx_Nucleo" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components/vl53l5cx" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components/vl53l5cx/modules" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/Drivers/BSP/Components/vl53l5cx/porting" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/TOF" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/TOF/Inc" -I"C:/Users/Utente/Desktop/LAUREA MAGISTRALE/2anno_2semestre/embedded control/PROJECT/HandTracking/TOF/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-vl53l5cx

clean-Drivers-2f-BSP-2f-Components-2f-vl53l5cx:
	-$(RM) ./Drivers/BSP/Components/vl53l5cx/vl53l5cx.cyclo ./Drivers/BSP/Components/vl53l5cx/vl53l5cx.d ./Drivers/BSP/Components/vl53l5cx/vl53l5cx.o ./Drivers/BSP/Components/vl53l5cx/vl53l5cx.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-vl53l5cx

