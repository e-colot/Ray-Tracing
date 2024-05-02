# VERSION CHATGPT :

CXX = g++
# $(shell sdl2-config --cflags)
CXXFLAGS += -std=c++20 -Wall -Wextra -pedantic -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wextra-semi -Wfloat-equal -Winit-self -Wnon-virtual-dtor -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wzero-as-null-pointer-constant -fmax-errors=2 -Wformat=2

LDFLAGS = -L"E:\Codage\SDL(Cpp_lib)\SDL2-2.30.2\lib\x64" -L"E:\Codage\SDL_TTF(Cpp_lib)\SDL2_ttf-2.22.0\lib\x64"
INCLUDES = -I"E:\Codage\SDL(Cpp_lib)\SDL2-2.30.2\include" -I"E:\Codage\SDL_TTF(Cpp_lib)\SDL2_ttf-2.22.0\include"
LIBS = -lSDL2 -lSDL2_ttf -lSDL2main

# Liste des fichiers source pour TARGET1
SRCS_TARGET1 = constants.cpp complex.cpp material.cpp vector.cpp wall.cpp path.cpp ray.cpp antenna.cpp tile.cpp graphics.cpp map.cpp test.cpp

# Liste des fichiers objets pour chaque ensemble de fichiers source
OBJS_TARGET1 = $(SRCS_TARGET1:.cpp=.o)

# Name of the executables
TARGET1 = test

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the executable

$(TARGET1): $(OBJS_TARGET1)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	del /F /Q $(OBJS_TARGET1) $(TARGET1)

# to compile and run :
# code.bat
# compile.bat
