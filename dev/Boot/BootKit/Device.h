/* -------------------------------------------

	Copyright (C) 2024, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <Mod/ATA/ATA.h>
#include <Mod/AHCI/AHCI.h>

using namespace Kernel;

/// @brief Device type.
class Device
{
public:
	explicit Device() = default;
	virtual ~Device() = default;

	OPENNE_MOVE_DEFAULT(Device);

	struct Trait
	{
		SizeT mBase{1024};
		SizeT mSize{1024};
	};

	virtual Trait& Leak() = 0;

	virtual Device& Read(Char* Buf, const SizeT& SecCount)	= 0;
	virtual Device& Write(Char* Buf, const SizeT& SecCount) = 0;
};

typedef Device BootDevice;
typedef Device NetworkDevice;
typedef Device DiskDevice;
