lint:
	clang-format -style=Google -i src/*.cc

idl:
	mkdir -p gen && flatc -o gen --cpp src/fbs/*.fbs

build: idl
	mkdir -p build && cd build && cmake .. && make && mkdir -p ../bin && cp -L fbs-demo ../bin

run:
	mkdir -p data && ./bin/fbs-demo

clean:
	rm -rf ./gen && rm -rf ./build && rm -rf ./bin && rm -rf ./data
