/* -------------------------------------------

	Copyright (C) 2024, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#ifndef FIRMWARE_KIT_EFI_H
#define FIRMWARE_KIT_EFI_H

/**
@brief Implementation of the main EFI protocols.
*/

#include <NewKit/Defines.h>

using namespace Kernel;

/* we always use stdcall in EFI, the pascal way of calling functions. */

#ifndef EPI_API
#define EFI_API __attribute__((ms_abi))
#endif // ifndef EPI_API

#define IN
#define OUT
#define OPTIONAL

#define EFI_FINAL final

// Forward decls

struct EfiTableHeader;
struct EfiLoadFileProtocol;
struct EfiSimpleTextOutputProtocol;
struct EfiDevicePathProtocol;
struct EfiBootServices;
struct EfiMemoryDescriptor;
struct EfiSystemTable;
struct EfiGUID;
struct EfiFileDevicePathProtocol;
struct EfiHandle;
struct EfiGraphicsOutputProtocol;
struct EfiBitmask;
struct EfiFileProtocol;
struct EfiSimpleTextInputProtocol;

typedef UInt64 EfiStatusType;

typedef Char16 EfiChar16Type;

/// @brief Core Handle Kind
/// Self is like NT's Win32 HANDLE type.
typedef struct EfiHandle
{
} * EfiHandlePtr;

/* UEFI uses wide characters by default. */
typedef WideChar EfiCharType;

typedef UInt64	EfiPhysicalAddress;
typedef UIntPtr EfiVirtualAddress;

/// What's BootBolicy?
/// If TRUE, indicates that the request originates from the boot manager, and
/// that the boot manager is attempting to load FilePath as a boot selection. If
/// FALSE, then FilePath must match an exact file to be loaded.

typedef UInt64(EFI_API* EfiTextString)(struct EfiSimpleTextOutputProtocol* Self,
									   const WideChar*					   OutputString);

typedef UInt64(EFI_API* EfiTextAttrib)(struct EfiSimpleTextOutputProtocol* Self,
									   const WideChar					   Attribute);

typedef UInt64(EFI_API* EfiTextClear)(struct EfiSimpleTextOutputProtocol* Self);

typedef UInt64(EFI_API* EfiLoadFile)(EfiLoadFileProtocol*		Self,
									 EfiFileDevicePathProtocol* FilePath,
									 Boolean					BootPolicy,
									 UInt32*					BufferSize,
									 VoidPtr					Buffer);

typedef UInt64(EFI_API* EfiCopyMem)(VoidPtr DstBuf, VoidPtr SrcBuf, SizeT Length);

typedef UInt64(EFI_API* EfiSetMem)(VoidPtr DstBuf, Char Byte, SizeT Length);

typedef UInt64(EFI_API* EfiHandleProtocol)(EfiHandlePtr Handle, EfiGUID* Guid, VoidPtr* Device);

typedef UInt64(EFI_API* EfiLocateDevicePath)(EfiGUID*				 Protocol,
											 EfiDevicePathProtocol** DevicePath,
											 EfiHandlePtr			 Device);

typedef UInt64(EFI_API* EfiStartImage)(EfiHandlePtr Handle, VoidPtr ArgsSize, VoidPtr ArgsPtr);

typedef UInt64(EFI_API* EfiLoadImage)(Boolean					 BootPolicy,
									  EfiHandlePtr				 ParentHandle,
									  EfiFileDevicePathProtocol* DeviceFile,
									  VoidPtr					 buffer,
									  SizeT						 size,
									  EfiHandlePtr*				 ppHandle);

/// EFI pool helpers, taken from iPXE.

