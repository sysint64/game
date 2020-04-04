BUILDDIR = build
LDFLAGS = -ljpeg -lpng -ltmx
CXX = clang++
CXXFLAGS = -I. -Ilib/imgui/ -Ilib/imgui/examples/ -Wall -std=c++11 -g -DVALIDATE
SANITIZE = false

ifeq ($(SANITIZE), true)
	CXXFLAGS += -fsanitize=address
endif

SINGLE_SOURCE = false

# ENUM: GLFW, SDL
PLATFORM = SDL

# ENUM: OPENGL, VULKAN, METAL
GAPI = OPENGL
IMGUI_FLAGS = -DIMGUI_IMPL_OPENGL_LOADER_GLEW

UNAME_S := $(shell uname -s)

IMGUI_SOURCES = lib/imgui/imgui.cpp \
	lib/imgui/imgui_draw.cpp \
	lib/imgui/imgui_widgets.cpp \
	lib/imgui/imgui_demo.cpp

SOURCES := $(shell find src -type f -name '*.cpp' ! -name 'platform_*.cpp' ! -name 'gapi_*.cpp')

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

ifeq ($(GAPI),OPENGL)
	SOURCES += src/gapi_opengl.cpp
	IMGUI_SOURCES += lib/imgui/examples/imgui_impl_opengl3.cpp
	LDFLAGS += -lGL -lGLEW
	CXXFLAGS += -DGAPI_OPENGL

	ifeq ($(PLATFORM),SDL)
		SOURCES += src/gapi_opengl_sdl2.cpp
	else ifeq ($(PLATFORM),GLFW)
		SOURCES += src/gapi_opengl_glfw.cpp
	endif
endif

ifeq ($(GAPI),VULKAN)
	CXXFLAGS += -DGAPI_VULKAN
	LDFLAGS += -lvulkan
	SOURCES += src/gapi_vulkan.cpp
	IMGUI_SOURCES += lib/imgui/examples/imgui_impl_vulkan.cpp

	ifeq ($(PLATFORM),SDL)
		SOURCES += src/gapi_vulkan_sdl2.cpp
	else ifeq ($(PLATFORM),GLFW)
		SOURCES += src/gapi_vulkan_glfw.cpp
	endif
endif

SOURCES += $(IMGUI_SOURCES)
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))

BINARY = game

build: $(BINARY)

ifeq ($(SINGLE_SOURCE), true)

$(BINARY): tidy
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -DSINGLE_SOURCE src/main.cpp -o build/$(BINARY)

tidy:
	clang-tidy src/main.cpp -- $(CXXFLAGS) $(LDFLAGS) -DSINGLE_SOURCE

else

$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o build/$(BINARY)

$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(BUILDDIR)/$(dir $<)
	$(CXX) $(CXXFLAGS) $(IMGUI_FLAGS) -I$(dir $<) -c $< -o $@

endif

run: $(BINARY)
	./$(BUILDDIR)/$(BINARY)

clean:
	rm -rf $(BUILDDIR)
