TARGET = DnsFwder
CXX ?= g++
COMPFLAGS = -g -O0 -fPIC -std=c++17 -Wall
SRCDIR = src
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean dir

all: dir $(TARGET)

dir:
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(COMPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/* $(TARGET)

