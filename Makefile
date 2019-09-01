BUILDDIR = build
CXX = clang++
CXXFLAGS = -I. -Ilib/imgui/ -Ilib/imgui/examples/ -Wall -std=c++11 -g
LDFLAGS = -lSDL2 -lGL -lGLEW
PLATFORM = SDL
IMGUI_FLAGS = -DIMGUI_IMPL_OPENGL_LOADER_GLEW

OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS += $(IMGUI_SOURCES:.cpp=.o)

UNAME_S := $(shell uname -s)

IMGUI_SOURCES = lib/imgui/examples/imgui_impl_sdl.cpp \
	lib/imgui/examples/imgui_impl_opengl3.cpp \
	lib/imgui/imgui.cpp \
	lib/imgui/imgui_draw.cpp \
	lib/imgui/imgui_widgets.cpp \
	lib/imgui/imgui_demo.cpp

SOURCES := $(shell find src -type f -name '*.cpp' ! -name 'platform*.cpp')
SOURCES += $(IMGUI_SOURCES)

ifeq ($(UNAME_S),Linux)
	SOURCES += src/platform_linux.cpp
endif

ifeq ($(PLATFORM),SDL)
	SOURCES += src/platform_sdl2.cpp
	CXXFLAGS += -DPLATFORM_SDL2
endif

OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))
BINARY = game

build: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o build/$(BINARY)

$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(BUILDDIR)/$(dir $<)
	$(CXX) $(CXXFLAGS) $(IMGUI_FLAGS) -I$(dir $<) -c $< -o $@

run: $(BINARY)
	./$(BUILDDIR)/$(BINARY)

clean:
	rm -rf $(BUILDDIR)
