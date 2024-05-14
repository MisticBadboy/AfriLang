SRCS = $(wildcard src/*.c)
INCLUDES = -Iinclude

compile: $(SRCS)
	gcc $^ $(INCLUDES) --std=c2x -o target

run : compile
	./target.exe .\main.afrl