typedef enum EfiMemoryType
{
	///
	/// Not used.
	///
	EfiReservedMemoryType,
	///
	/// The code portions of a loaded application.
	/// (Note that UEFI OS loaders are UEFI applications.)
	///
	EfiLoaderCode,
	///
	/// The data portions of a loaded application and the default data allocation
	/// type used by an application to allocate pool memory.
	///
	EfiLoaderData,
	///
	/// The code portions of a loaded Boot Services Driver.
	///
	EfiBootServicesCode,
	///
	/// The data portions of a loaded Boot Serves Driver, and the default data
	/// allocation type used by a Boot Services Driver to allocate pool memory.
	///
	EfiBootServicesData,
	///
	/// The code portions of a loaded Runtime Services Driver.
	///
	EfiRuntimeServicesCode,
	///
	/// The data portions of a loaded Runtime Services Driver and the default
	/// data allocation type used by a Runtime Services Driver to allocate pool
	/// memory.
	///
	EfiRuntimeServicesData,
	///
	/// Free (unallocated) memory.
	///
	EfiConventionalMemory,
	///
	/// Memory in which errors have been detected.
	///
	EfiUnusableMemory,
	///
	/// Memory that holds the ACPI tables.
	///
	EfiACPIReclaimMemory,
	///
	/// Address space reserved for use by the firmware.
	///
	EfiACPIMemoryNVS,
	///
	/// Used by system firmware to request that a memory-mapped IO region
	/// be mapped by the OS to a virtual address so it can be accessed by EFI
	/// runtime services.
	///
	EfiMemoryMappedIO,
	///
	/// System memory-mapped IO region that is used to translate memory
	/// cycles to IO cycles by the processor.
	///
	EfiMemoryMappedIOPortSpace,
	///
	/// Address space reserved by the firmware for code that is part of the
	/// processor.
	///
	EfiPalCode,
	///
	/// A memory region that operates as EfiConventionalMemory,
	/// however it happens to also support byte-addressable non-volatility.
	///
	EfiPersistentMemory,
	///
	/// A memory region that describes system memory that has not been accepted
	/// by a corresponding call to the underlying isolation architecture.
	///
	EfiUnacceptedMemoryType,
	///
	/// The last type of memory.
	/// Not a real type.
	///
	EfiMaxMemoryType,
} EfiMemoryType;

typedef enum EfiAllocateType
{
	/// Anything that satisfy the request.
	AllocateAnyPages,
	AllocateMaxAddress,
	///
	/// Allocate pages at a specified address.
	///
	AllocateAddress,
	///
	/// Maximum enumeration value that may be used for bounds checking.
	///
	MaxAllocateType
} EfiAllocateType;

typedef struct EfiMemoryDescriptor
{
	///
	/// Kind of the memory region.
	/// Kind EFI_MEMORY_TYPE is defined in the
	/// AllocatePages() function description.
	///
	UInt32 Kind;
	///
	/// Physical address of the first byte in the memory region. PhysicalStart
	/// must be aligned on a 4 KiB boundary, and must not be above
	/// 0xfffffffffffff000. Kind EFI_PHYSICAL_ADDRESS is defined in the
	/// AllocatePages() function description
	///
	EfiPhysicalAddress PhysicalStart;
	///
	/// Virtual address of the first byte in the memory region.
	/// VirtualStart must be aligned on a 4 KiB boundary,
	/// and must not be above 0xfffffffffffff000.
	///
	EfiVirtualAddress VirtualStart;
	///
	/// NumberOfPagesNumber of 4 KiB pages in the memory region.
	/// NumberOfPages must not be 0, and must not be any value
	/// that would represent a memory page with a start address,
	/// either physical or virtual, above 0xfffffffffffff000.
	///
	UInt64 NumberOfPages;
	///
	/// Attributes of the memory region that describe the bit mask of capabilities
	/// for that memory region, and not necessarily the current settings for that
	/// memory region.
	///
	UInt64 Attribute;
} EfiMemoryDescriptor;

typedef UInt64(EFI_API* EfiAllocatePool)(EfiMemoryType PoolType, UInt32 Size, VoidPtr* Buffer);

typedef UInt64(EFI_API* EfiFreePool)(VoidPtr Buffer);

typedef UInt64(EFI_API* EfiCalculateCrc32)(VoidPtr Data, UInt32 DataSize, UInt32* CrcOut);

