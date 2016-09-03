Packages 		= \
	  			 Gnuplot \
	  			 WaveLib \
	  			 Communication \
	  			 VoicePCM \
	  			 VoiceCMD \
	  			 AutoFile \
	  			 MainFrame \

Empty                  =
Space                  = $(Empty) $(Empty)
temproot               = $(subst /Source,$(Space),$(CURDIR))
DEV_ROOT               = $(word 1,$(temproot))

ObjectFiles            = $(wildcard Object/*/*.o) 

Main        	= smarthome
SUDO        	= sudo

PackageListLoop        = $(patsubst %,Source/%/.loop,$(Packages))

makelinux:
	@echo "Start Make Linux"

LinkerOption    = -std=gnu99 -pthread -lm -lmad -ldl -lrt -lfftw3

%.loop:
	@$(MAKE) $(MakeOptions) -C $(subst .loop,,$@) -f Make package_$(MAKECMDGOALS)

ARMGCC = arm-tool/bin/arm-linux-gcc
Compiler  = $(ARMGCC)
Execute = @echo no run
ifneq ($(ARMGCC), $(wildcard $(ARMGCC)))
	Compiler  = gcc
	Execute = /home/laojing/arm11/$(Main)
endif

build: makelinux $(PackageListLoop)
	@echo "End Make Linux"
	@$(Compiler) -g -o /home/laojing/arm11/$(Main) $(DEV_ROOT)/Source/main.c $(ObjectFiles) $(LinkerOption)
	@$(Execute)

clean: $(PackageListLoop)
	@echo Donw Clean!

depend: $(PackageListLoop) 
	@echo Done Depend!

back:
	@$(MAKE) $(MakeOptions) -C Source/Cgi -f Make package_build
