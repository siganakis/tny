################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/tnyDB_list.c \
../src/tnyDB_mem.c \
../src/tnyDB_tarray.c \
../src/tnyDB_tmap.c \
../src/tnyDB_tword.c \
../src/tnyDB_wtree.c 

OBJS += \
./src/main.o \
./src/tnyDB_list.o \
./src/tnyDB_mem.o \
./src/tnyDB_tarray.o \
./src/tnyDB_tmap.o \
./src/tnyDB_tword.o \
./src/tnyDB_wtree.o 

C_DEPS += \
./src/main.d \
./src/tnyDB_list.d \
./src/tnyDB_mem.d \
./src/tnyDB_tarray.d \
./src/tnyDB_tmap.d \
./src/tnyDB_tword.d \
./src/tnyDB_wtree.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


