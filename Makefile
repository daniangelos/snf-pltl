all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean
	rm -f *.*~

clean_parser:
	$(MAKE) -C src clean_parser

clean_all:
	$(MAKE) -C src clean_all

example: $(TARGET) ex
	./pltl -i ex

valgrind: $(TARGET) ex
	valgrind --leak-check=yes ./pltl <ex 2>valgrind.out

bigcase: $(TARGET) 
	valgrind --leak-check=yes ./pltl <../benchmarks/anzu/amba/amba_c/spec_c_11.negated.tspass 2>valgrind.bigcase

script: tests/scripts/simpl.py
	tests/scripts/simpl.py rm

