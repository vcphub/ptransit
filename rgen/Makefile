
CFLAGS = -Wall -g
OBJS = main.o stops.o routes.o trips.o check_data.o output_html.o utils.o 

run: $(OBJS)
	g++ $(CFLAGS) $(OBJS) -o rgen.exe

main.o : main.cpp route.h output_html.h
	g++ $(CFLAGS) -c main.cpp

routes.o : routes.cpp route.h utils.h
	g++ $(CFLAGS) -c routes.cpp

stops.o : stops.cpp stop.h utils.h
	g++ $(CFLAGS) -c stops.cpp

trips.o : trips.cpp route.h utils.h
	g++ $(CFLAGS) -c trips.cpp

output_html.o : output_html.cpp route.h utils.h output_html.h
	g++ $(CFLAGS) -c output_html.cpp

utils.o: utils.cpp
	g++ $(CFLAGS) -c utils.cpp

check_data.o: check_data.cpp
	g++ $(CFLAGS) -c check_data.cpp

PHONY:

clean:
	rm $(OBJS) rgen.exe html/en-r*.html html/r*.html html/compact-r*.html html/s*.html html/en-s*.html
