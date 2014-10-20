/*
 * hash.h
 *
 *  Created on: 2014-10-20
 *      Author: Feng
 */

#ifndef __HASH_H__
#define __HASH_H__

#include "config.h"


#include <string>
#include <opencv/cv.h>


namespace hash{

using std::string;
using cv::Mat_;




class HashBuilder
{
	public:
		typedef hash_t code_type;
		typedef unsigned int size_type;
		virtual ~HashBuilder() {};

		virtual bool load(const string& file) = 0;
		virtual code_type hash(float* data, int len) const = 0;
		virtual size_type length() const = 0;
};



class KernelHash : public HashBuilder
{
	public:
		typedef HashBuilder::code_type code_type;
		typedef HashBuilder::size_type size_type;

		KernelHash();
		virtual ~KernelHash();

		virtual bool load(const string& file);
		virtual code_type hash(float* data, int len) const;
		virtual size_type length() const;

	private:
		float sigma;
		Mat_<float> mvec;
		Mat_<float> anchors;
		Mat_<float> A1;
};

}
#endif // __HASH_H__
