all: master consumer

master: master.o
	gcc -o master master.o

master.o: master.c
	gcc -c master.c

consumer: consumer.o
	gcc -o consumer consumer.o

consumer.o: consumer.c
	gcc -c consumer.c

clean: remove

remove:
	rm *.o master consumer *.out

success:
	$(info SUCCESS)
