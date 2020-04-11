PATH := $(DEVKITARM)/bin:$(PATH)

# --- Project details -------------------------------------------------

PROJ    := crazy_road
TITLE   := $(PROJ)

# Setup Environment var LIBTONC to /C/tonc/code/tonclib or /opt/devkitpro/libtonc
INCLUDE  := -I$(LIBTONC)/include
LIBPATHS := -L$(LIBTONC)/lib

LIBS    := -ltonc

CROSS   ?= arm-none-eabi-
AS      := $(CROSS)gcc
CC      := $(CROSS)gcc
LD      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy

all: crazy_road.gba

background.c :
	grit background.png -gu16 -gB4 -mLs -ftc

background.o : background.c
	arm-none-eabi-gcc $(INCLUDE) -O2 -Wall -fno-strict-aliasing -mthumb-interwork -mthumb -c background.c -o background.o

sprite_sheet.c : sprite_sheet.png
	grit sprite_sheet.png -gu16 -gB4 -Mw 4 -Mh 4 -ftc

sprite_sheet.o : sprite_sheet.c
	arm-none-eabi-gcc $(INCLUDE) -O2 -Wall -fno-strict-aliasing -mthumb-interwork -mthumb -c sprite_sheet.c -o sprite_sheet.o

crazy_road.o : crazy_road.c background.c sprite_sheet.c
	arm-none-eabi-gcc $(INCLUDE) -O2 -Wall -fno-strict-aliasing -mthumb-interwork -mthumb -c crazy_road.c -o crazy_road.o

crazy_road.elf : crazy_road.o background.o sprite_sheet.o
	arm-none-eabi-gcc crazy_road.o background.o sprite_sheet.o -mthumb-interwork -mthumb -specs=gba.specs $(LIBPATHS) -ltonc -o crazy_road.elf

crazy_road.gba : crazy_road.elf
	arm-none-eabi-objcopy -v -O binary crazy_road.elf crazy_road.gba
	gbafix crazy_road.gba -tcrazy_road

clean :
	rm -f background.c
	rm -f background.o
	rm -f sprite_sheet.c
	rm -f sprite_sheet.o
	rm -f crazy_road.o
	rm -f crazy_road.elf
	rm -f crazy_road.gba
