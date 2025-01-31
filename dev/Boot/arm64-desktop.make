##################################################
# (c) Amlal EL Mahrouss, all rights reserved.
# This is the bootloader makefile.
##################################################

CC_GNU			= clang++
LD_GNU			= lld-link

ADD_FILE=touch
COPY=cp
HTTP_GET=wget

# Select this for Windows.
ifneq ($(findstring CYGWIN_NT-10.0,$(shell uname)), )
EMU=qemu-system-aarch64w.exe
else
# this for NT distributions
EMU=qemu-system-aarch64
endif

ifeq ($(NEWS_MODEL), )
OPENNE_MODEL=-DkMachineModel="\"ZKA\""
endif

BIOS=OVMF.fd
IMG=epm-master-1.img
IMG_2=epm-slave.img
IMG_3=epm-master-2.img

EMU_FLAGS=-net none -smp 4 -m 8G -cpu max -M virt \
			-bios $(BIOS) \
			-drive id=disk,file=$(IMG),format=raw,if=none \
			-drive \
			file=fat:rw:src/Root/,index=2,format=raw \
		    -no-shutdown -no-reboot -cpu cortex-a72 -device virtio-gpu-pci

LD_FLAGS=-subsystem:efi_application -entry:Main /nodefaultlib

STANDALONE_MACRO=-D__BOOTLDR_STANDALONE__
OBJ=*.o

REM=rm
REM_FLAG=-f

FLAG_ASM=-f win64
FLAG_GNU=-fshort-wchar -c -ffreestanding -MMD -mno-red-zone -D__OPENNE_ARM64__ -fno-rtti -fno-exceptions -I./ \
			 -target aarch64-unknown-windows \
				-std=c++20 -DBOOTZ_EPM_SUPPORT -DZBA_USE_FB -D__FSKIT_USE_NEFS__ -D__BOOTLDR_STANDALONE__ -D__MINOSKRNL__ -D__ZBAOSLDR__ -D__HAVE_OPENNE_APIS__ -D__OPENNE__ -I../ -I../Kernel

BOOT_LOADER=zbaosldr.exe
KERNEL=neoskrnl.exe
SYSCHK=syschk.sys
STARTUP=startup.sys

.PHONY: invalid-recipe
invalid-recipe:
	@echo "invalid-recipe: Use make compile-<arch> instead."

.PHONY: all
all: compile
	mkdir -p src/Root/EFI/BOOT
	$(LD_GNU) $(OBJ) $(LD_FLAGS) /out:src/$(BOOT_LOADER)
	$(COPY) src/$(BOOT_LOADER) src/Root/EFI/BOOT/BOOTAA64.EFI
	$(COPY) src/$(BOOT_LOADER) src/Root/EFI/BOOT/BootZ.EFI
	$(COPY) ../Kernel/$(KERNEL) src/Root/$(KERNEL)
	$(COPY) ./Mod/SysChk/$(SYSCHK) src/Root/$(SYSCHK)
	$(COPY) src/$(BOOT_LOADER) src/Root/$(BOOT_LOADER)

ifneq ($(DEBUG_SUPPORT), )
DEBUG =  -D__DEBUG__
endif

.PHONY: compile
compile:
	$(RESCMD)
	$(CC_GNU) $(OPENNE_MODEL) $(STANDALONE_MACRO) $(FLAG_GNU) $(DEBUG) \
	$(wildcard src/HEL/ARM64/*.cc) \
	$(wildcard src/HEL/ARM64/*.S) \
	$(wildcard src/*.cc)

.PHONY: run
run:
	$(EMU) $(EMU_FLAGS)

# img_2 is the rescue disk. img is the bootable disk, as provided by the Zeta.
.PHONY: epm-img
epm-img:
	qemu-img create -f raw $(IMG) 10G
	qemu-img create -f raw $(IMG_2) 4G
	qemu-img create -f raw $(IMG_3) 4G

.PHONY: efi
efi:
	$(HTTP_GET) https://retrage.github.io/edk2-nightly/bin/DEBUGAARCH64_QEMU_EFI.fd -O OVMF.fd

BINS=*.bin
EXECUTABLES=zbaosldr.exe neoskrnl.exe OVMF.fd

TARGETS=$(REM_FLAG) $(OBJ) $(BIN) $(IMG) $(IMG_2) $(EXECUTABLES)

.PHONY: clean
clean:
	$(REM) $(TARGETS)

.PHONY: help
help:
	@echo "=== HELP ==="
	@echo "epm-img: Format a disk using the Explicit Partition Map."
	@echo "gpt-img: Format a disk using the Explicit Partition Map."
	@echo "clean: clean bootloader."
	@echo "bootloader-amd64: Build bootloader. (PC AMD64)"
	@echo "run: Run bootloader. (PC AMD64)"
