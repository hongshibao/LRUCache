default: cmake-release release test-release

cmake-debug:
	mkdir -p build/debug && \
	cd build/debug && \
	cmake -DCMAKE_BUILD_TYPE=Debug ../../

cmake-release:
	mkdir -p build/release && \
	cd build/release && \
	cmake -DCMAKE_BUILD_TYPE=Release ../../

debug:
	cd build/debug && $(MAKE)

release:
	cd build/release && $(MAKE)

test:
	cd build/debug && \
	./TestMain

test-release:
	cd build/release && \
	./TestMain

clean:
	rm -rf build/

