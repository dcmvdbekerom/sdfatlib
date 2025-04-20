PREFIX ?= arm-none-eabi
#CFLAGS += -mcpu=cortex-m3 -mthumb -Wno-address-of-packed-member 
CFLAGS += -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Wno-address-of-packed-member

#LDFLAGS += -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float 
LDFLAGS += -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -march=armv7e-m -mfix-cortex-m3-ldrd



export PREFIX
export CFLAGS
export LDFLAGS

.PHONY: all doc FORCE 

all:
	$(MAKE) -C src

doc:
	doxygen Doxyfile

%: FORCE
	$(MAKE) -C src $@
