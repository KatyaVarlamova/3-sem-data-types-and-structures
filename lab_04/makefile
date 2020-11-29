CC=gcc
INCPATH=./inc/
SRCPATH=./src/
OBJPATH=./out/
INC=-I$(INCPATH)
CFLAGS=-std=c99 $(INC) -g3 -Wfloat-equal -Wvla --coverage
LFLAGS=--coverage
LCHECKFLAGS=-lcheck -lpthread
APPOBJS=$(OBJPATH)main.o $(OBJPATH)stack_on_array.o $(OBJPATH)stack_on_list.o $(OBJPATH)vector.o
.PHONY=clean debug release unit func
.NOTPARALLEL: release debug run app.exe unit_tests.exe

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
$(OBJPATH)stack_on_array.o: $(SRCPATH)stack_on_array.c $(INCPATH)stack_on_array.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)stack_on_list.o: $(SRCPATH)stack_on_list.c $(INCPATH)stack_on_list.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)main.o: $(SRCPATH)main.c
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)vector.o: $(SRCPATH)vector.c $(INCPATH)vector.h 
	$(CC) $(CFLAGS) -o$@ -c $<
run: 
	drmemory -batch -lib_blacklist "*" -- ./app.exe

clean: 
	rm -fv *.exe $(OBJPATH)*.c.gcov $(OBJPATH)*.o $(OBJPATH)*.gcno $(OBJPATH)*.gcda $(OBJPATH)*.lastbuildstate