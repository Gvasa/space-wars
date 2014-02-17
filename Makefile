# Select compiler.
CC = g++

# Get current directory.
SELF_DIR = $(dir $(lastword $(MAKEFILE_LIST)))

# Set output name.
BINNAME = main.exe

# Set output directory.
BINDIR = bin/

# Specify files to compile.
SRCDIR = $(wildcard src/*.cpp)

# Set lib directory.
LIBDIR = -L"$(SELF_DIR)lib"

# Set include directory
INCDIR = -I"$(SELF_DIR)include" -I"$(SELF_DIR)include/Bullet"

# Specify libraries.
LIBRARIES = -lenet  -lwinmm -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -losg -losgUtil -losgDB -losgGA -losgViewer -lsgct -lopengl32 -lglu32 -lgdi32 -lws2_32

compile: $(SRCDIR)
	$(CC) $(SRCDIR) -o $(BINDIR)$(BINNAME) $(LIBDIR) $(INCDIR) $(LIBRARIES)