/**
@brief Present in every header, used to identify a UEFI structure.
*/
typedef struct EfiTableHeader
{
	UInt64 Signature;
	UInt32 Revision;
	UInt32 HeaderSize;
	UInt32 Crc32;
	UInt32 Reserved;
} EfiTableHeader;

#define EFI_ACPI_TABLE_PROTOCOL_GUID                       \
	{                                                      \
		0xffe06bdd, 0x6107, 0x46a6,                        \
		{                                                  \
			0x7b, 0xb2, 0x5a, 0x9c, 0x7e, 0xc5, 0x27, 0x5c \
		}                                                  \
	}

#define EFI_LOAD_FILE_PROTOCOL_GUID                        \
	{                                                      \
		0x56EC3091, 0x954C, 0x11d2,                        \
		{                                                  \
			0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b \
		}                                                  \
	}

#define EFI_LOAD_FILE2_PROTOCOL_GUID                       \
	{                                                      \
		0x4006c0c1, 0xfcb3, 0x403e,                        \
		{                                                  \
			0x99, 0x6d, 0x4a, 0x6c, 0x87, 0x24, 0xe0, 0x6d \
		}                                                  \
	}

#define EFI_LOADED_IMAGE_PROTOCOL_GUID                     \
	{                                                      \
		0x5B1B31A1, 0x9562, 0x11d2,                        \
		{                                                  \
			0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B \
		}                                                  \
	}

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID                  \
	{                                                      \
		0x9042a9de, 0x23dc, 0x4a38,                        \
		{                                                  \
			0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a \
		}                                                  \
	}

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID               \
	{                                                      \
		0x0964e5b22, 0x6459, 0x11d2,                       \
		{                                                  \
			0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b \
		}                                                  \
	}

#define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID         \
	{                                                      \
		0xbc62157e, 0x3e33, 0x4fec,                        \
		{                                                  \
			0x99, 0x20, 0x2d, 0x3b, 0x36, 0xd7, 0x50, 0xdf \
		}                                                  \
	}

#define EFI_DEVICE_PATH_PROTOCOL_GUID                     \
	{                                                     \
		0x9576e91, 0x6d3f, 0x11d2,                        \
		{                                                 \
			0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b \
		}                                                 \
	}

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID               \
	{                                                      \
		0x0964e5b22, 0x6459, 0x11d2,                       \
		{                                                  \
			0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b \
		}                                                  \
	}

typedef UInt64(EfiImageUnload)(EfiHandlePtr ImageHandle);

enum
{
	kPixelRedGreenBlueReserved8BitPerColor,
	kPixelBlueGreenRedReserved8BitPerColor,
	kPixelBitMask,
	kPixelBltOnly,
	kPixelFormatMax
};

typedef struct EfiBitmask
{
	UInt32 RedMask;
	UInt32 GreenMask;
	UInt32 BlueMask;
	UInt32 ReservedMask;
} EfiBitmask;

typedef struct
{
	UInt8 Blue;
	UInt8 Green;
	UInt8 Red;
	UInt8 Reserved;
} EfiGraphicsOutputBltPixel;

typedef enum EfiGraphicsOutputProtocolBltOperation
{
	EfiBltVideoFill,
	EfiBltVideoToBltBuffer,
	EfiBltBufferToVideo,
	EfiBltVideoToVideo,
	EfiGraphicsOutputBltOperationMax
} EfiGraphicsOutputProtocolBltOperation;

typedef struct EfiGraphicsOutputProtocolModeInformation
{
	UInt32	   Version;
	UInt32	   HorizontalResolution;
	UInt32	   VerticalResolution;
	UInt32	   PixelFormat;
	EfiBitmask PixelInformation;
	UInt32	   PixelsPerScanLine;
} EfiGraphicsOutputProtocolModeInformation;

typedef UInt64(EFI_API* EfiGraphicsOutputProtocolQueryMode)(
	EfiGraphicsOutputProtocol* Self, UInt32 ModeNumber, UInt32* SizeOfInfo, EfiGraphicsOutputProtocolModeInformation** Info);

