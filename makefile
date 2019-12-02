CC = gcc
DEBUG_FLAG =
OBJS = matamazom.o matamazom_print.o matamazom_main.o matamazom_tests.o amount_set.o
EXEC = matamazom
LIBS = -L. -lm -lmtm
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors

# build matamazom using dependencies
$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAGS) $(CFLAGS) $(LIBS) $(OBJS) -o $@

# build example amount_set program with tests using dependencies
amount_set: amount_set.o amount_set_main.o amount_set_tests.o
	$(CC) $(DEBUG_FLAGS) $(CFLAGS) amount_set.o amount_set_main.o amount_set_tests.o -o $@

# build matamazom tests runner library
matamazom_main.o: tests/matamazom_tests.h tests/matamazom_tests.c matamazom_tests.o matamazom.o
	$(CC) -c $(DEBUG_FLAGS) $(CFLAGS) tests/$*.c

# build matamazom tests library
matamazom_tests.o: tests/matamazom_tests.c tests/matamazom_tests.h tests/test_utilities.h matamazom.o
	$(CC) -c $(DEBUG_FLAGS) $(CFLAGS) tests/$*.c

# build amount_set tests runner library
amount_set_main.o: tests/amount_set_main.c amount_set_tests.o amount_set.o
	$(CC) -c $(DEBUG_FLAGS) $(CFLAGS) tests/$*.c

# build amount_set tests library file
amount_set_tests.o: tests/amount_set_tests.c tests/amount_set_tests.h amount_set.o
	$(CC) -c $(DEBUG_FLAGS) $(CFLAGS) tests/$*.c

# build matamazom library file
matamazom.o: matamazom.c matamazom.h amount_set.o matamazom_print.o
	$(CC) -c $(DEBUG_FLAGS) $(CFLAGS) $*.c

# build amount_set library file
amount_set.o: amount_set.c amount_set.h
	$(CC) -c $(DEUBG_FLAGS) $(CFLAGS) $*.c

# build matamazom_print library file
matamazom_print.o: matamazom_print.c matamazom_print.h
	$(CC) -c $(DEUBG_FLAGS) $(CFLAGS) $*.c

# clean working directory
clean:
	rm -f amount_set amount_set_main.o amount_set_tests.o $(OBJS) $(EXEC)
