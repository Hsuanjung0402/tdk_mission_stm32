################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main_program/initial.c \
../Core/Src/main_program/rtos_main.c \
../Core/Src/main_program/servo_control.c 

C_DEPS += \
./Core/Src/main_program/initial.d \
./Core/Src/main_program/rtos_main.d \
./Core/Src/main_program/servo_control.d 

OBJS += \
./Core/Src/main_program/initial.o \
./Core/Src/main_program/rtos_main.o \
./Core/Src/main_program/servo_control.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/main_program/%.o Core/Src/main_program/%.su Core/Src/main_program/%.cyclo: ../Core/Src/main_program/%.c Core/Src/main_program/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I"/home/hsuanjung/STM32CubeIDE/tdk_mission/tdk_mission_stm32/Core/Inc/uros" -I"/home/hsuanjung/STM32CubeIDE/tdk_mission/tdk_mission_stm32/micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros/include" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-main_program

clean-Core-2f-Src-2f-main_program:
	-$(RM) ./Core/Src/main_program/initial.cyclo ./Core/Src/main_program/initial.d ./Core/Src/main_program/initial.o ./Core/Src/main_program/initial.su ./Core/Src/main_program/rtos_main.cyclo ./Core/Src/main_program/rtos_main.d ./Core/Src/main_program/rtos_main.o ./Core/Src/main_program/rtos_main.su ./Core/Src/main_program/servo_control.cyclo ./Core/Src/main_program/servo_control.d ./Core/Src/main_program/servo_control.o ./Core/Src/main_program/servo_control.su

.PHONY: clean-Core-2f-Src-2f-main_program

