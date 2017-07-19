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
	./pltl <ex

