/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrous, all rights reserved.

------------------------------------------- */

#pragma once

#ifdef OPENNE_USE_MBCI_FLASH

#include <NewKit/Defines.h>

Kernel::SizeT drv_get_sector_count(Kernel::Int32 slot);

Kernel::SizeT drv_get_size(Kernel::Int32 slot);

Kernel::Void drv_enable_at(Kernel::Int32 slot);

Kernel::Void drv_disable_at(Kernel::Int32 slot);

#endif // ifdef OPENNE_USE_MBCI_FLASH
