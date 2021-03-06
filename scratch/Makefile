MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -std=c++14 # -Wextra -pedantic
CFLAGS        = -O2 -Wall $(DEFINES)
CXXFLAGS      = -O2 -Wall $(DEFINES)
INCPATH       = -I. -I../ -I../sample/
LINK          = g++
LFLAGS        = -Wl,-O1 
LIBS          = $(SUBLIBS) -L./../sample -lsample
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = 
OBJECTS       = $(patsubst %.o,../%.o,support.o mathmore.o commandline.o)	## dependent on .o files here
DIST          =
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = main	synthetic	readfile	readhist


first: all
####### Implicit rules

# .SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: $(TARGET)

main: $(OBJECTS) main.o
	$(LINK) $(LFLAGS) $(OBJECTS) main.o $(LIBS) -o main

synthetic: $(OBJECTS) synthetic.o
	$(LINK) $(LFLAGS) $(OBJECTS) synthetic.o $(LIBS) -o synthetic

readfile: $(OBJECTS) readfile.o
	$(LINK) $(LFLAGS) $(OBJECTS) readfile.o $(LIBS) -o readfile

readhist: $(OBJECTS) readhist.o
	$(LINK) $(LFLAGS) $(OBJECTS) readhist.o $(LIBS) -o readhist

# $(TARGET):  $(OBJECTS)  
# 	$(LINK) $(LFLAGS) $(OBJECTS) $(OBJCOMP) $(LIBS) -o $(TARGET)

dist: 
	@test -d .tmp/test1.0.0 || mkdir -p .tmp/test1.0.0
	$(COPY_FILE) --parents $(DIST) .tmp/test1.0.0/ && $(COPY_FILE) --parents hello.h .tmp/test1.0.0/ && $(COPY_FILE) --parents hello.cpp main.cpp .tmp/test1.0.0/ && (cd `dirname .tmp/test1.0.0` && $(TAR) test1.0.0.tar test1.0.0 && $(COMPRESS) test1.0.0.tar) && $(MOVE) `dirname .tmp/test1.0.0`/test1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/test1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)

distclean: clean 
	-$(DEL_FILE) $(TARGET) 

####### Sub-libraries

check: first

compiler_clean: 

####### Compile

# TODO: add all dependencies for the .o

