################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/main.cpp 

OBJS += \
./Src/main.o 

CPP_DEPS += \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.cpp Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++20 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/nico/Gits/CESE/PdM/PW1_Exercise_1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/nico/Gits/CESE/PdM/PW1_Exercise_1/Drivers/CMSIS/Include" -I"/home/nico/Gits/CESE/PdM/PW1_Exercise_1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/nico/Gits/CESE/PdM/PW1_Exercise_1/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/nico/Gits/CESE/PdM/PW1_Exercise_1/Drivers/Core/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su

.PHONY: clean-Src

