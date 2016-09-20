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
../peak.c \
../peakCircularArray.c \
../peakSearcher.c \
../rPeakFinder.c \
../rawData.c \
../tests.c 

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
./peak.o \
./peakCircularArray.o \
./peakSearcher.o \
./rPeakFinder.o \
./rawData.o \
./tests.o 

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
./peak.d \
./peakCircularArray.d \
./peakSearcher.d \
./rPeakFinder.d \
./rawData.d \
./tests.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


