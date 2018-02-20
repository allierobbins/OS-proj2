master: master.o
	gcc -o master master.o

master.o: master.c
	gcc -c master.c

consumer: consumer.o
	gcc -o consumer consumer.o

consumer.o: consumer.c
	gcc -c consumer.c

producer:producer.o
	gcc -o producer producer.o

producer.o: producer.c
	gcc -c producer.c

clean:
	rm *.o master consumer producer *.o
