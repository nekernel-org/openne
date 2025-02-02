/* -------------------------------------------

	Copyright (C) 2024-2025, Amlal EL Mahrous, all rights reserved.

------------------------------------------- */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <Hints/CompilerHint.h>
#include <NewKit/Defines.h>
#include <NewKit/ErrorOr.h>

namespace Kernel
{
	/// @brief This interface is used to make loader contracts (MSCOFF, PEF).
	/// @author @Amlal-El-Mahrouss
	class LoaderInterface
	{
	public:
		explicit LoaderInterface() = default;
		virtual ~LoaderInterface() = default;

		OPENNE_COPY_DEFAULT(LoaderInterface);

	public:
		virtual _Output ErrorOr<VoidPtr> GetBlob()												= 0;
		virtual _Output const Char* AsString()													= 0;
		virtual _Output const Char* MIME()														= 0;
		virtual _Output const Char* Path()														= 0;
		virtual _Output ErrorOr<VoidPtr> FindStart()											= 0;
		virtual _Output VoidPtr			 FindSymbol(_Input const Char* name, _Input Int32 kind) = 0;
	};
} // namespace Kernel
