CXXFLAGS=-O0 -g -ftemplate-backtrace-limit=1000

EXES=r0 r3
LOGS=$(EXES:%=log_%.txt)

all: $(EXES)

go: $(LOGS)

r%: baser.cpp
	$(LINK.cpp) -D__$(@)__ $(CXXFLAGS) -o $@ $<

log_results: $(LOGS)
	for i in log_r?.txt ; do echo $$i ; python scale_times.py $$i ; done


log_%.txt: %
	./$< | tee $@

clean:
	$(RM) $(EXES) $(EXES:%=%.exe)
