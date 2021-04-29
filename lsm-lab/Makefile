
LINK.o = $(LINK.cc)
CXXFLAGS = -std=c++14 -Wall

all: correctness persistence

correctness: kvstore.o correctness.o skiplist.o diskcontroller.o diskLevel.o ssTable.o

persistence: kvstore.o persistence.o skiplist.o diskcontroller.o diskLevel.o ssTable.o

clean:
	-rm -f correctness persistence *.o
