################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECGScanner.c \
../avgCircularArray.c \
../benchmarks.c \
../circularArray.c \
../derSqrMwiFilter.c \
../filter.c \
../highPassFilter.c \
../lowPassFilter.c \
../main.c \
../peak.c \
../peakCircularArray.c \
../peakSearcher.c \
../rPeakFinder.c \
../rawData.c \
../sensor.c \
../tests.c \
../ui.c 

OBJS += \
./ECGScanner.o \
./avgCircularArray.o \
./benchmarks.o \
./circularArray.o \
./derSqrMwiFilter.o \
./filter.o \
./highPassFilter.o \
./lowPassFilter.o \
./main.o \
./peak.o \
./peakCircularArray.o \
./peakSearcher.o \
./rPeakFinder.o \
./rawData.o \
./sensor.o \
./tests.o \
./ui.o 

C_DEPS += \
./ECGScanner.d \
./avgCircularArray.d \
./benchmarks.d \
./circularArray.d \
./derSqrMwiFilter.d \
./filter.d \
./highPassFilter.d \
./lowPassFilter.d \
./main.d \
./peak.d \
./peakCircularArray.d \
./peakSearcher.d \
./rPeakFinder.d \
./rawData.d \
./sensor.d \
./tests.d \
./ui.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


