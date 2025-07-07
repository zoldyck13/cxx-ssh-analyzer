TARGET = ssh_enum

SRC = src/ssh_enum.cpp

LIBS = -lssh2

$(TARGET): $(SRC)
    @g++ $(SRC) -o $(TARGET) $(LIBS)
    @echo "Build complete: $(TARGET)"
