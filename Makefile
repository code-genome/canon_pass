all: pass.so

llvm_build:
	bash scripts/local_llvm_8.sh

export:
	export PATH=$(pwd)/llvm_build/llvm-project-llvmorg-8.0.1/llvm/build/bin:$PATH

pass.so: export
	mkdir -p build && cd build && cmake ../pass && make -j$(nproc)

clean:
	rm -rf build
	rm -rf tests/build

test: pass.so
	./build/canonicalization_test

test_sample: pass.so
	./llvm_build/llvm-project-llvmorg-8.0.1/llvm/build/bin/opt -load ./build/libcanonicalization-pass.so -canonicalization < tests/global.ir >/dev/null
