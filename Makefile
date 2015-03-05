MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -std=c++11 # -Wextra -pedantic
CFLAGS        = -pipe -O2 -D_REENTRANT -Wall -W -fPIE $(DEFINES)
CXXFLAGS      = -pipe -O2 -D_REENTRANT -Wall -W -fPIE $(DEFINES)
INCPATH       = -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS) -lpthread 
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

SOURCES       = filereader.cpp	main.cpp	support.cpp		chebmore.cpp 	fileofflinereader.cpp 	stringop.cpp	samplegen.cpp	commandline.cpp	## TODO: add all .cpp files here
OBJECTS       = filereader.o	main.o		support.o		chebmore.o 		fileofflinereader.o 	stringop.o		samplegen.o		commandline.o	## TODO: add all corresponding .o files here
DIST          =
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = support


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

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

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) $(OBJECTS) $(OBJCOMP) $(LIBS) -o $(TARGET)

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

filereader.o: filereader.cpp filereader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o filereader.o filereader.cpp

main.o: main.cpp filereader.h support.h fileofflinereader.h samplegen.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

support.o: support.cpp support.h chebmore.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o support.o support.cpp

chebmore.o: chebmore.cpp chebmore.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o chebmore.o chebmore.cpp

fileofflinereader.o: fileofflinereader.cpp fileofflinereader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fileofflinereader.o fileofflinereader.cpp

stringop.o: stringop.cpp stringop.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stringop.o stringop.cpp

samplegen.o: samplegen.cpp samplegen.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o samplegen.o samplegen.cpp

commandline.o: commandline.cpp commandline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o commandline.o commandline.cpp
