TARGET = doolhof
OBJS = main.o \
gfx.o \
glib2d.o
 
CFLAGS = -G0 -Wall -g
CXXFLAGS = $(CFLAGS) -fno-rtti  -std=c++14 -fno-exceptions
ASFLAGS = $(CFLAGS)

LIBS = -lstdc++ -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lm -lpspvram

# psp stuff
BUILD_PRX = 1
PSP_FW_VERSION = 500
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = doolhof, de game!
PSP_EBOOT_ICON = icon0.png
PSP_EBOOT_SND0 = SND0.at3

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
install:
	cp ./EBOOT.PBP $(MEDIA)$(TARGET)
dir:
	mkdir $(MEDIA)$(TARGET)
lclean:
	rm *.o