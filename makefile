TARGET = main
SRCS = *.cpp
OBJS = $(SRCS:.cpp=.o)
CFLAGS = -Wall -Wextra -pedantic
LIBS = -pthread
CXX = g++ # clang++


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

$(OBJS): $(SRCS)
	$(CXX) $(CFLAGS) -c $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

rebuild: clean all run 

.PHONY: all clean run rebuild
