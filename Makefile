#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for libsocket source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------


PROJECT			=   socketcd

CXX				=	g++

CXXFLAGS		=	-Werror -std=c++11
CXXFLAGS       += 	-Wall
#CXXFLAGS		+=  -g

SUBDIRS 		=   src/server src/client

export CXX CXXFLAGS


#-------------------------------------------------------------------------------------------------------
#																									   #
#											    Make rules 								   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean install $(SUBDIRS)

all:$(SUBDIRS)
	ar -rcs $(PROJECT).a $(shell find ./ -name "*.o")

$(SUBDIRS):
	$(MAKE) -C $@	

install:clean
	mkdir socketcd
	cp $(shell find ./ -name "*.hpp") ./socketcd
	mv $(PROJECT).a ./socketcd
 
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
	@rm -rf socketcd 

