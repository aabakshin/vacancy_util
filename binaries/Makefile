CC = gcc
LIBS_PATH = ../libs
SRCMODULES = $(LIBS_PATH)/Additional.c $(LIBS_PATH)/DataList.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -Wall -g
INPUT_FILE = ../input_files/file.txt
OUTPUT_FILE = ../output_files/file.txt

%.o: $(LIBS_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $(LIBS_PATH)/$@

start: start.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

run: start
	./start $(INPUT_FILE) $(OUTPUT_FILE)

clean:
	rm -rf $(OBJMODULES) start
