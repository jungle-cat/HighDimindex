/*
 * kernelhash.cpp
 *
 *  Created on: 2014-10-20
 *      Author: Feng
 */

#include "../include/hash.h"

using namespace hash;

KernelHash::KernelHash()
	: sigma(-1)
{}

KernelHash::~KernelHash()
{}

bool KernelHash::load(const string& fn)
{
	cv::FileStorage fs(fn, cv::FileStorage::READ);
	if (!fs.isOpened()) {
		return false;
	}

	fs["sigma"] >> this->sigma;
	fs["mvec"] >> this->mvec;
	fs["anchors"] >> this->anchors;
	fs["A1"] >> this->A1;

	return sigma >= 0 && !mvec.empty() && !anchors.empty() && !A1.empty();
}

KernelHash::code_type KernelHash::hash(float* data, int len) const
{
	Mat_<float> feature = Mat_<float>(1, len, data);
	const int nanchors = anchors.rows;
	Mat_<float> dist(1, nanchors);
	for (int i = 0; i < nanchors; ++i) {
		double l2 = norm(feature, anchors.row(i), cv::NORM_L2);
		dist(i) = l2*l2;
	}

	Mat_<float> p = -dist/(2*sigma);
	cv::exp(p, dist);
	dist -= mvec;

	Mat_<unsigned char> ret = static_cast<Mat_<unsigned char> >(dist * A1 > 0);
	int r = A1.cols;

	KernelHash::code_type init = 0x01, code = 0x0;
	for(int i = 0; i < r; ++i)
		if(ret(i)) code |= init<<i;

	return code;
}


KernelHash::size_type KernelHash::length() const
{
	return A1.cols;
}
