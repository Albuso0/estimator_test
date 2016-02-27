MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -std=c++14
CFLAGS        = -O2 -Wall $(DEFINES)
CXXFLAGS      = -O2 -Wall $(DEFINES)
INCPATH       = -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS) 
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
OBJECTS       = filereader.o	support.o	mathmore.o 	fileofflinereader.o 	fileofflinelinereader.o	samplegen.o	commandline.o	entropy.o	## TODO: add all corresponding .o files here
DIST          =
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = support


first: all
####### Implicit rules

# .SUFFIXES: .o .c .cpp .cc .cxx .C

# .cpp.o:
# 	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

# .cc.o:
# 	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

# .cxx.o:
# 	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

# .C.o:
# 	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

# .c.o:
# 	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: $(TARGET)

# entropy: entropy.o samplegen.o commandline.o mainentropy.o
# 	$(LINK) $(LFLAGS) entropy.o samplegen.o mainentropy.o commandline.o $(LIBS) -o entropy

support: $(OBJECTS) main.o
	$(LINK) $(LFLAGS) main.o $(OBJECTS) $(LIBS) -o support

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

filereader.o: filereader.cpp filereader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o filereader.o filereader.cpp

fileofflinereader.o: fileofflinereader.cpp fileofflinereader.h hister.h hister.tpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fileofflinereader.o fileofflinereader.cpp

fileofflinelinereader.o: fileofflinelinereader.cpp fileofflinelinereader.h hister.h hister.tpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fileofflinelinereader.o fileofflinelinereader.cpp

samplegen.o: samplegen.cpp samplegen.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o samplegen.o samplegen.cpp

histreader.o: histreader.cpp histreader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o histreader.o histreader.cpp




commandline.o: commandline.cpp commandline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o commandline.o commandline.cpp

support.o: support.cpp support.h mathmore.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o support.o support.cpp

mathmore.o: mathmore.cpp mathmore.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mathmore.o mathmore.cpp

entropy.o: entropy.cpp entropy.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o entropy.o entropy.cpp

main.o: main.cpp filereader.h support.h fileofflinereader.h samplegen.h	distvec.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainentropy.o: mainentropy.cpp entropy.h samplegen.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainentropy.o mainentropy.cpp
