
OBJS    = client server
SUBDIRS = 
NAMEDIR = $(shell dirname `pwd`)
 
 
#-------------------------------------------------------------------------------------------------------
#																									   #
#										  Make rules 									   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------


.PHONY: all clean $(SUBDIRS)

all:
	for i in $(OBJS);													   						 \
	do															    	   						 \
		$(CXX) $(CXXFLAGS) "$$i".cpp -L$(NAMEDIR) -lsocketcd -Wl,-rpath=$(NAMEDIR) -o "$$i".out; \
	done

.PHONY:clean
clean:
	rm -rf *.out


