
# Common dependencies
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main" >> /etc/apt/sources.list
echo "deb-src http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main" >> /etc/apt/sources.list
sudo apt-get update
sudo apt-get install -y cmake libllvm6.0 libz-dev llvm-6.0 llvm-6.0-dev llvm-6.0-runtime ninja-build python3 rsync

if [ $CC = "clang" ]
then
	# Clang 6
	sudo apt-get install -y clang-6.0 libclang-common-6.0-dev libclang1-6.0 libclang-6.0-dev
else
	# GCC 7
	sudo add-apt-repository ppa:ubuntu-toolchain-r/test
	sudo apt update
	sudo apt install -y gcc-7 g++-7
fi
