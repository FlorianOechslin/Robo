################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Application.c \
../Sources/Buzzer.c \
../Sources/Debounce.c \
../Sources/Drive.c \
../Sources/Event.c \
../Sources/Events.c \
../Sources/KeyDebounce.c \
../Sources/Keys.c \
../Sources/LED.c \
../Sources/LineFollow.c \
../Sources/MCP4728.c \
../Sources/Motor.c \
../Sources/Pid.c \
../Sources/Platform.c \
../Sources/QuadCalib.c \
../Sources/RTOS.c \
../Sources/Reflectance.c \
../Sources/Sem.c \
../Sources/Shell.c \
../Sources/ShellQueue.c \
../Sources/Tacho.c \
../Sources/Timer.c \
../Sources/Trigger.c \
../Sources/main.c 

OBJS += \
./Sources/Application.o \
./Sources/Buzzer.o \
./Sources/Debounce.o \
./Sources/Drive.o \
./Sources/Event.o \
./Sources/Events.o \
./Sources/KeyDebounce.o \
./Sources/Keys.o \
./Sources/LED.o \
./Sources/LineFollow.o \
./Sources/MCP4728.o \
./Sources/Motor.o \
./Sources/Pid.o \
./Sources/Platform.o \
./Sources/QuadCalib.o \
./Sources/RTOS.o \
./Sources/Reflectance.o \
./Sources/Sem.o \
./Sources/Shell.o \
./Sources/ShellQueue.o \
./Sources/Tacho.o \
./Sources/Timer.o \
./Sources/Trigger.o \
./Sources/main.o 

C_DEPS += \
./Sources/Application.d \
./Sources/Buzzer.d \
./Sources/Debounce.d \
./Sources/Drive.d \
./Sources/Event.d \
./Sources/Events.d \
./Sources/KeyDebounce.d \
./Sources/Keys.d \
./Sources/LED.d \
./Sources/LineFollow.d \
./Sources/MCP4728.d \
./Sources/Motor.d \
./Sources/Pid.d \
./Sources/Platform.d \
./Sources/QuadCalib.d \
./Sources/RTOS.d \
./Sources/Reflectance.d \
./Sources/Sem.d \
./Sources/Shell.d \
./Sources/ShellQueue.d \
./Sources/Tacho.d \
./Sources/Timer.d \
./Sources/Trigger.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/Users/Florian/Desktop/Studium/6.Semester/INTRO/KDS/Robo/Static_Code/PDD" -I"/Users/Florian/Desktop/Studium/6.Semester/INTRO/KDS/Robo/Static_Code/IO_Map" -I"/Users/Florian/Desktop/Studium/6.Semester/INTRO/KDS/Robo/Sources" -I"/Users/Florian/Desktop/Studium/6.Semester/INTRO/KDS/Robo/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


