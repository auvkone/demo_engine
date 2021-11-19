CFLAGS=-g -O2 -Wall -Wextra -rdynamic -I/usr/local/babassl-share/include $(OPTFLAGS)
LIBS=-L/usr/local/babassl-share/lib/ -lcrypto $(OPTLIBS)

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

SO_TARGET=demo.so

all: $(SO_TARGET)

$(SO_TARGET): CFLAGS += -fPIC
$(SO_TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

clean:
	rm -f *.o *.so

test:
	/usr/local/babassl-share/bin/openssl engine -t -c `pwd`/demo.so
	echo whatever | /usr/local/babassl-share/bin/openssl dgst -md5 -engine `pwd`/demo.so
	echo whatever | /usr/local/babassl-share/bin/openssl dgst -md5
