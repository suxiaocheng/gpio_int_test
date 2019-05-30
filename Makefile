
ifneq ($(KERNELRELEASE),)

obj-m += gpio_test.o

else

KERNELDIR ?= /home/user14/g6p-hqx/LINUX/android/out/target/product/msm8996_gvmq/obj/KERNEL_OBJ
PWD ?= $(shell pwd)
ARCH = arm64
CROSS_COMPILE=/home/user14/g6p-hqx/LINUX/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
SIG_PRG=/home/user14/g6p-hqx/LINUX/android/out/target/product/msm8996_gvmq/obj/KERNEL_OBJ/scripts/sign-file
PEM=/home/user14/g6p-hqx/LINUX/android/out/target/product/msm8996_gvmq/obj/KERNEL_OBJ/certs/signing_key.pem
X509=/home/user14/g6p-hqx/LINUX/android/out/target/product/msm8996_gvmq/obj/KERNEL_OBJ/certs/signing_key.x509

.PHONY: modules clean

modules:
	    $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) modules
	    ${SIG_PRG} sha512 ${PEM} ${X509} gpio_test.ko
clean: 
	    @rm -rf *.o *.order *.symvers *.mod.* .*.o.cmd .*.mod.o.cmd .*.ko.cmd .tmp_versions *.ko

endif

