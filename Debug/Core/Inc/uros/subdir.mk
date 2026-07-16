################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/uros/uros_init.cpp 

OBJS += \
./Core/Inc/uros/uros_init.o 

CPP_DEPS += \
./Core/Inc/uros/uros_init.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/uros/%.o Core/Inc/uros/%.su Core/Inc/uros/%.cyclo: ../Core/Inc/uros/%.cpp Core/Inc/uros/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I"/home/hsuanjung/STM32CubeIDE/tdk_mission/tdk_mission_stm32/Core/Src/main_program" -I"/home/hsuanjung/STM32CubeIDE/tdk_mission/tdk_mission_stm32/Core/Inc/uros" -I"/home/hsuanjung/STM32CubeIDE/tdk_mission/tdk_mission_stm32/micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros/include" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-uros

clean-Core-2f-Inc-2f-uros:
	-$(RM) ./Core/Inc/uros/uros_init.cyclo ./Core/Inc/uros/uros_init.d ./Core/Inc/uros/uros_init.o ./Core/Inc/uros/uros_init.su

.PHONY: clean-Core-2f-Inc-2f-uros

