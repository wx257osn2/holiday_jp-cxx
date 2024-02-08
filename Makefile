all: generate test example

.PHONY: generate test example clean

clean:
	$(RM) bin/test bin/example generator/generator

test: test/test.cpp
	$(CXX) -c -std=c++20 -Wall -Wextra -pedantic-errors -O3 -I include -o/dev/null $<
	@echo "test passed!"

example: bin/example
generate: include/stringized_holidays_yml.ipp

include/stringized_holidays_yml.ipp: generator/generator holiday_jp/holidays.yml
	./$< < ../holiday_jp/holidays.yml > $@

generator/generator: generator/generator.cpp
	$(CXX) -std=c++11 -Wall -Wextra -pedantic-errors -O3 -o$@ $<

bin/example: example/example.cpp include/holiday_jp.hpp generate
	$(CXX) -std=c++20 -Wall -Wextra -pedantic-errors -O3 -I include -o$@ $<
