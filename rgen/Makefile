
CFLAGS = -Wall -g

run: main.cpp routes.cpp output_html.cpp utils.cpp check_data.cpp
	g++ $(CFLAGS) main.cpp routes.cpp output_html.cpp utils.cpp check_data.cpp -o rgen.exe

PHONY:

clean:
	rm rgen.exe r*.html
