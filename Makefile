BUILDDIR = build
CXX = clang++
CXXFLAGS = -I. -Ilib/imgui/ -Ilib/imgui/examples/ -Wall -std=c++11 -g
LDFLAGS = -lGL -lGLEW
PLATFORM = GLFW
IMGUI_FLAGS = -DIMGUI_IMPL_OPENGL_LOADER_GLEW

UNAME_S := $(shell uname -s)

IMGUI_SOURCES = lib/imgui/examples/imgui_impl_opengl3.cpp \
	lib/imgui/imgui.cpp \
	lib/imgui/imgui_draw.cpp \
	lib/imgui/imgui_widgets.cpp \
	lib/imgui/imgui_demo.cpp

SOURCES := $(shell find src -type f -name '*.cpp' ! -name 'platform*.cpp')

ifeq ($(UNAME_S),Linux)
	SOURCES += src/platform_linux.cpp
endif

ifeq ($(PLATFORM),SDL)
	SOURCES += src/platform_sdl2.cpp
	IMGUI_SOURCES += lib/imgui/examples/imgui_impl_sdl.cpp
	CXXFLAGS += -DPLATFORM_SDL2
	LDFLAGS += -lSDL2
endif

ifeq ($(PLATFORM),GLFW)
	SOURCES += src/platform_glfw.cpp
	IMGUI_SOURCES += lib/imgui/examples/imgui_impl_glfw.cpp
	CXXFLAGS += -DPLATFORM_GLFW
	LDFLAGS += -lglfw
endif

SOURCES += $(IMGUI_SOURCES)
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
