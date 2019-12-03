#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for libsocket source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------


PROJECT				=   socketcd

CXX					=	g++

CXXFLAGS			=	-Werror -std=c++11
CXXFLAGS   	       += 	-Wall
#CXXFLAGS			+=  -g

SUBDIRS 			=   src/server src/client

CPLUS_INCLUDE_PATH = $(shell dirname `pwd`)

export CXX CXXFLAGS CPLUS_INCLUDE_PATH


#-------------------------------------------------------------------------------------------------------
#																									   #
#											    Make rules 								   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean install $(SUBDIRS)

all:$(SUBDIRS)
	ar -rcs $(PROJECT).a $(shell find ./src -name "*.o")
	$(CXX) -fPIC -shared $(shell find ./src -name "*.cpp") -o $(PROJECT).so

$(SUBDIRS):
	$(MAKE) -C $@	

install:
	@make
	@mkdir socketcd
	@mkdir socketcd/include
	@mkdir socketcd/lib
	@rm -rf socketcd/client/Makefile
	@rm -rf socketcd/client/socketc.cpp
	@rm -rf socketcd/server/Makefile
	@rm -rf socketcd/server/socketd.cpp
	@mv ./socketcd.a socketcd/lib
	@mv ./socketcd.so socketcd/lib

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

