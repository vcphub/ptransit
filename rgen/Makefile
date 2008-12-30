
CFLAGS = -Wall -g

run: main.cpp routes.cpp output_html.cpp utils.cpp
	g++ $(CFLAGS) main.cpp routes.cpp output_html.cpp utils.cpp -o rgen.exe

PHONY:

clean:
	rm rgen.exe r*.html
