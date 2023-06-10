# Generic OpenMPI Makefile
# Ostermann - Mar 15, 2022

CC=mpicc -Wall -Werror -O2 -Wno-unused-result -Wno-nonnull


CSOURCES=${wildcard *.c}
TARGET=hash_cracker

all: ${TARGET}	

${TARGET}: ${CSOURCES}

test: ${TARGET}	
	-chmod a+rx test.*
	-rm -f test.*.*
	-./test.0
	-./test.1
	-./test.2
	-./test.3
	-./test.4
	-./test.5
	-./test.6
	-./test.7

clean:
	rm -f  *.o ${TARGET} test.*.*
