# Компилятор и основные флаги
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Определяем систему (macOS или Linux) и добавляем соответствующие пути и библиотеки
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    CHECK_LIBS = -lcheck
    LCOV_FLAGS = --ignore-errors inconsistent,corrupt
    INCLUDE_FLAGS = -I/opt/homebrew/include
    LIB_FLAGS = -L/opt/homebrew/lib
else
    CHECK_LIBS = -lcheck -lm 
    LCOV_FLAGS =
    INCLUDE_FLAGS = -I/usr/local/include
    LIB_FLAGS = -L/usr/local/lib
endif

BUILD_DIR = .

# Цель по умолчанию
all: s21_matrix.a 

# Сборка объекта для библиотеки
s21_matrix.o: s21_matrix.c s21_matrix.h
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c s21_matrix.c -o s21_matrix.o

# Создание статической библиотеки
s21_matrix.a: s21_matrix.o
	ar rc s21_matrix.a s21_matrix.o
	ranlib s21_matrix.a

# Сборка объекта для тестов
test_s21_matrix.o: test.c s21_matrix.h
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(INCLUDE_FLAGS) -c test.c -o test_s21_matrix.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(INCLUDE_FLAGS) -c s21_matrix.c -o s21_matrix.o

# Сборка и линковка тестов
s21_matrix_tests: test_s21_matrix.o s21_matrix.a
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o s21_matrix_tests test_s21_matrix.o s21_matrix.a $(LIB_FLAGS) $(CHECK_LIBS) 
	
# Запуск тестов
test: s21_matrix_tests
	./s21_matrix_tests
	
# Генерация отчета покрытия
gcov_report: test
	
	gcov -b -l -p -c s21_matrix.gcno
	gcovr --html --html-details -o gcov_report.html --exclude s21_matrix.c
	lcov ${LCOV_FLAGS} -o test.info -c -d $(BUILD_DIR)
	genhtml -o $(BUILD_DIR)/report test.info

# Очистка временных файлов
clean:
	rm -rf s21_matrix_tests *.gcda *.gcno *.info *.o *.a *.gcov *.html *.css report
