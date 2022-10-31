NDK_ROOT=/home/kali/Desktop/android-ndk-r25b

export PATH=$NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

mkdir -p out && rm -rf out/*

make ebpf-build