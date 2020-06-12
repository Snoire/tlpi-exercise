targets = large_file write_append atomic_append \
          verify_dupfd output_after_write

all : $(targets)
.PHONY : all clean cleanobj

verify_dupfd.o : dup_fcntl.h
verify_dupfd : verify_dupfd.o dup_fcntl.o

clean : cleanobj
	/bin/rm -f $(targets) tfile

cleanobj :
	/bin/rm -f *.o
