################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/main_program/arm/ik_calculator.cpp 

OBJS += \
./Core/Src/main_program/arm/ik_calculator.o 

CPP_DEPS += \
./Core/Src/main_program/arm/ik_calculator.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/main_program/arm/%.o Core/Src/main_program/arm/%.su Core/Src/main_program/arm/%.cyclo: ../Core/Src/main_program/arm/%.cpp Core/Src/main_program/arm/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I"/home/jessielin/STM32CubeIDE/workspace_1.19.0/tdk_mission_stm32-main/Core/Src/main_program" -I"/home/jessielin/STM32CubeIDE/workspace_1.19.0/tdk_mission_stm32-main/Core/Inc/uros" -I"/home/jessielin/STM32CubeIDE/workspace_1.19.0/tdk_mission_stm32-main/micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros/include" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-main_program-2f-arm

clean-Core-2f-Src-2f-main_program-2f-arm:
	-$(RM) ./Core/Src/main_program/arm/ik_calculator.cyclo ./Core/Src/main_program/arm/ik_calculator.d ./Core/Src/main_program/arm/ik_calculator.o ./Core/Src/main_program/arm/ik_calculator.su

.PHONY: clean-Core-2f-Src-2f-main_program-2f-arm

