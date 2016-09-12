################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
<<<<<<< HEAD
../derivativeSquareFilter.c \
=======
../benchmarks.c \
../circularArray.c \
../derivativeSquareFilter.c \
../filter.c \
>>>>>>> d27204af3f1a45d7d4460bcf75c99bc94050ee17
../highPassFilter.c \
../inputManager.c \
../lowPassFilter.c \
../main.c \
../movingWindowFilter.c \
<<<<<<< HEAD
../peakSearcher.c \
../rawData.c 

OBJS += \
./derivativeSquareFilter.o \
=======
../rawData.c \
../tests.c 

OBJS += \
./benchmarks.o \
./circularArray.o \
./derivativeSquareFilter.o \
./filter.o \
>>>>>>> d27204af3f1a45d7d4460bcf75c99bc94050ee17
./highPassFilter.o \
./inputManager.o \
./lowPassFilter.o \
./main.o \
./movingWindowFilter.o \
<<<<<<< HEAD
./peakSearcher.o \
./rawData.o 

C_DEPS += \
./derivativeSquareFilter.d \
=======
./rawData.o \
./tests.o 

C_DEPS += \
./benchmarks.d \
./circularArray.d \
./derivativeSquareFilter.d \
./filter.d \
>>>>>>> d27204af3f1a45d7d4460bcf75c99bc94050ee17
./highPassFilter.d \
./inputManager.d \
./lowPassFilter.d \
./main.d \
./movingWindowFilter.d \
<<<<<<< HEAD
./peakSearcher.d \
./rawData.d 
=======
./rawData.d \
./tests.d 
>>>>>>> d27204af3f1a45d7d4460bcf75c99bc94050ee17


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
<<<<<<< HEAD
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
=======
	gcc -O3 -std=c99 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
>>>>>>> d27204af3f1a45d7d4460bcf75c99bc94050ee17
	@echo 'Finished building: $<'
	@echo ' '


