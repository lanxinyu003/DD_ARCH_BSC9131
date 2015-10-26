################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 1
first : all
-include ../makefile.init

# 如果存在，此文件包含在生成文件和.args文件中使用的环境变量。
-include makefile.local

RM := "$(Gnu_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include rdb_source/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S1_DEPS)),)
-include $(S1_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
STARCORE_LIBRARY_OUTPUTS += \
../../bsp_psc9131rdb.elb \

STARCORE_LIBRARY_OUTPUTS_QUOTED += \
"..\..\bsp_psc9131rdb.elb" \

STARCORE_LIBRARY_OUTPUTS_OS_FORMAT += \
..\..\bsp_psc9131rdb.elb \


# All Target
all: ../../bsp_psc9131rdb.elb

# Tool invocations
../../bsp_psc9131rdb.elb: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #2 $@'
	@echo 'Invoking: StarCore Librarian Linker'
	"$(SCToolsInstallDirEnv)/sc100-ar" -r -c "..\..\bsp_psc9131rdb.elb" @"bsp_psc9131rdb.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(STARCORE_LIBRARY_OUTPUTS_QUOTED) "./*/*.eln"  $(CPP_DEPS_QUOTED) $(CC_DEPS_QUOTED) $(S1_DEPS_QUOTED) $(CXX_DEPS_QUOTED) $(LISTING_FILE_QUOTED) $(C_UPPER_DEPS_QUOTED) $(C++_DEPS_QUOTED) "./*/*.d"  $(RDB_SUPPORT)/bsp_psc9131rdb.elb
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
