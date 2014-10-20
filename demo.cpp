/*
 * demo.cpp
 *
 *  Created on: 2014-10-20
 *      Author: Feng
 */

#include "include/index.h"
#include "include/hash.h"


using namespace hash;

#include <iostream>

/*!
 * @brief User specified type for insert into indexing database.
 *        the member names, i.e. @id and @code, shall not change.
 */
struct info_type
{
	int id;
	HashBuilder::code_type code;
};

/*!
 * @brief Output info_type to the output stream.
 * @param os
 * @param v
 * @return
 */
std::ostream& operator<<(std::ostream& os, const info_type& v)
{
	os << "(" << v.id << ", " << v.code << ")";
	return os;
}


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

	// print results
	for (auto it = results.begin(); it != results.end(); ++it) {
		std::cout << it->first << " ";
	}
	std::cout << std::endl;
	return 0;
}

