# Makefile for Creative Labs EMU10K1 
#
# 12 Apr 2000 Rui Sousa

O_TARGET := audigy.o

obj-y := audio.o cardmi.o cardmo.o cardwi.o cardwo.o efxmgr.o emuadxmg.o \
	hwaccess.o irqmgr.o main.o midi.o mixer.o recmgr.o timer.o voicemgr.o \
	ecard.o passthrough.o

obj-m := $(O_TARGET)

ifdef DEBUG
    EXTRA_CFLAGS += -DEMU10K1_DEBUG
endif

ifdef CONFIG_MIDI_EMU10K1
    EXTRA_CFLAGS += -DEMU10K1_SEQUENCER
endif

include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s
