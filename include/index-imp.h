/*
 * index-imp.h
 *
 *  Created on: 2014-10-20
 *      Author: Feng
 */

#ifndef __INDEX_IMP_H__
#define __INDEX_IMP_H__


namespace hash{

template< typename ValueT, typename HashT = std::hash<ValueT>, typename PredT=std::equal_to<ValueT> >
using hash_set = std::unordered_set<ValueT, HashT, PredT>;


template<typename InfoType>
inline FastRetriever<InfoType>::FastRetriever(int _maxdist, int codelen)
	: code_length(codelen), ntable(_maxdist+1)
{
}

template <typename InfoType>
bool FastRetriever<InfoType>::insert(const InfoType& v)
{
	typedef FastRetriever<InfoType>::code_type code_type;

	int epoch = std::ceil(code_length / static_cast<float>(ntable));
	code_type init = 0x01, c;;
	for(int i = 0, j = 0; i < ntable; ++i) {
		c = 0;
		int start = epoch*i;
		int end = std::min(start+epoch, code_length);
		while(j++ < end) {
			c |= ((v.code & (init << j)) == 0) ? 0 : (init << (end-start));
		}

		hashtables[i][c].push_back(v);
	}
	return true;
}

template<typename InfoType>
struct InfoTypeHash
{
	std::size_t operator()(const InfoType& x) const noexcept
	{
		return static_cast<std::size_t>(x.id);
	}
};

template<typename InfoType>
struct InfoTypeEqual
{
	bool operator()(const InfoType& x, const InfoType& y) const noexcept
	{
		return x.id == y.id;
	}
};

template <typename InfoType>
vector<typename FastRetriever<InfoType>::candidate_type> FastRetriever<InfoType>::query(const typename FastRetriever<InfoType>::code_type& code)
{
	typedef typename FastRetriever<InfoType>::candidate_type candidate_type;
	typedef typename FastRetriever<InfoType>::code_type      code_type;

	hash_set<InfoType, InfoTypeHash<InfoType>, InfoTypeEqual<InfoType> > candidates;

	int epoch = std::ceil(code_length / static_cast<float>(ntable));
	code_type init = 0x01, c;;
	for(int i = 0, j = 0; i < ntable; ++i) {
		c = 0;
		int start = epoch*i;
		int end = std::min(start+epoch, code_length);

		while(j++ < end) {
			c |= ((code & (init << j)) == 0) ? 0 : (init << (end-start));
		}

		auto&& ret = hashtables[i][c];
		if (!ret.empty()) {
			for(int k = 0; k < ret.size(); ++k)
				candidates.insert(ret[k]);
		}
	}

	vector<candidate_type> results;
	for(auto it = candidates.begin(); it != candidates.end(); ++it) {
		results.push_back(std::make_pair(*it, 0));
	}

	return std::move(results);
}

template <typename InfoType>
vector<typename FastRetriever<InfoType>::candidate_type> FastRetriever<InfoType>::filter_by_dist(vector<typename FastRetriever<InfoType>::candidate_type>&& candidates, int maxdist)
{
	auto it = candidates.begin();
	for(auto first = candidates.begin(); first != candidates.end(); ++first) {
		if (first->second >= maxdist && it != first) {
			*it++ = *first;
		}
	}
	candidates.erase(it, candidates.end());

	return std::move(candidates);
}

template <typename InfoType>
vector<typename FastRetriever<InfoType>::candidate_type> FastRetriever<InfoType>::filter_by_num(vector<typename FastRetriever<InfoType>::candidate_type>&& candidates, int maxnum)
{
	std::sort(candidates.begin(), candidates.end(), [](decltype(candidates[0]) x, decltype(candidates[0]) y){return x->second < y->second;});
	int i = 0;
	int prev_dist;
	for(; i < candidates.size(); ++i) {
		if(i > maxnum && candidates[i].second == prev_dist)
			break;
		prev_dist = candidates[i];
	}

	candidates.erase(candidates.begin()+i, candidates.end());
	return std::move(candidates);
}

}
#endif // __INDEX_IMP_H__
