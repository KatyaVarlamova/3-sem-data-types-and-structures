CC=gcc
INCPATH=./inc/
SRCPATH=./src/
OBJPATH=./out/
INC=-I$(INCPATH)
CFLAGS=-std=c99 $(INC) -g3 -Wfloat-equal -Wvla --coverage
LFLAGS=--coverage
LCHECKFLAGS=-lcheck -lpthread
APPOBJS=$(OBJPATH)main.o $(OBJPATH)ordinary_matrix.o $(OBJPATH)sparse_matrix.o $(OBJPATH)sparse_vector.o $(OBJPATH)vector.o
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
$(OBJPATH)ordinary_matrix.o: $(SRCPATH)ordinary_matrix.c $(INCPATH)ordinary_matrix.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)sparse_matrix.o: $(SRCPATH)sparse_matrix.c $(INCPATH)sparse_matrix.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)main.o: $(SRCPATH)main.c
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)sparse_vector.o: $(SRCPATH)sparse_vector.c $(INCPATH)sparse_vector.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)vector.o: $(SRCPATH)vector.c $(INCPATH)vector.h
	$(CC) $(CFLAGS) -o$@ -c $<

	
clean: 
	rm -fv *.exe $(OBJPATH)*.c.gcov $(OBJPATH)*.o $(OBJPATH)*.gcno $(OBJPATH)*.gcda $(OBJPATH)*.lastbuildstate