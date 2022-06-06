###########################################################################
##                          Makefile for tester                          ##
###########################################################################

CXX      = clang++
CXXFLAGS = -c -g3 -std=c++11 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 -pthread
STRENV   = stream/
STRENLNV = stream/liners/

tester: tester.cpp stream/CodeStream.o stream/liners/Liner.o
	${CXX} ${LDFLAGS} -o tester tester.cpp stream/CodeStream.o \
	stream/liners/Liner.o

${STRENV}/CodeStream.o: stream/*.h stream/liners/Liner.h
	${CXX} ${CXXFLAGS} -c stream/CodeStream.cpp

${STRENLNV}/Liner.o: stream/*.h stream/liners/*.h
	${CXX} ${CXXFLAGS} -c stream/liners/Liner.cpp

clean:
	@rm -rf *.o *~ *.gch
	@rm -rf ${STRENV}*.o ${STRENV}*~ ${STRENV}*.gch
	@rm -rf ${STRENLNV}*.o ${STRENLNV}*~ ${STRENLNV}*.gch


	