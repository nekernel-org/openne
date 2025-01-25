/* -------------------------------------------

	Copyright (C) 2024, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

/**
	@brief The Explicit Partition Map scheme.
*/

#ifndef FIRMWAREKIT_EPM_H
#define FIRMWAREKIT_EPM_H

#include <NewKit/Defines.h>

#define kEPMNameLength		 (32)
#define kEPMFilesystemLength (16)
#define kEPMMagicLength		 (5)

/* @brief AMD64 magic for EPM */
#define kEPMMagic86 " PMAM"

/* @brief RISC-V magic for EPM */
#define kEPMMagicRISCV " PMRV"

/* @brief ARM magic for EPM */
#define kEPMMagicARM " PMAR"

/* @brief POWER magic for EPM */
#define kEPMMagicPPC " PMPC"

/* @brief Invalid magic for EPM */
#define kEPMMagicError " PM??"

#ifdef __ZKA_AMD64__
#define kEPMMagic kEPMMagic86
#else
#ifdef __ZKA_ARM64__
#define kEPMMagic kEPMMagicARM
#else
#define kEPMMagic kEPMMagicError
#endif
#endif

///! @brief partition must start at this address.
///! Anything below is reserved for Data backup by the Main OS.
#define kEPMPartBlockLba (sizeof(EPM_PART_BLOCK))

///! @brief Current EPM revision.
#define kEPMRevisionBcd (0x0100)

/// !@brief EPM boot block address.
#define kEPMBootBlockLba (0U)

#define kEPMReserveLen (401)

struct EPM_PART_BLOCK;

/* The first 0 > 128 addresses of a disk contains these headers. */

/**
 * @brief The EPM boot block.
 * @note NumBlock and LbaStart are ignored on some platforms.
 */
struct PACKED EPM_PART_BLOCK
{
	Kernel::Char  Magic[kEPMMagicLength];
	Kernel::Char  Name[kEPMNameLength];
	Kernel::Int32 Version;
	Kernel::Int64 NumBlocks;
	Kernel::Int64 SectorSz;
	Kernel::Int64 LbaStart; // base offset
	Kernel::Int64 LbaEnd;	// addition of lba_start to get the end of partition.
	Kernel::Int16 Kind;
	Kernel::Int32 FsVersion;
	Kernel::Char  Fs[kEPMFilesystemLength]; /* OpenNeFS, NeFS, ffs2... */
	Kernel::Char  Reserved[kEPMReserveLen]; // to fill a full sector.
};

///! @brief Version kind enum.
///! @brief Use in boot block version field.

enum
{
	kEPMInvalid	  = 0x00,
	kEPMGenericOS = 0xcf, // Generic OS
	kEPMLinux	  = 0x8f, // Linux on EPM
	kEPMBSD		  = 0x9f, // Berkeley Soft. Distribution
	kEPMZkaOS	  = 0x1f, // This OS.
	kEPMInvalidOS = 0xff,
};

typedef struct EPM_PART_BLOCK BOOT_BLOCK_STRUCT;

#endif // ifndef FIRMWAREKIT_EPM_H
