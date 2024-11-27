# Имя компилятора
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -Iincludes

# Папки
INCLUDE_DIR = includes
SOURCE_DIR = source

# Исходные файлы
SRCS = s21_grep.c $(SOURCE_DIR)/grepflags.c
OBJS = $(SRCS:.c=.o)

# Целевой исполняемый файл
TARGET = s21_grep

# Правило сборки всего
all: $(TARGET)

# Правило сборки цели
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Правило для компиляции объектов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Файлы без конкретных целей
.PHONY: all clean
