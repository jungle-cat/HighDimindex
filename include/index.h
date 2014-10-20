/*
 * index.h
 *
 *  Created on: 2014-10-20
 *      Author: Feng
 */

#ifndef __INDEX_H__
#define __INDEX_H__

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>

#include "config.h"

namespace hash{

using std::vector;
using std::pair;
using std::deque;

template <typename KeyT, typename ValueT>
using hash_map = std::unordered_map<KeyT, ValueT>;

template <typename InfoType>
class FastRetriever
{
	public:
		typedef pair<InfoType, int>      candidate_type;
		typedef deque<InfoType>          infolist_type;
		typedef hash_t                   code_type;

		FastRetriever(int maxdist, int codelen);

		bool insert(const InfoType& v);
		vector<candidate_type> query(const code_type& code);
		vector<candidate_type> filter_by_dist(vector<candidate_type>&& candidates, int maxdist);
		vector<candidate_type> filter_by_num(vector<candidate_type>&& candidates, int maxnum);

	private:
		int code_length;
		int ntable;
		vector<hash_map<code_type, infolist_type> >  hashtables;
};


}


#include "index-imp.h"


#endif // __INDEX_H__
