PREFIX?=/opt/babassl
CFLAGS=-g -O2 -Wall -Wextra -rdynamic -I$(PREFIX)/include $(OPTFLAGS)
LIBS=-L$(PREFIX)/lib/ -lcrypto -Wl,-rpath=$(PREFIX)/lib/ $(OPTLIBS)

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

SO_TARGET=demo.so

all: $(SO_TARGET)

$(SO_TARGET): CFLAGS += -fPIC
$(SO_TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f *.o *.so

test:
	OPENSSL_ENGINES=`pwd` $(PREFIX)/bin/openssl engine -t -c demo
	echo whatever | OPENSSL_ENGINES=`pwd` $(PREFIX)/bin/openssl dgst -md5 -engine demo
	echo whatever | OPENSSL_ENGINES=`pwd` $(PREFIX)/bin/openssl dgst -md5
