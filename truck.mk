CFLAGS=	-DCfg_locus -DCfg_log -DCYGWIN -DDEBUG -g -std=c99 -I. -I.. -I../..	\
	-I../../.. -Iqueue -Iintex -Ivfile/v1 -Ivfile/v2 -Inote -Iprint

truck:	truck.o
	gcc $^ -o truck -lm

truck.o: truck.c
