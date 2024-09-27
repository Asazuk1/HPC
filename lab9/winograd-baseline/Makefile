
#all:
#	gcc -std=c11 -fopenmp -O3 -g driver.c winograd.c -o winograd
#	# gcc -std=c11 -D__DEBUG -O0 -g driver.c winograd.c -o winograd

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11 -fopenmp -O3 -g

# Linker flags (Link with the OpenBLAS library)
LDFLAGS = -lopenblas

# Source files
SRC = driver.c winograd.c

# Output binary
TARGET = winograd

# Default rule
all: $(TARGET)

# Rule to compile the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Debug build (uncomment the line below to enable debug build)
# debug:
#	$(CC) -std=c11 -D__DEBUG -O0 -g $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean rule to remove binary
clean:
	rm -f $(TARGET)
