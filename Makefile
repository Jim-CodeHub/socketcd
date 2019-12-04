#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for socketcd source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------


TARGET				=	socketcd
PROJECT				=   lib$(TARGET)

CXX					=	g++

CXXFLAGS			=	-Werror -std=c++11
CXXFLAGS   	       += 	-Wall
CXXFLAGS   	       += 	-lpthread
#CXXFLAGS			+=  -g

SUBDIRS 			=   src/server src/client

CPLUS_INCLUDE_PATH = $(shell dirname `pwd`)

export CXX CXXFLAGS CPLUS_INCLUDE_PATH


#-------------------------------------------------------------------------------------------------------
#																									   #
#											    Make rules 								   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean install $(SUBDIRS) tst

all:$(SUBDIRS)
	ar -rcs $(PROJECT).a $(shell find ./src -name "*.o")
	$(CXX) -fPIC -shared $(shell find ./src -name "*.cpp") -o $(PROJECT).so

$(SUBDIRS):
	$(MAKE) -C $@	

tst:
	$(MAKE) -C tst

install:
	@make
	$(shell if [ ! -d $(TARGET) ]; then `mkdir $(TARGET)`; fi;)
	$(shell cp -rf src/* ./$(TARGET)/ )
	@rm -rf `find ./$(TARGET) -name "*.o"`
	@rm -rf `find ./$(TARGET) -name "*.cpp"`
	@rm -rf `find ./$(TARGET) -name "Makefile"`
	@mv ./$(PROJECT).a ./$(PROJECT).so $(TARGET) 

tags:
	@rm -rf ./tags
	ctags --exclude="docs"			\
		  --exclude="*.o"			\
		  --exclude="*.a"			\
		  --exclude="*.so"			\
		  --exclude="*.elf"			\
		  --exclude="*.map"			\
		  --exclude="*.lss" -R .

clean:
	for dir in $(SUBDIRS); do 		\
		$(MAKE) -C $$dir clean;		\
	done
	@rm -rf $(shell find ./ -name "*.o")

