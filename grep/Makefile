CC = gcc
CFLAGS = -Werror -Wextra -Wall -std=c11

GREP_C = $(wildcard *.c) $(wildcard ../common/*.c)
OBJ = $(GREP_C:.c=.o)

ifeq ($(OS),Windows_NT)
    LIBS = -lregex
else
    LIBS =
endif

s21_grep: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q /F *.o s21_grep.exe
	cmd /c "cd ..\common && del /Q /F *.o"
	cmd /c "cd ..\cat && del /Q /F *.o *.exe"
else
	rm -f *.o s21_grep ../common/*.o ../cat/*.o ../cat/s21_cat
endif
