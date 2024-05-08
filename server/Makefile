# MinecraftServerC/server Makefile Usage: make [usage/build/run/clean/fresh-build/fresh-run/docs-gen]
.PHONY: usage build run clean fresh-build fresh-run docs-gen

# Utility Programs
P_ECHO?=echo
P_MKDIR?=mkdir -p
P_RM?=rm -f
P_RM_DIR?=$(P_RM) -r
P_MV?=mv -f

# Compiler / Linker Programs
P_CC?=gcc
P_CPP?=g++
P_LD?=g++

# Doxygen Generation
_USE_DOXYGEN?=true
P_DOXYGEN?=~/doxygen-1.10.0/bin/doxygen
P_PYTHON3?=python3
P_BROWSER?=

# Dependancy Programs
_DEPS_USEDEPS?=true
P_DEPS_CC?=$(P_CC)
P_DEPS_CPP?=$(P_CPP)

# Project Directory Structure
DIR_DEPS?=./deps
DIR_BUILD?=./build
DIR_SOURCE?=./source
DIR_INCLUDE?=./include

# Source Files / Target Output
TARGET?=a.out
SRC_CCFILES:=$(shell find $(DIR_SOURCE) -name '*.c')
SRC_CPPFILES:=$(shell find $(DIR_SOURCE) -name '*.cpp')

# Target Run CommandLine
TARGET_RUNTIME_MODE?=release #[debug/release]
TARGET_CMDLINE_DEBUG?=
TARGET_CMDLINE_RELEASE?=

# Debug/NDebug Check
ifeq ($(TARGET_RUNTIME_MODE),debug)
DEBUG_MODE_DEFINES:=-D_DEBUG
DIR_BUILD_INFIX:=debug
else
DEBUG_MODE_DEFINES:=-DNDEBUG
DIR_BUILD_INFIX:=release
endif

# Naming Convention
EXT_CC_OBJ?=.c.obj
EXT_CPP_OBJ?=.cpp.obj
EXT_DEP_OBJ?=.d

# Compiler Flags
_CFLAGS:=$(DEBUG_MODE_DEFINES) -Wall -pedantic -I$(DIR_INCLUDE)
CCFLAGS:=$(_CFLAGS) -std=c2x
CPPFLAGS:=$(_CFLAGS) -std=c++23
LDFLAGS:=

# Object Files
OBJ_CCFILES:=$(SRC_CCFILES:$(DIR_SOURCE)/%.c=$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.c.obj)
OBJ_CPPFILES:=$(SRC_CPPFILES:$(DIR_SOURCE)/%.cpp=$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.cpp.obj)
OBJ_ALL:=$(OBJ_CCFILES) $(OBJ_CPPFILES)

# Makefile Commands (User Callable)
usage:
	@$(P_ECHO) "Usage: \"make [usage/build/run/clean/fresh-build/fresh-run]\""
	@$(P_ECHO) "   usage       : Displays this message."
	@$(P_ECHO) "   build       : Builds the project, won't build any files that haven't changed since last build."
	@$(P_ECHO) "   run         : Uses the build command to build the project before running the built executable."
	@$(P_ECHO) "   clean       : Cleans the entire project structure of any binary/object files and deletes the executable."
	@$(P_ECHO) "   fresh-build : Uses the clean command before using the build command."
	@$(P_ECHO) "   fresh-run   : Uses the clean command before using the run command."
	@$(P_ECHO) ""
	@$(P_ECHO) "Flags:"
	@$(P_ECHO) "   _DEPS_USEDEPS        : Enables(true)/Disables(false) the usage of dependancy files, \".d\", to improve recompile process."
	@$(P_ECHO) "   TARGET_RUNTIME_MODE  : Determines if the project will be built for Release(release) or Debug(debug) runtime environment."
	@$(P_ECHO) ""
	@$(P_ECHO) ""

.PHONY: prebuild
prebuild:
	@$(P_MKDIR) $(DIR_BUILD)/$(DIR_BUILD_INFIX)
ifeq ($(_DEPS_USEDEPS),true)
	@$(P_MKDIR) $(DIR_DEPS)/$(DIR_BUILD_INFIX)
else
	@$(P_ECHO) "WARN: Building without dependancy files, this will change makefile to rebuild if any header files are modified. This can cause a whole project rebuild to occur at the slightest change, it's recommended that you enable \"_DEPS_USEDEPS\"."
endif

build: prebuild $(DIR_BUILD)/$(DIR_BUILD_INFIX)/$(TARGET)

