CC=gcc
INCPATH=./inc/
SRCPATH=./src/
OBJPATH=./out/
VIZPATH=./viz/
INC=-I$(INCPATH)
CFLAGS=-std=c99 $(INC) -g3 -Wfloat-equal -Wvla --coverage
LFLAGS=--coverage
LCHECKFLAGS=-lcheck -lpthread
APPOBJS=$(OBJPATH)main.o $(OBJPATH)tree.o $(OBJPATH)element.o $(OBJPATH)balanced_tree.o $(OBJPATH)hash_table.o $(OBJPATH)list.o $(OBJPATH)file.o $(OBJPATH)utils.o
.PHONY=clean debug release
.NOTPARALLEL: release debug run app.exe unit_tests.exe

release: CFLAGS:=-std=c99 $(INC)
release: LFLAGS:=-o2
release: $(OBJPATH) $(VIZPATH) $(OBJPATH)release.lastbuildstate app.exe

debug: $(OBJPATH) $(VIZPATH) $(OBJPATH)debug.lastbuildstate app.exe

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
$(VIZPATH):
	mkdir $(VIZPATH)
$(OBJPATH)tree.o: $(SRCPATH)tree.c $(INCPATH)tree.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)balanced_tree.o: $(SRCPATH)balanced_tree.c $(INCPATH)balanced_tree.h
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)hash_table.o: $(SRCPATH)hash_table.c $(INCPATH)hash_table.h 
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)list.o: $(SRCPATH)list.c $(INCPATH)list.h 
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)main.o: $(SRCPATH)main.c
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)element.o: $(SRCPATH)element.c $(INCPATH)element.h 
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)file.o: $(SRCPATH)file.c $(INCPATH)file.h 
	$(CC) $(CFLAGS) -o$@ -c $<
$(OBJPATH)utils.o: $(SRCPATH)utils.c $(INCPATH)utils.h 
	$(CC) $(CFLAGS) -o$@ -c $<

run: 
	make
	valgrind --tool=memcheck --leak-check=full ./app.exe file.txt

clean: 
	rm -fv *.exe $(OBJPATH)*.c.gcov $(OBJPATH)*.o $(OBJPATH)*.gcno $(OBJPATH)*.gcda $(OBJPATH)*.lastbuildstate file.txt* $(VIZPATH)*
	rm -r *.exe.dSYM
