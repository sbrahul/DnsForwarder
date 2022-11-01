TARGET = DnsFwder
CXX ?= g++
#CFLAGS ?= -fsanitize=address -g
COMPFLAGS = -O2 -fPIC -std=c++17 -Wall
MYLDFLAGS = -pthread
SRCDIR = src
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
HDRS = $(wildcard $(SRCDIR)/*.h)

.PHONY: all clean dir

all: dir $(TARGET)

dir:
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) $(COMPFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRS)
	$(CXX) $(CFLAGS) $(COMPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/* $(TARGET)