run: build
ifeq ($(TARGET_RUNTIME_MODE),debug)
	$(DIR_BUILD)/$(DIR_BUILD_INFIX)/$(TARGET) $(TARGET_CMDLINE_DEBUG)
else
	$(DIR_BUILD)/$(DIR_BUILD_INFIX)/$(TARGET) $(TARGET_CMDLINE_RELEASE)
endif

clean:
	@$(P_ECHO) "Cleaning up executable..."
	@$(P_RM) $(TARGET)
	@$(P_ECHO) "Cleaning up build/deps directories, if any..."
	@$(P_RM_DIR) $(DIR_BUILD)
	@$(P_RM_DIR) $(DIR_DEPS)
	@$(P_ECHO) "Cleaning has finished..."

fresh-build: clean build

fresh-run: clean run

docs-gen:
ifeq ($(_USE_DOXYGEN),true)
	@$(P_DOXYGEN) ./doxygen.cfg
ifneq ($(_WEBBROWSER),false)
ifeq ($(P_BROWSER),)
	@$(P_PYTHON3) -m webbrowser "./docs/doxygen/html/index.html"
else
	@$(P_BROWSER) "./docs/doxygen/html/index.html"
endif
endif
else
	@$(P_ECHO) "Doxygen is not enabled, cannot generate documentation."
endif

# Makefile Commands (INTERNAL DON'T CALL USING MAKE)
$(DIR_BUILD)/$(DIR_BUILD_INFIX)/$(TARGET): $(OBJ_ALL)
	@$(P_MKDIR) $(dir $@)
	@$(P_LD) $(LDFLAGS) $^ -o $@

ifneq ($(_DEPS_USEDEPS),true) # Using Poor Man's Dependencies
$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.c.obj: $(DIR_SOURCE)/%.c $(DIR_INCLUDE)
	@$(P_MKDIR) $(dir $@)
	$(P_CC) $(CCFLAGS) -c $< -o $@

$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.cpp.obj: $(DIR_SOURCE)/%.cpp $(DIR_INCLUDE)
	@$(P_MKDIR) $(dir $@)
	$(P_CPP) $(CPPFLAGS) -c $< -o $@
else # Using Dependencies

# Dependency Files / Flags / Targets / etc...
DEPS_CC:=$(SRC_CFILES:$(DIR_SOURCE)/%.c=$(DIR_BUILD)/%.c.d)
DEPS_CPP:=$(SRC_CPPFILES:$(DIR_SOURCE)/%.cpp=$(DIR_BUILD)/%.cpp.d)
DEPS_ALL:=$(DEPS_CC) $(DEPS_CPP)
DEPS_TARGET:=$(DIR_DEPS)/$(DIR_BUILD_INFIX)/%.d
DEPS_FLAGS= -MT $@ -MMD -MP -MF $(DIR_DEPS)/$(DIR_BUILD_INFIX)/$*.Td
DEPS_POSTCOMPILE= $(P_MV) $(DIR_DEPS)/$(DIR_BUILD_INFIX)/$*.Td $(DIR_DEPS)/$(DIR_BUILD_INFIX)/$*.d

$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.c.obj: $(DIR_SOURCE)/%.c
$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.c.obj: $(DIR_SOURCE)/%.c $(DEPS_TARGET)
	@$(P_MKDIR) $(dir $@)
	@$(P_MKDIR) $(dir $(DIR_DEPS)/$(DIR_BUILD_INFIX)/$*.Td)
	$(P_DEPS_CC) $(DEPS_FLAGS) $(CCFLAGS) -c $< -o $@
	@$(DEPS_POSTCOMPILE)


$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.cpp.obj: $(DIR_SOURCE)/%.cpp
$(DIR_BUILD)/$(DIR_BUILD_INFIX)/%.cpp.obj: $(DIR_SOURCE)/%.cpp $(DEPS_TARGET)
	@$(P_MKDIR) $(dir $@)
	@$(P_MKDIR) $(dir $(DIR_DEPS)/$(DIR_BUILD_INFIX)/$*.Td)
	$(P_DEPS_CPP) $(DEPS_FLAGS) $(CPPFLAGS) -c $< -o $@
	@$(DEPS_POSTCOMPILE)

$(DIR_DEPS)/$(DIR_BUILD_INFIX)/%.d: ;
.PRECIOUS: $(DIR_DEPS)/$(DIR_BUILD_INFIX)/%.d

-include $(DEPS_ALL)
endif