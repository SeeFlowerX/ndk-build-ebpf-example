# ndk-build-ebpf-example

本项目用于演示如何使用NDK编译一个eBPF程序，主要参考[为Android平台编译eBPF程序](https://www.52pojie.cn/thread-1649849-1-1.html)

# 步骤

## 准备工作

先同步需要的源代码，这里选择的是`android11-gsi`分支，可以根据自己的需求修改

```bash
git clone -b android11-gsi https://android.googlesource.com/platform/bionic --depth=1
git clone -b android11-gsi https://android.googlesource.com/platform/system/core --depth=1
git clone -b android11-gsi https://android.googlesource.com/platform/system/bpf --depth=1
```

整理文件结构

```bash
mkdir system && mv core system/ && mv bpf system/
```

然后准备好NDK，修改`build.sh`中的`NDK_ROOT`即可

## 编译

执行`./build.sh`即可，产物在`out`目录下

# 其他

Q: 和参考文章有什么不同吗？

A: 编译增加了`-g`选项，作用是生成带有BTF信息的`.o`文件

    https://clang.llvm.org/docs/UsersManual.html#cmdoption-g