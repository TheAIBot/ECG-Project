################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../avgCircularArray.c \
../benchmarks.c \
../circularArray.c \
../derSqrMwiFilter.c \
../filter.c \
../highPassFilter.c \
../inputManager.c \
../lowPassFilter.c \
../main.c \
../peakSearcher.c \
../rPeakFinder.c \
../rawData.c \
../tests.c \
../ui.c 

OBJS += \
./avgCircularArray.o \
./benchmarks.o \
./circularArray.o \
./derSqrMwiFilter.o \
./filter.o \
./highPassFilter.o \
./inputManager.o \
./lowPassFilter.o \
./main.o \
./peakSearcher.o \
./rPeakFinder.o \
./rawData.o \
./tests.o \
./ui.o 

C_DEPS += \
./avgCircularArray.d \
./benchmarks.d \
./circularArray.d \
./derSqrMwiFilter.d \
./filter.d \
./highPassFilter.d \
./inputManager.d \
./lowPassFilter.d \
./main.d \
./peakSearcher.d \
./rPeakFinder.d \
./rawData.d \
./tests.d \
./ui.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


