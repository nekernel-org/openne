/* -------------------------------------------

	Copyright (C) 2024, Amlal EL Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <NewKit/Defines.h>
#include <KernelKit/Timer.h>
#include <CompilerKit/CompilerKit.h>

namespace Kernel
{
	class UserProcess;

	typedef UserProcess& UserProcessRef;

	/// @brief Access control class, which locks a task until one is done.
	class Semaphore final
	{
	public:
		explicit Semaphore() = default;
		~Semaphore()		 = default;

	public:
		bool IsLocked() const;
		bool Unlock() noexcept;

	public:
		void WaitForProcess() noexcept;

	public:
		bool Lock(UserProcess& process);
		bool LockOrWait(UserProcess& process, TimerInterface* timer);

	public:
		OPENNE_COPY_DEFAULT(Semaphore);

	private:
		UserProcessRef fLockingProcess;
	};
} // namespace Kernel
