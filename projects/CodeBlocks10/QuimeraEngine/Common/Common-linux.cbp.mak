#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.137                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

#DebugLinux32SharedrtStatic
INC_DEBUGLINUX32SHAREDRTSTATIC =  -I$(MAKEPATH)trunk/headers -I$(MAKEPATH)trunk/3rdparty/Boost/include
CFLAGS_DEBUGLINUX32SHAREDRTSTATIC =  -Wshadow -Wredundant-decls -Wcast-align -Winline -Wmissing-include-dirs -Wswitch-default -Wall -g -m32 -D_DEBUG
RESINC_DEBUGLINUX32SHAREDRTSTATIC =  
RCFLAGS_DEBUGLINUX32SHAREDRTSTATIC =  
LIBDIR_DEBUGLINUX32SHAREDRTSTATIC =  
LIB_DEBUGLINUX32SHAREDRTSTATIC = 
LDFLAGS_DEBUGLINUX32SHAREDRTSTATIC =  
OBJDIR_DEBUGLINUX32SHAREDRTSTATIC = $(MAKEPATH)trunk/garbage/CodeBlocks10/DebugLinux32SharedrtStatic
DEP_DEBUGLINUX32SHAREDRTSTATIC = 
OUT_DEBUGLINUX32SHAREDRTSTATIC = $(MAKEPATH)trunk/bin/CodeBlocks10/DebugLinux32SharedrtStatic/libQuimeraEngineCommon.a
OBJ_DEBUGLINUX32SHAREDRTSTATIC = $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/Assertions.o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQVF32.o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQFloat.o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQBoolean.o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQInteger.o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/QAlignment.o

#DebugLinux32SharedrtDynamic
INC_DEBUGLINUX32SHAREDRTDYNAMIC =  -I$(MAKEPATH)trunk/headers -I$(MAKEPATH)trunk/3rdparty/Boost/include
CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC =  -Wshadow -Wredundant-decls -Wcast-align -Winline -Wmissing-include-dirs -Wswitch-default -Wall -g -m32 -D_DEBUG -DQE_PREPROCESSOR_EXPORTLIB_COMMON
RESINC_DEBUGLINUX32SHAREDRTDYNAMIC =  
RCFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC =  
LIBDIR_DEBUGLINUX32SHAREDRTDYNAMIC =  
LIB_DEBUGLINUX32SHAREDRTDYNAMIC = 
LDFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC =  
OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC = $(MAKEPATH)trunk/garbage/CodeBlocks10/DebugLinux32SharedrtDynamic
DEP_DEBUGLINUX32SHAREDRTDYNAMIC = 
OUT_DEBUGLINUX32SHAREDRTDYNAMIC = $(MAKEPATH)trunk/bin/CodeBlocks10/DebugLinux32SharedrtDynamic/libQuimeraEngineCommon.so
OBJ_DEBUGLINUX32SHAREDRTDYNAMIC = $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/Assertions.o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQVF32.o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQFloat.o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQBoolean.o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQInteger.o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/QAlignment.o 

all: debuglinux32sharedrtstatic debuglinux32sharedrtdynamic

clean: clean_debuglinux32sharedrtstatic clean_debuglinux32sharedrtdynamic

#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
#  DebugLinux32SharedrtStatic ~~~  DebugLinux32SharedrtStatic  ~~~  DebugLinux32SharedrtStatic  ~~~  DebugLinux32SharedrtStatic  ~~~  DebugLinux32SharedrtStatic  ~~~        #  
#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#

before_debuglinux32sharedrtstatic: 
	test -d $(MAKEPATH)trunk/bin/CodeBlocks10/DebugLinux32SharedrtStatic || mkdir -p $(MAKEPATH)trunk/bin/CodeBlocks10/DebugLinux32SharedrtStatic
	test -d $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source || mkdir -p $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source

after_debuglinux32sharedrtstatic: 

debuglinux32sharedrtstatic: before_debuglinux32sharedrtstatic out_debuglinux32sharedrtstatic after_debuglinux32sharedrtstatic

out_debuglinux32sharedrtstatic: $(OBJ_DEBUGLINUX32SHAREDRTSTATIC) $(DEP_DEBUGLINUX32SHAREDRTSTATIC)
	$(AR) rcs $(OUT_DEBUGLINUX32SHAREDRTSTATIC) $(OBJ_DEBUGLINUX32SHAREDRTSTATIC)

$(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/Assertions.o: $(MAKEPATH)trunk/source/Assertions.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTSTATIC) $(INC_DEBUGLINUX32SHAREDRTSTATIC) -c $(MAKEPATH)trunk/source/Assertions.cpp -o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/Assertions.o

$(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQInteger.o: $(MAKEPATH)trunk/source/SQInteger.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTSTATIC) $(INC_DEBUGLINUX32SHAREDRTSTATIC) -c $(MAKEPATH)trunk/source/SQInteger.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQInteger.o

