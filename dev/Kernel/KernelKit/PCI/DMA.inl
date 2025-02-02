/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrous, all rights reserved.

------------------------------------------- */

namespace Kernel
{
	template <class T>
	T* DMAWrapper::operator->()
	{
		return fAddress;
	}

	template <class T>
	T* DMAWrapper::Get(const UIntPtr offset)
	{
		return reinterpret_cast<T*>((UIntPtr)fAddress + offset);
	}
} // namespace Kernel
