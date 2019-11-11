CXXFLAGS=-O0 -g -ftemplate-backtrace-limit=1000

EXES=r0 r3

all: $(EXES)

go: $(EXES:%=log_%.txt)

r%: baser.cpp
	$(LINK.cpp) -D__$(@)__ $(CXXFLAGS) -o $@ $<
	

log_%.txt: %
	./$< | tee $@

clean:
	$(RM) $(EXES) $(EXES:%=%.exe)
