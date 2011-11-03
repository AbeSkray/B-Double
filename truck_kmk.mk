CFLAGS=	-DCfg_locus -DCfg_log -DCYGWIN -DDEBUG -g -std=c99 -I. -I.. -I../..	\
	-I../../.. -Iqueue -Iintex -Ivfile/v1 -Ivfile/v2 -Inote -Iprint

truck:	truck.o ../log/v4/log.o ../except/except.o ../trap/trap.o		\
	  ../queue/queue_1.o ../memory/heap/heap_min.o ../io/io.o		\
	  ../debug/watch.o ../string/copy.o ../string/ascii.o			\
	  ../debug/debug.o ../thread/thread.o ../system/system.o		\
	  ../error/error.o ../string/swap.o ../bits/bit.o			\
	  ../string/wstring.o ../string/string.o ../queue/queue.o		\
	  ../class/class.o ../note/note.o ../map/tree/tree.o			\
	  ../print/print.o ../sort/sort_min.o ../vfile/v3/vfile.o		\
	  ../input/input.o ../input/load.o ../string/bm.o			\
	  ../string/casecpy.o ../frame/frame.o ../path/path.o			\
	  ../string/in.o ../path/dir2.o ../map/pair/pair.o
	gcc $^ -o truck

truck.o: truck.c