$(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQVF32.o: $(MAKEPATH)trunk/source/SQVF32.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTSTATIC) $(INC_DEBUGLINUX32SHAREDRTSTATIC) -c $(MAKEPATH)trunk/source/SQVF32.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQVF32.o

$(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQBoolean.o: $(MAKEPATH)trunk/source/SQBoolean.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTSTATIC) $(INC_DEBUGLINUX32SHAREDRTSTATIC) -c $(MAKEPATH)trunk/source/SQBoolean.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQBoolean.o

$(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQFloat.o: $(MAKEPATH)trunk/source/SQFloat.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTSTATIC) $(INC_DEBUGLINUX32SHAREDRTSTATIC) -c $(MAKEPATH)trunk/source/SQFloat.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/SQFloat.o

$(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/QAlignment.o: $(MAKEPATH)trunk/source/QAlignment.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTSTATIC) $(INC_DEBUGLINUX32SHAREDRTSTATIC) -c $(MAKEPATH)trunk/source/QAlignment.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTSTATIC)$(MAKEPATH__)trunk/source/QAlignment.o
	
clean_debuglinux32sharedrtstatic: 
	rm -f $(OBJ_DEBUGLINUX32SHAREDRTSTATIC) $(OUT_DEBUGLINUX32SHAREDRTSTATIC)

#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
#  DebugLinux32SharedrtDynamic  ~~~  DebugLinux32SharedrtDynamic  ~~~  DebugLinux32SharedrtDynamic  ~~~  DebugLinux32SharedrtDynamic  ~~~  DebugLinux32SharedrtDynamic  ~~~  #  
#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#

before_debuglinux32sharedrtdynamic: 
	test -d $(MAKEPATH)trunk/bin/CodeBlocks10/DebugLinux32SharedrtDynamic || mkdir -p $(MAKEPATH)trunk/bin/CodeBlocks10/DebugLinux32SharedrtDynamic
	test -d $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source || mkdir -p $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source

after_debuglinux32sharedrtdynamic: 

debuglinux32sharedrtdynamic: before_debuglinux32sharedrtdynamic out_debuglinux32sharedrtdynamic after_debuglinux32sharedrtdynamic

out_debuglinux32sharedrtdynamic: $(OBJ_DEBUGLINUX32SHAREDRTDYNAMIC) $(DEP_DEBUGLINUX32SHAREDRTDYNAMIC)
	$(LD) -shared $(LDFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(LIBDIR_DEBUGLINUX32SHAREDRTDYNAMIC) $(OBJ_DEBUGLINUX32SHAREDRTDYNAMIC) $(LIB_DEBUGLINUX32SHAREDRTDYNAMIC) -Wl,-soname,libQuimeraEngineCommon.so -o $(OUT_DEBUGLINUX32SHAREDRTDYNAMIC)

$(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/Assertions.o: $(MAKEPATH)trunk/source/Assertions.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(INC_DEBUGLINUX32SHAREDRTDYNAMIC) -c $(MAKEPATH)trunk/source/Assertions.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/Assertions.o

$(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQInteger.o: $(MAKEPATH)trunk/source/SQInteger.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(INC_DEBUGLINUX32SHAREDRTDYNAMIC) -c $(MAKEPATH)trunk/source/SQInteger.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQInteger.o

$(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQVF32.o: $(MAKEPATH)trunk/source/SQVF32.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(INC_DEBUGLINUX32SHAREDRTDYNAMIC) -c $(MAKEPATH)trunk/source/SQVF32.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQVF32.o

$(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQBoolean.o: $(MAKEPATH)trunk/source/SQBoolean.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(INC_DEBUGLINUX32SHAREDRTDYNAMIC) -c $(MAKEPATH)trunk/source/SQBoolean.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQBoolean.o

$(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQFloat.o: $(MAKEPATH)trunk/source/SQFloat.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(INC_DEBUGLINUX32SHAREDRTDYNAMIC) -c $(MAKEPATH)trunk/source/SQFloat.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/SQFloat.o

$(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/QAlignment.o: $(MAKEPATH)trunk/source/QAlignment.cpp
	$(CXX) $(CFLAGS_DEBUGLINUX32SHAREDRTDYNAMIC) $(INC_DEBUGLINUX32SHAREDRTDYNAMIC) -c $(MAKEPATH)trunk/source/QAlignment.cpp  -o $(OBJDIR_DEBUGLINUX32SHAREDRTDYNAMIC)$(MAKEPATH__)trunk/source/QAlignment.o
	
clean_debuglinux32sharedrtdynamic: 
	rm -f $(OBJ_DEBUGLINUX32SHAREDRTDYNAMIC) $(OUT_DEBUGLINUX32SHAREDRTDYNAMIC)


.PHONY: before_debuglinux32sharedrtstatic after_debuglinux32sharedrtstatic clean_debuglinux32sharedrtstatic before_debuglinux32sharedrtdynamic after_debuglinux32sharedrtdynamic clean_debuglinux32sharedrtdynamic

