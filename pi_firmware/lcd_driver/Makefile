CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0
LIBS = -pthread -larmbianio
all: sangria_lcd

sangria_lcd: sangria_lcd.o sangria_effects.o sharp_memory_display_driver.o fb_convert.o
	$(CC) sangria_lcd.o sangria_effects.o sharp_memory_display_driver.o fb_convert.o $(LIBS) -o sangria_lcd

startup_logo.txt: startup_logo.png
	./startup_logo.py

sangria_effects.o: sangria_effects.c sangria_effects.h startup_logo.txt sharp_memory_display_driver.h
	$(CC) $(CFLAGS) sangria_effects.c

sharp_memory_display_driver.o: sharp_memory_display_driver.c sharp_memory_display_driver.h
	$(CC) $(CFLAGS) sharp_memory_display_driver.c

fb_convert.o: fb_convert.c fb_convert.h
	$(CC) $(CFLAGS) fb_convert.c

sangria_lcd.o: sangria_lcd.c sangria_effects.h sharp_memory_display_driver.h fb_convert.h
	$(CC) $(CFLAGS) sangria_lcd.c

clean:
	rm -rf *.o sangria_lcd

install: sangria_lcd
	echo "Install sangria_lcd service."
	sudo systemctl stop sangria_lcd.service
	sudo cp ./sangria_lcd /usr/local/bin/
	sudo cp ./sangria_lcd.service /etc/systemd/system/
	sudo chmod 644 /etc/systemd/system/sangria_lcd.service
	sudo systemctl enable sangria_lcd.service
	sudo systemctl start sangria_lcd.service
