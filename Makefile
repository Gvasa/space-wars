# Select compiler.
CXX = g++

# Set flags
CXXFLAGS := -std=c++11 -std=gnu++11

# Get current directory.
SELF_DIR = $(dir $(lastword $(MAKEFILE_LIST)))

# Set output name.
BINNAME = main.exe

# Set output directory.
BINDIR = bin/

# Specify files to compile.
SRCDIR = $(wildcard src/*.cpp) $(wildcard src/core/*.cpp) $(wildcard src/objects/*.cpp) $(wildcard src/json/*.cpp)

# Set lib directory.
LIBDIR = -L"$(SELF_DIR)lib"

# Set include directory
INCDIR = -I"$(SELF_DIR)include" -I"$(SELF_DIR)include/Bullet" -I"$(SELF_DIR)src"

# Specify libraries.
LIBRARIES = -lenet -lwinmm -losgbDynamics -losgbCollision -losgbInteraction -losg -losgUtil -losgDB -losgGA -losgViewer -losgFX -lsgct -lopengl32 -lglu32 -lgdi32 -lws2_32 -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody

compile: $(SRCDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR) -o $(BINDIR)$(BINNAME) $(LIBDIR) $(INCDIR) $(LIBRARIES)