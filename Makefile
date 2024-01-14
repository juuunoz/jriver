obj.m += jriver.o

all:
	gcc test.c -o "test"
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
clean:
	rm -f test
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
