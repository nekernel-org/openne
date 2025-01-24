<!-- README of NeKernel -->

# New Kernel Architecture (NeKernel)

NeKernel is designed for servers, desktop, mobile and workstations.

## Requirements

- MinGW/GCC for the Kernel, Boot Loader and the LibSCI.
- Netwide Assembler for the x64 assembly files (in case you're compiling for AMD64).

### Installation

Clone repository:

```
git clone git@github.com:mediaswirl/nekernel.git
cd kernel-services
```

</br>

And then select the makefile (arm64 and amd64 are stable and EFI based) to execute:

```
make -f amd64-efi.make all
```

###### Copyright (C) 2024, Amlal EL Mahrouss, all rights reserved. All rights reserved.
