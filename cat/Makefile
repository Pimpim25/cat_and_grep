CC = gcc
CFLAGS = -Werror -Wextra -Wall -std=c11

CAT_C= $(wildcard *.c) $(wildcard ../common/*.c)
OBJ= $(CAT_C:.c=.o)

cat: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q /F *.o cat.exe
	cmd /c "cd ..\common && del /Q /F *.o"
	cmd /c "cd ..\grep && del /Q /F *.o *.exe"
else
	rm -rf *.o cat ../common/*.o ../grep/*.o ../grep/grep
endif
