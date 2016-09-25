all:

CFLAGS += -Wall -Wextra -pipe -O3 -g
LDFLAGS += -fopenmp

QBIN2HEX := qbin2hex
QASM2 := qasm2
M4 := m4
RM := rm -f

all: main

main: LDLIBS += -lvc4vec
main: main.o qpu_memcpy.o cpu_memcpy_1.o cpu_memcpy_2.o cpu_memcpy_3.o  qpu_memcpy_1.o

cpu_memcpy_2.o: CFLAGS += -fopenmp
cpu_memcpy_3.o: CFLAGS += -fopenmp
qpu_memcpy_1.o: qpu_memcpy_1.qhex

.PRECIOUS: %.qhex
%.qhex: %.qbin
	$(QBIN2HEX) <"$<" >"$@"

.PRECIOUS: %.qbin
%.qbin: %.qasm2
	$(QASM2) <"$<" >"$@"

.PRECIOUS: %.qasm2
%.qasm2: %.qasm2m4
	$(M4) <"$<" >"$@"

.PHONY: clean
clean:
	$(RM) main main.o
	$(RM) qpu_memcpy.o cpu_memcpy_1.o cpu_memcpy_2.o cpu_memcpy_3.o qpu_memcpy_1.o
	$(RM) qpu_memcpy_1.qhex qpu_memcpy_1.qbin qpu_memcpy_1.qasm2
