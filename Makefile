CC = gcc
CFLAGS = -g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic -Wno-unused-function
CFLAGS_GCOV = $(CFLAGS) -fprofile-arcs -ftest-coverage
BASE_NAME = huffman
SRC_C = huffman.c priority_queue.c frequencies.c bit_writer.c
TEST_C = test_huffman.c
ASG_NICKNAME = HW20
SRC_H = huffman.h priority_queue.h frequencies.h bit_writer.h clog.h miniunit.h
TEST_EXPECTED = expected.txt
SUBMIT_FILES = $(TEST_C) Makefile $(SRC_C) $(SRC_H)
TEST_ACTUAL = actual.txt
EXECUTABLE = test_huffman
EXECUTABLE_DBG = $(EXECUTABLE)_dbg
EXECUTABLE_GCOV = $(EXECUTABLE)_gcov
SHELL = /bin/bash

$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -o $(EXECUTABLE) $(SRC_C) $(TEST_C) $(CFLAGS)

test: $(EXECUTABLE) $(TEST_EXPECTED)
	@if diff -a -B <("./$(EXECUTABLE)") $(TEST_EXPECTED) &> /dev/null ; then \
		if [ -t 2 ] ; then \
			echo -e -n "\x1b[32m" >> /dev/stderr; \
		fi; \
		echo "Test passed: output of $(EXECUTABLE) matches $(TEST_EXPECTED)" >> /dev/stderr; \
		if [ -t 2 ] ; then \
			echo -e -n "\x1b[0m" >> /dev/stderr; \
		fi \
	else \
		if [ -t 2 ] ; then \
			echo -e -n "\x1b[31m" >> /dev/stderr; \
		fi; \
		echo "Test failed: output of $(EXECUTABLE) does NOT match $(TEST_EXPECTED)" >> /dev/stderr; \
		if [ -t 2 ] ; then \
			echo -e -n "\x1b[0m" >> /dev/stderr; \
		fi; \
	fi
	valgrind ./$(EXECUTABLE)

submit: clean $(EXECUTABLE)
	264submit $(ASG_NICKNAME) $(SUBMIT_FILES) test.txt one.txt test.bits compressed.bits

pretest: submit
	264test $(ASG_NICKNAME)

coverage: $(SRC_C) $(TEST_C) 
	$(CC) -o $(EXECUTABLE_GCOV) $(SRC_C) $(TEST_C) $(CFLAGS_GCOV) -DNDEBUG
	./$(EXECUTABLE_GCOV)
	gcov -f $(SRC_C)

debug: $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -o $(EXECUTABLE_DBG) $(SRC_C) $(TEST_C) $(CFLAGS) -DDEBUG

clean: 
	rm -f -v huffman.c.gcov huffman.gcda huffman.gcno frequencies.c.gcov frequencies.gcda frequencies.gcno priority_queue.c.gcov priority_queue.gcda priority_queue.gcno test_huffman.gcda test_huffman.gcno test_huffman.gcda test_huffman.gcno $(TEST_ACTUAL) $(EXECUTABLE) $(EXECUTABLE) $(EXECUTABLE_GCOV) $(EXECUTABLE_DBG)

.PHONY: submit test pretest coverage debug clean
