RM := rm -rf

PROGNAME := assignment1

C_SRCS += \
MainProgram.c \
Reader.c \
TaskQueue.c \
Writer.c \
small_lib.c 

OBJS += \
MainProgram.o \
Reader.o \
TaskQueue.o \
Writer.o \
small_lib.o 

LIBS :=-lpthread
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(PROGNAME)

# Tool invocations
assignment1: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o"$(PROGNAME)" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) $(PROGNAME)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

