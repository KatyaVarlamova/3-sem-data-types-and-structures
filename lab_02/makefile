CC=gcc
FUNCTESTSPATH=./func_tests/
UNITTESTSPATH=./unit_tests/
INCPATH=./inc/
SRCPATH=./src/
OBJPATH=./out/
INC=-I$(INCPATH)
CFLAGS=-std=c99 $(INC) -g3 -Wall -Wfloat-equal -Wvla -Wextra -Werror --coverage
LFLAGS=--coverage
LCHECKFLAGS=-lcheck -lpthread
APPOBJS=$(OBJPATH)main.o $(OBJPATH)autos.o $(OBJPATH)auto.o $(OBJPATH)sort.o
.PHONY=clean debug release unit func
.NOTPARALLEL: release debug func unit app.exe unit_tests.exe

release: CFLAGS:=-std=c99 $(INC)
release: LFLAGS:=-o2
release: $(OBJPATH) $(OBJPATH)release.lastbuildstate app.exe

debug: $(OBJPATH) $(OBJPATH)debug.lastbuildstate app.exe

$(OBJPATH)debug.lastbuildstate:
	rm -fv *.exe $(OBJPATH)*.o $(OBJPATH)*.gcno $(OBJPATH)*.gcda $(OBJPATH)release.lastbuildstate
	touch $(OBJPATH)debug.lastbuildstate

$(OBJPATH)release.lastbuildstate:
	rm -fv *.exe $(OBJPATH)*.o $(OBJPATH)*.gcno $(OBJPATH)*.gcda $(OBJPATH)debug.lastbuildstate
	touch $(OBJPATH)release.lastbuildstate

app.exe: $(OBJPATH) $(APPOBJS)
	$(CC) $(LFLAGS) $(APPOBJS) -o $@
$(OBJPATH):
	mkdir $(OBJPATH)
$(OBJPATH)autos.o: $(SRCPATH)autos.c $(INCPATH)autos.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)sort.o: $(SRCPATH)sort.c $(INCPATH)sort.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)main.o: $(SRCPATH)main.c
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)auto.o: $(SRCPATH)auto.c $(INCPATH)auto.h
	$(CC) $(CFLAGS) -o$@ -c $<
	
func: $(OBJPATH) $(OBJPATH)debug.lastbuildstate app.exe
	cd $(FUNCTESTSPATH) && bash all_test.cmd && cd ..
	cd $(OBJPATH) && gcov sort.o main.o autos.o auto.o && cd ..
	
clean: 
	rm -fv *.exe $(OBJPATH)*.c.gcov $(OBJPATH)*.o $(OBJPATH)*.gcno $(OBJPATH)*.gcda $(OBJPATH)*.lastbuildstate