typedef UInt64(EFI_API* EfiGraphicsOutputProtocolSetMode)(
	EfiGraphicsOutputProtocol* Self, UInt32 ModeNumber);

typedef UInt64(EFI_API* EfiGraphicsOutputProtocolBlt)(
	EfiGraphicsOutputProtocol* Self, EfiGraphicsOutputBltPixel* BltBuffer, EfiGraphicsOutputProtocolBltOperation BltOperation, UInt32 SourceX, UInt32 SourceY, UInt32 DestinationX, UInt32 DestinationY, UInt32 Width, UInt32 Height, UInt32 Delta);

typedef struct
{
	UInt32									  MaxMode;
	UInt32									  Mode;
	EfiGraphicsOutputProtocolModeInformation* Info;
	UInt32									  SizeOfInfo;
	UIntPtr									  FrameBufferBase;
	UInt32									  FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EfiGraphicsOutputProtocol
{
	EfiGraphicsOutputProtocolQueryMode QueryMode;
	EfiGraphicsOutputProtocolSetMode   SetMode;
	EfiGraphicsOutputProtocolBlt	   Blt;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* Mode;
} EfiGraphicsOutputProtocol;

typedef struct EfiLoadImageProtocol
{
	UInt32			Revision;
	EfiHandlePtr	ParentHandle;
	EfiSystemTable* SystemTable;

	// Source location of the image
	EfiHandlePtr		   DeviceHandle;
	EfiDevicePathProtocol* FilePath;
	Void*				   Reserved;

	// Image’s load options
	UInt32 LoadOptionsSize;
	Void*  LoadOptions;

	// Location where image was loaded
	Void*		   ImageBase;
	UInt64		   ImageSize;
	EfiMemoryType  ImageCodeType;
	EfiMemoryType  ImageDataType;
	EfiImageUnload Unload;
} EfiLoadImageProtocol;

typedef struct EfiLoadFileProtocol
{
	EfiLoadFile LoadFile;
} EfiLoadFileProtocol;

typedef struct EfiDevicePathProtocol
{
	UInt8 Kind;
	UInt8 SubType;
	UInt8 Length[2];
} EfiDevicePathProtocol;

typedef struct EfiFileDevicePathProtocol
{
	EfiDevicePathProtocol Proto;

	///
	///   File Path of this struct
	///
	WideChar Path[kPathLen];
} EfiFileDevicePathProtocol;

typedef UInt64(EFI_API* EfiExitBootServices)(VoidPtr ImageHandle,
											 UInt32	 MapKey);

typedef UInt64(EFI_API* EfiAllocatePages)(EfiAllocateType	  AllocType,
										  EfiMemoryType		  MemType,
										  UInt32			  Count,
										  EfiPhysicalAddress* Memory);

typedef UInt64(EFI_API* EfiFreePages)(EfiPhysicalAddress* Memory, UInt32 Pages);

typedef UInt64(EFI_API* EfiGetMemoryMap)(UInt32*			  MapSize,
										 EfiMemoryDescriptor* DescPtr,
										 UInt32*			  MapKey,
										 UInt32*			  DescSize,
										 UInt32*			  DescVersion);

/**
 * @brief GUID type, something you can also find in CFKit.
 */
typedef struct EfiGUID EFI_FINAL
{
	UInt32 Data1;
	UInt16 Data2;
	UInt16 Data3;
	UInt8  Data4[8];
} EfiGUID;

/***
 * Protocol stuff...
 */

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID                \
	{                                                      \
		0x387477c1, 0x69c7, 0x11d2,                        \
		{                                                  \
			0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b \
		}                                                  \
	}

/** some helpers */
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL		  0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL		  0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER			  0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE			  0x00000020

typedef UInt64(EFI_API* EfiLocateProtocol)(EfiGUID* Protocol,
										   VoidPtr	Registration,
										   VoidPtr* Interface);

typedef UInt64(EFI_API* EfiOpenProtocol)(EfiHandlePtr Handle, EfiGUID* Guid, VoidPtr* Interface, EfiHandlePtr AgentHandle, EfiHandlePtr ControllerHandle, UInt32 Attributes);

typedef UInt64(EFI_API* EfiEnableCursor)(EfiSimpleTextOutputProtocol* Self, Boolean Visible);

/**
@name EfiBootServices
@brief UEFI Boot Services record, it contains functions necessary to a
firmware level application.
*/
typedef struct EfiBootServices
{
	EfiTableHeader		SystemTable;
	VoidPtr				RaiseTPL;
	VoidPtr				RestoreTPL;
	EfiAllocatePages	AllocatePages;
	EfiFreePages		FreePages;
	EfiGetMemoryMap		GetMemoryMap;
	EfiAllocatePool		AllocatePool;
	EfiFreePool			FreePool;
	VoidPtr				CreateEvent;
	VoidPtr				SetTimer;
	VoidPtr				WaitForEvent;
	VoidPtr				SignalEvent;
	VoidPtr				CloseEvent;
	VoidPtr				CheckEvent;
	VoidPtr				InstallProtocolInterface;
	VoidPtr				ReinstallProtocolInterface;
	VoidPtr				UninstallProtocolInterface;
	EfiHandleProtocol	HandleProtocol;
	VoidPtr				Reserved;
	VoidPtr				RegisterProtocolNotify;
	VoidPtr				LocateHandle;
	EfiLocateDevicePath LocateDevicePath;
	VoidPtr				InstallConfigurationTable;
	EfiLoadImage		LoadImage;
	EfiStartImage		StartImage;
	VoidPtr				Exit;
	VoidPtr				UnloadImage;
	EfiExitBootServices ExitBootServices;
	VoidPtr				GetNextMonotonicCount;
	VoidPtr				Stall;
	EfiStatusType(EFI_API* SetWatchdogTimer)(UInt32 Timeout, UInt64 WatchdogCode, UInt32 DataSize, EfiCharType* Data);
	VoidPtr			  ConnectController;
	VoidPtr			  DisconnectController;
	EfiOpenProtocol	  OpenProtocol;
	VoidPtr			  CloseProtocol;
	VoidPtr			  OpenProtocolInformation;
	VoidPtr			  ProtocolsPerHandle;
	VoidPtr			  LocateHandleBuffer;
	EfiLocateProtocol LocateProtocol;
	VoidPtr			  InstallMultipleProtocolInterfaces;
	VoidPtr			  UninstallMultipleProtocolInterfaces;
	EfiCalculateCrc32 CalculateCrc32;
	EfiCopyMem		  CopyMem;
	EfiSetMem		  SetMem;
	VoidPtr			  CreateEventEx;
} EfiBootServices;

#define kEntireDevPath	  0xFF
#define kThisInstancePath 0x01

/**
@brief PrintF like protocol.
*/
typedef struct EfiSimpleTextOutputProtocol
{
	VoidPtr			Reset;
	EfiTextString	OutputString;
	VoidPtr			TestString;
	VoidPtr			QueryMode;
	VoidPtr			SetMode;
	EfiTextAttrib	SetAttribute;
	EfiTextClear	ClearScreen;
	VoidPtr			SetCursorPosition;
	EfiEnableCursor EnableCursor;
	VoidPtr			Mode;
} EfiSimpleTextOutputProtocol;

typedef struct
{
	UInt16		  ScanCode;
	EfiChar16Type UnicodeChar;
} EfiInputKey;

typedef EfiStatusType(EFI_API* EfiInputReadKey)(
	IN EfiSimpleTextInputProtocol* This,
	OUT EfiInputKey* Key);

typedef EfiStatusType(EFI_API* EfiInputReset)(
	IN EfiSimpleTextInputProtocol* This,
	IN Boolean					   ExtendedChk);

typedef EfiStatusType(EFI_API* EfiWaitForEvent)(
	IN UInt32  NumberOfEvents,
	IN VoidPtr Event,
	OUT UInt32* Index);

typedef struct EfiSimpleTextInputProtocol
{
	EfiInputReset	Reset;
	EfiInputReadKey ReadKeyStroke;
	EfiWaitForEvent WaitForKey;
} EfiSimpleTextInputProtocol;

/// @biref Open Volume procedure ptr.
typedef UInt64(EFI_API* EfiOpenVolume)(struct EfiSimpleFilesystemProtocol*,
									   struct EfiFileProtocol**);

struct EfiSimpleFilesystemProtocol
{
	UInt64		  Revision;
	EfiOpenVolume OpenVolume;
};

/**
@brief The Structure that they give you when booting.
*/
typedef struct EfiSystemTable
{
	EfiTableHeader				 SystemHeader;
	WideChar*					 FirmwareVendor;
	UInt32						 FirmwareRevision;
	EfiHandlePtr				 ConsoleInHandle;
	EfiSimpleTextInputProtocol*	 ConIn;
	EfiHandlePtr				 ConsoleOutHandle;
	EfiSimpleTextOutputProtocol* ConOut;
	EfiHandlePtr				 StandardErrorHandle;
	VoidPtr						 StdErr;
	VoidPtr						 RuntimeServices;
	EfiBootServices*			 BootServices;
	UInt64						 NumberOfTableEntries;
	/// The configuration table (contains the RSD PTR entry.)
	struct
	{
		EfiGUID VendorGUID;
		VoidPtr VendorTable;
	} * ConfigurationTable;
} EfiSystemTable;

#define kEfiOk			0
#define kEfiFail		-1
#define kBufferTooSmall 5

#define EFI_EXTERN_C extern "C"

typedef struct EfiIPV4
{
	UInt8 Addr[4];
} EfiIPV4;

///
/// 16-byte buffer. An IPv6 internet protocol address.
///
typedef struct EfiIPV6
{
	UInt8 Addr[16];
} EfiIPV6;

#define kEFIYellow (0x01 | 0x02 | 0x04 | 0x08)

#ifdef __x86_64
#define __EFI_x86_64__ 1
#endif // __x86_64

enum
{
	kEFIHwDevicePath	   = 0x01,
	kEFIAcpiDevicePath	   = 0x02,
	kEFIMessaingDevicePath = 0x03,
	kEFIMediaDevicePath	   = 0x04,
	kEFIBiosBootPath	   = 0x05,
	kEFIEndOfPath		   = 0x06,
	kEFICount			   = 6,
};

#define END_DEVICE_PATH_TYPE			 0x7f
#define END_ENTIRE_DEVICE_PATH_SUBTYPE	 0xFF
#define END_INSTANCE_DEVICE_PATH_SUBTYPE 0x01

#define kEfiOffsetOf(T, F) __builtin_offsetof(T, F)

/// File I/O macros

#define kEFIFileRead   0x0000000000000001
#define kEFIFileWrite  0x0000000000000002
#define kEFIFileCreate 0x0000000000000000

#define kEFIReadOnly  0x01
#define kEFIHidden	  0x02
#define kEFISystem	  0x04
#define kEFIReserved  0x08
#define kEFIDirectory 0x10
#define kEFIArchive	  0x20

#define EFI_FILE_PROTOCOL_REVISION		  0x00010000
#define EFI_FILE_PROTOCOL_REVISION2		  0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

#define EFI_EXTRA_DESCRIPTOR_SIZE 8

#define EFI_MP_SERVICES_PROTOCOL_GUID  \
	{                                  \
		0x3fdda605, 0xa76e, 0x4f46,    \
		{                              \
			0xad, 0x29, 0x12, 0xf4,    \
				0x53, 0x1b, 0x3d, 0x08 \
		}                              \
	}

#define PROCESSOR_AS_BSP_BIT		0x00000001
#define PROCESSOR_ENABLED_BIT		0x00000002
#define PROCESSOR_HEALTH_STATUS_BIT 0x00000004

#define END_OF_CPU_LIST 0xffffffff

typedef struct EfiIOToken
{
	//
	// If Event is NULL, then blocking I/O is performed.
	// If Event is not NULL and non-blocking I/O is supported, then non-blocking
	// I/O is performed, and Event will be signaled when the read request is
	// completed. The caller must be prepared to handle the case where the
	// callback associated with Event occurs before the original asynchronous I/O
	// request call returns.
	//
	UInt64 Event;

	//
	// Defines whether or not the signaled event encountered an error.
	//
	UInt64 Status;

	//
	// For OpenEx():  Not Used, ignored.
	// For ReadEx():  On input, the size of the Buffer. On output, the amount of
	// data returned in Buffer.
	//                In both cases, the size is measured in bytes.
	// For WriteEx(): On input, the size of the Buffer. On output, the amount of
	// data actually written.
	//                In both cases, the size is measured in bytes.
	// For FlushEx(): Not used, ignored.
	//
	UInt32 BufferSize;

	//
	// For OpenEx():  Not Used, ignored.
	// For ReadEx():  The buffer into which the data is read.
	// For WriteEx(): The buffer of data to write.
	// For FlushEx(): Not Used, ignored.
	//
	Void* Buffer;
} EfiIOToken;

typedef struct EfiFileProtocol
{
	UInt64 Revision;

	EfiStatusType(EFI_API* Open)(struct EfiFileProtocol*  Self,
								 struct EfiFileProtocol** Out,
								 EfiCharType*			  CharType,
								 UInt64					  OpenMode,
								 UInt64					  Attrib);

	EfiStatusType(EFI_API* Close)(struct EfiFileProtocol* Self);

	EfiStatusType(EFI_API* Delete)(struct EfiFileProtocol* Self);

	EfiStatusType(EFI_API* Read)(struct EfiFileProtocol* Self, UInt64* BufSize, VoidPtr BufOut);

	EfiStatusType(EFI_API* Write)(struct EfiFileProtocol* Self, UInt64* BufSize, VoidPtr BufOut);

	EfiStatusType(EFI_API* GetPosition)(EfiFileProtocol* Self, UInt64* Position);

	EfiStatusType(EFI_API* SetPosition)(EfiFileProtocol* Self, UInt64* Position);

	EfiStatusType(EFI_API* GetInfo)(struct EfiFileProtocol*, struct EfiGUID*, UInt32*, void*);

	EfiStatusType(EFI_API* SetInfo)(struct EfiFileProtocol*, struct EfiGUID*, UInt32*, void*);

	EfiStatusType(EFI_API* Flush)(EfiFileProtocol*);

	EfiStatusType(EFI_API* OpenEx)(EfiFileProtocol*	  Self,
								   EfiFileProtocol**  OutHandle,
								   EfiCharType*		  Path,
								   UInt64			  Mode,
								   UInt64			  Attrib,
								   struct EfiIOToken* Token);

	EfiStatusType(EFI_API* ReadEx)(EfiFileProtocol*	  Self,
								   struct EfiIOToken* Token);

	EfiStatusType(EFI_API* WriteEx)(EfiFileProtocol*   Self,
									struct EfiIOToken* Token);

	EfiStatusType(EFI_API* FlushEx)(EfiFileProtocol*   Self,
									struct EfiIOToken* Token);
} EfiFileProtocol, *EfiFileProtocolPtr;

typedef UInt64 EfiCursorType;

typedef struct EfiTime
{
	UInt16 Year;
	UInt8  Month;
	UInt8  Day;
	UInt8  Hour;
	UInt8  Minute;
	UInt8  Second;
	UInt8  Pad1;
	UInt32 Nanosecond;
	Int16  TimeZone;
	UInt8  Daylight;
	UInt8  Pad2;
} EfiTime;

#define EFI_FILE_INFO_GUID                                 \
	{                                                      \
		0x09576e92, 0x6d3f, 0x11d2,                        \
		{                                                  \
			0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b \
		}                                                  \
	}

struct EfiFileInfo EFI_FINAL
{
	/// @brief Structure size.
	UInt64 Size;
	/// @brief File size.
	UInt64 FileSize;
	/// @brief Physical size on disk.
	UInt64 PhysicalSize;
	/// @brief Create time.
	EfiTime CreateTime;
	/// @brief Last access time.
	EfiTime LastAccessTime;
	/// @brief Edit time.
	EfiTime EditTime;
	/// @brief Attributes.
	UInt64 Attribute;
	/// @brief VLA file name.
	WideChar FileName[1];
};

//*******************************************************
// EFI_CPU_PHYSICAL_LOCATION
// @note As in the EFI specs.
//*******************************************************
typedef struct _EfiCPUPhyiscalLocation
{
	UInt32 Package;
	UInt32 Core;
	UInt32 Thread;
} EfiCPUPhyiscalLocation;

typedef union _EfiExtendedProcessorInformation {
	EfiCPUPhyiscalLocation Location2;
} EfiExtendedProcessorInformation;

typedef struct _EfiProcessorInformation
{
	UInt64							ProcessorId;
	UInt32							StatusFlag;
	EfiCPUPhyiscalLocation			Location;
	EfiExtendedProcessorInformation ExtendedInformation;
} EfiProcessorInformation;

typedef EfiStatusType EFI_API (*EfiMpServicesGetNumberOfProcessors)(
	IN struct _EfiMpServicesProtocol* Self,
	OUT UInt32* NumberOfProcessors,
	OUT UInt32* NumberOfEnabledProcessors);

typedef EfiStatusType EFI_API (*EfiMpServicesGetProcessorInfo)(
	IN struct _EfiMpServicesProtocol* Self,
	IN UInt32*							 ProcessorNumber,
	OUT struct _EfiProcessorInformation* NumberOfEnabledProcessors);

typedef void EFI_API (*EFI_AP_PROCEDURE)(
	IN VoidPtr ProcedureArgument);

typedef EfiStatusType EFI_API (*EfiMpServicesStartupAllAPS)(
	IN struct _EfiMpServicesProtocol* Self,
	IN EFI_AP_PROCEDURE				  Procedure,
	IN Boolean						  SingleThread,
	IN VoidPtr WaitEvent			  OPTIONAL, // EFI_EVENT first, but unused here.
	IN UInt32						  TimeoutInMicroSeconds,
	IN Void* ProcedureArgument OPTIONAL,
	OUT UInt32** FailedCpuList OPTIONAL);

typedef EfiStatusType EFI_API (*EfiMpServicesSwitchBSP)(
	IN struct _EfiMpServicesProtocol* Self,
	IN UInt32						  ProcessorNumber,
	IN Boolean						  EnableOldBSP);

typedef EfiStatusType EFI_API (*EfiMpServicesStartupThisAP)(
	IN struct _EfiMpServicesProtocol* Self,
	IN EFI_AP_PROCEDURE				  Procedure,
	IN UInt32						  ProcessorNumber,
	IN VoidPtr WaitEvent			  OPTIONAL,
	IN UInt32						  TimeoutInMicroseconds,
	IN Void* ProcedureArgument OPTIONAL,
	OUT Boolean* Finished OPTIONAL);

typedef EfiStatusType EFI_API (*EfiMpServicesDisableThisAP)(
	IN struct _EfiMpServicesProtocol* Self,
	IN UInt32						  ProcessorNumber,
	IN Boolean						  EnableAP,
	IN UInt32* HealthFlag OPTIONAL);

typedef EfiStatusType EFI_API (*EfiMpServicesWhoAmI)(
	IN struct _EfiMpServicesProtocol* Self,
	OUT UInt32* ProcessorNumber);

typedef struct _EfiMpServicesProtocol
{
	EfiMpServicesGetNumberOfProcessors GetNumberOfProcessors;
	EfiMpServicesGetProcessorInfo	   GetProcessorInfo;
	EfiMpServicesStartupAllAPS		   StartupAllAPs;
	EfiMpServicesStartupThisAP		   StartupThisAP;
	EfiMpServicesSwitchBSP			   SwitchBSP;
	EfiMpServicesDisableThisAP		   EnableDisableAP;
	EfiMpServicesWhoAmI				   WhoAmI;
} EfiMpServicesProtocol;

#endif // ifndef FIRMWARE_KIT_EFI_H
