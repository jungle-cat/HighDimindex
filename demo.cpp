/*
 * demo.cpp
 *
 *  Created on: 2014-10-20
 *      Author: Feng
 */

#include "include/index.h"
#include "include/hash.h"


using namespace hash;


struct info_type
{
	int id;
	HashBuilder::code_type code;
};


int main()
{
	string hashconfig = "";


	// initialize hasher for hash code generation
	KernelHash hasher;
	hasher.load(hashconfig);	// initialize hash from config file
	int codelen = hasher.length();

	// initialize fast indexing for retrieving
	const int nhashtables = 5;
	FastRetriever<info_type> retriever(nhashtables, codelen);

	// type aliasing
	typedef HashBuilder::code_type                   code_type;
	typedef FastRetriever<info_type>::candidate_type candidate_type;

	// insert
	info_type t = {0, 0};
	retriever.insert(t);

	// retrieve
	float feature[128] = {0};
	code_type c = hasher.hash(feature, sizeof(feature)/sizeof(float));
	auto results = retriever.query(c);

	return 0;
}

