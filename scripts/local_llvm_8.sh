mkdir llvm_build
cd llvm_build
wget https://github.com/llvm/llvm-project/archive/llvmorg-8.0.1.tar.gz
tar -xzf llvmorg-8.0.1.tar.gz
cd llvm-project-llvmorg-8.0.1/
patch -fN -p1 -i ../../scripts/llvm-gcc-fix.patch
cd llvm
mkdir build
cd build
cmake ..
make -j$(nproc --all)
