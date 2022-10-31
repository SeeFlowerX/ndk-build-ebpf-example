ebpf-build:
	clang \
	--target=bpf \
	-c \
	-nostdlibinc \
	-no-canonical-prefixes \
	-O2 \
	-isystem bionic/libc/include \
	-isystem bionic/libc/kernel/uapi \
	-isystem bionic/libc/kernel/uapi/asm-arm64 \
	-isystem bionic/libc/kernel/android/uapi \
	-I       system/core/libcutils/include \
	-I       system/bpf/progs/include \
	-g \
	-MD -MF out/stack.d \
	-o out/stack.o \
	src/stack.c