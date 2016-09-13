################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../benchmarks.c \
../circularArray.c \
../derivativeSquareFilter.c \
../filter.c \
../highPassFilter.c \
../inputManager.c \
../lowPassFilter.c \
../main.c \
../movingWindowFilter.c \
../peakSearcher.c \
../rPeakFinder.c \
../rawData.c \
../tests.c 

OBJS += \
./benchmarks.o \
./circularArray.o \
./derivativeSquareFilter.o \
./filter.o \
./highPassFilter.o \
./inputManager.o \
./lowPassFilter.o \
./main.o \
./movingWindowFilter.o \
./peakSearcher.o \
./rPeakFinder.o \
./rawData.o \
./tests.o 

C_DEPS += \
./benchmarks.d \
./circularArray.d \
./derivativeSquareFilter.d \
./filter.d \
./highPassFilter.d \
./inputManager.d \
./lowPassFilter.d \
./main.d \
./movingWindowFilter.d \
./peakSearcher.d \
./rPeakFinder.d \
./rawData.d \
./tests.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


