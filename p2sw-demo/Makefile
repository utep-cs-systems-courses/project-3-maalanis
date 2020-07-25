all: p2sw-demo.elf p2sw-demo.a

CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as
AR              = msp430-elf-ar


p2sw-demo.a: p2sw-demo.o lcdutils.o lcddraw.o font-11x16.o font-5x7.o font-8x12.o
	$(AR) crs $@ $^
lcddraw.o: lcddraw.c lcddraw.h lcdutils.h

lcdutils.o: lcdutils.c lcdutils.h

install: p2sw-demo.a
	mkdir-p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

p2sw-demo.elf: p2sw-demo.o p2sw-demo.a
	$(CC) $(CFLAGS) ${LDFLAGS} -o $@ $^ -lLcd -lp2sw -lTimer



clean:
	rm -f *.a *.o *.elf

load: p2sw-demo.elf
	msp430loader.sh $^

