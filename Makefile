# MinecraftServerC Makefile Usage: make [usage/build/run/clean/doxygen]
.PHONY: usage build run clean doxygen

# Utility Programs
P_ECHO?=echo
P_CD  ?=cd

# Makefile Commands (User Callable)
usage:
	@$(P_ECHO) "Usage: \"make [usage/build/run/clean/doxygen]\""
	@$(P_ECHO) "   usage       : Displays this message."
	@$(P_ECHO) "   build       : Builds the project, uses dependency files \".d\" if \"_DEPS_USEDEPS\" is enabled to reduce rebuild overhead; otherwise we will alwaysrebuild if any include headers are changed."
	@$(P_ECHO) "   run         : Builds the project before running the resulting binary."
	@$(P_ECHO) "   clean       : Cleans the build directory, and deletes the existing project binary."
	@$(P_ECHO) "   doxygen     : Generates documentation for the source code using doxygen."
	@$(P_ECHO) ""
	@$(P_ECHO) "Flags:"
	@$(P_ECHO) "   _DEPS_USEDEPS        : Enables(true)/Disables(false) the usage of dependancy files, \".d\", to improve recompile process."
	@$(P_ECHO) ""
	@$(P_ECHO) ""

build:
	@$(P_CD) server && $(MAKE) --no-print-directory build

run: 
	@$(P_CD) server && $(MAKE) --no-print-directory run

clean: 
	@$(P_CD) server && $(MAKE) --no-print-directory clean

doxygen:
	@$(P_ECHO) "Unimplemented makefile command!"