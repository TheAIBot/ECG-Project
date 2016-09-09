################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../derivativeSquareFilter.c \
../highPassFilter.c \
../inputManager.c \
../lowPassFilter.c \
../main.c \
../movingWindowFilter.c \
../peakSearcher.c \
../rawData.c 

OBJS += \
./derivativeSquareFilter.o \
./highPassFilter.o \
./inputManager.o \
./lowPassFilter.o \
./main.o \
./movingWindowFilter.o \
./peakSearcher.o \
./rawData.o 

C_DEPS += \
./derivativeSquareFilter.d \
./highPassFilter.d \
./inputManager.d \
./lowPassFilter.d \
./main.d \
./movingWindowFilter.d \
./peakSearcher.d \
./rawData.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


