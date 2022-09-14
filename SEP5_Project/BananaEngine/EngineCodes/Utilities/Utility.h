#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include "../Precompiled/pch.h"

namespace BE
{
	//Unreferenced parameters
	#define UNUSED(x) (void)(x)

	//Compilation macros
	#define USE_EDITOR			//Build with editor
	#define USE_DEBUGGER		//Build with debugger

	//Sizes
	constexpr int MAX_STR_BUFFER = 256;


	//Helper function to convert filepath slash from \\ to /
	void ConvertFilePathSlash(std::string& _fp);

	//Helper function to truncate filepath to the last part
	void TruncateFilePath(std::string& _fp);
}

#endif