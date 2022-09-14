#pragma once
#ifndef TESTFILE_H
#define TESTFILE_H

#include <Precompiled/pch.h>

namespace BE
{
	class TestFile
	{
		int* values;
		size_t size;
		double dd;

	public:
		TestFile(const std::string& _fp, size_t sz, double d)
		{
			values = new int[sz];
			size = sz;
			dd = d;

			std::cout << _fp << " " << sz << " " << d << std::endl;

			for (int i = 0; i < sz; ++i)
				values[i] = i;
		}

		TestFile(TestFile&& rhs) noexcept
		{
			values = rhs.values;
			size = rhs.size;
			dd = rhs.dd;

			rhs.values = nullptr;
			rhs.size = 0;
		}

		TestFile& operator=(TestFile&& rhs) noexcept
		{
			this->~TestFile();

			values = rhs.values;
			size = rhs.size;
			dd = rhs.dd;

			rhs.values = nullptr;
			rhs.size = 0;

			return *this;
		}


		~TestFile()
		{
			delete[] values;
		}

		void Render()
		{
			for (size_t i = 0; i < size; ++i)
				std::cout << values[i] << " ";

			std::cout << std::endl;
		}
	};

}

#endif