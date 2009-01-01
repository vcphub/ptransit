
CFLAGS = -Wall -g
OBJS = main.o routes.o output_html.o utils.o check_data.o

run: $(OBJS)
	g++ $(CFLAGS) $(OBJS) -o rgen.exe

main.o : main.cpp route.h 
	g++ $(CFLAGS) -c main.cpp

routes.o : routes.cpp route.h utils.h
	g++ $(CFLAGS) -c routes.cpp

output_html.o : output_html.cpp route.h utils.h
	g++ $(CFLAGS) -c output_html.cpp

utils.o: utils.cpp
	g++ $(CFLAGS) -c utils.cpp

check_data.o: check_data.cpp
	g++ $(CFLAGS) -c check_data.cpp

PHONY:

clean:
	rm $(OBJS) rgen.exe r*.html
