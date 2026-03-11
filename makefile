CC = gcc
TARGET = app$(EXE)
SRC = main.c
OBJ = main.o

ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET)
