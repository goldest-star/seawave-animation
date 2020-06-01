TARGET ?= pgm
SRC_DIRS ?= .

CXX ?= g++

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS  := .
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS += $(INC_FLAGS) -MMD -MP -DIMGUI_IMPL_OPENGL_LOADER_GLAD -g -O2 -std=c++11 -Wall -Wextra
LDLIBS += -lglfw -ldl -lm

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)

#Makefile inspired from https://spin.atomicobject.com/2016/08/26/makefile-c-projects/ @Job Vranish