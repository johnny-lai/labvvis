#ifndef _VVIS_TRANSFORM_H
#define _VVIS_TRANSFORM_H

#include <vvm/vvm.h>

namespace vvis
{

//==============================================================================
// Algorithms
//------------------------------------------------------------------------------
template<typename T, typename SrcImageIterator>
void _loadFrontEdge(vvm::vector<T> &iv, vvm::vector<T> &ixtr, const SrcImageIterator *&pi, const int ioffset, const int roffset)
{
	const int scalar_count = vvm::vector_traits<vvm::vector<T> >::scalar_count;
	// roffset - step = -ooffset
	vvm::vector_uchar ifix = vvm::lvsl(roffset - scalar_count, pi);
	// Load input
	if(roffset + ioffset < vvm::vector<T>::step)
	{ // Loading from previous and first vector
		vvm::vector<T> iprev = vvm::ld(-16, reinterpret_cast<const vvm::vector<T> *>(pi));
		iv = vvm::perm(iprev, ixtr, ifix);
	}
	else
	{ // Loading from first and next vector
		vvm::vector<T> inext = vvm::ld(16, reinterpret_cast<const vvm::vector<T> *>(pi));
		iv = vvm::perm(ixtr, inext, ifix);
		// Need to advance pi because first vector has already been used completely
		pi += scalar_count;
		ixtr = inext;
	}
}
	
/** This algorithm applies one-to-one correlation functors.
 */
template<typename srcImageIteratorT, typename destImageIteratorT, typename functorT>
void transform(srcImageIteratorT begin, srcImageIteratorT end, destImageIteratorT out, functorT f) {
	typedef typename srcImageIteratorT::value_type src_scalar_type;
	typedef typename destImageIteratorT::value_type dest_scalar_type;
	typedef vvm::vector<typename srcImageIteratorT::value_type> src_vector_type;
	typedef vvm::vector<typename destImageIteratorT::value_type> dest_vector_type;

	src_vector_type iv, ixtr, inext, iprev;
	dest_vector_type ov;
	
	vvm::vector_uchar ifix;

	const int scalar_count = vvm::vector_traits<vvm::vector<T> >::scalar_count;
	typename SrcImageIterator::difference_type count = end - begin;

	// Create pointers for walking through the input and output
	const src_scalar_type *pi = begin;
	src_scalar_type *po = out;

	//dst(inp, 0x10010100, 0);

	// Load location = input offset + (scalar count - output offset)
	// % steps required because only want values 0 to scalar count - 1
	int ioffset = (reinterpret_cast<unsigned long>(pi) & 0xf);
	int ooffset = (reinterpret_cast<unsigned long>(po) & 0xf);
	int roffset = ((scalar_count - ooffset) % scalar_count);

	// Load the first position
	ixtr = vvm::ld(0, reinterpret_cast<const src_vector_type*>(pi));

	// Do front with the vector processor
	// The number of elements needed to be processed is roffset
	if(roffset) {
		// Load Input
		_loadFrontEdge(iv, ixtr, pi, ioffset, roffset);
		// Load output
		ov = vvm::ld(0, reinterpret_cast<const dest_vector_type*>(po));
		// Calculate result and graft back
		vvm::vector_uchar graft(ooffset);
		graft = vvm::lvsl(0, reinterpret_cast<vvm::vector_uchar*>(0)) >= graft;
		vvm::st(vvm::sel(ov, f(iv), graft), 0, po);
		// Advance output
		po += step;
	}

	// Do Initial Load
	ifix = vvm::lvsl(roffset, pi);
	pi += step;

	// Do middle with vector processor
	// For upper bound, (- step) because want to avoid walking past end
	//                  <= because want to use the middle loop for optimal
	int i;
	for(i = roffset; i <= count - scalar_count; i += scalar_count)
	{
		// vec_dst(pi, 0x10010100, 0);
		// Load unaligned
		iv = ixtr;
		ixtr = vvm::ld(0, reinterpret_cast<const src_vector_type*>(pi));
		pi += step;
		iv = vvm::perm(iv, ixtr, ifix);
		// Do operation
		ov = f(iv);
		// Write aligned
		vvm::st(ov, 0, po);
		po += scalar_count;
	}

	// Do end with vector processor
	if(i < count)
	{
		vvm::vector_uchar graft(count - i);
		ov = vvm::ld(0, reinterpret_cast<const dest_vector_type*>(po));
		iv = vvm::ld(0, reinterpret_cast<const src_vector_type*>(pi));
		iv = vvm::perm(ixtr, iv, ifix);
		graft = vvm::lvsl(0, reinterpret_cast<vvm::vector_uchar*>(0)) < graft;
		ov = vvm::sel(ov, f(iv), graft);
		vvm::st(ov, 0, po);
	}
}

/** Transform algorithm for two-to-one functors
 */
template<typename T, typename F>
void transform(const T *begin1, const T *end1, const T* begin2, T *out, F f)
{
	typedef vvm::vector<T> V;

	V ov;
	V iv1, ixtr1, inext1, iprev1;
	V iv2, ixtr2, inext2, iprev2;
	vvm::vectorUChar ifix1, ifix2;

	const int step = V::step;
	int count = end1 - begin1;

	// Create pointers for walking through the input and output
	const T *pi1 = begin1;
	const T *pi2 = begin2;
	T *po = out;

	//dst(inp, 0x10010100, 0);

	// Load location = input offset + (scalar count - output offset)
	// % steps required because only want values 0 to scalar count - 1
	int ioffset1 = ((unsigned long)pi1 & 0xf);
	int ioffset2 = ((unsigned long)pi2 & 0xf);
	int ooffset = ((unsigned long)po & 0xf);
	int roffset = ((step - ooffset) % step);

	// Load the first position
	ixtr1 = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(pi1));
	ixtr2 = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(pi2));

	// Do front with the vector processor
	// The number of elements needed to be processed is roffset
	if(roffset)
	{
		// Load input
		_loadFrontEdge(iv1, ixtr1, pi1, ioffset1, roffset);
		_loadFrontEdge(iv2, ixtr2, pi2, ioffset2, roffset);
		// Load output
		ov = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(po));
		// Calculate result and graft back
		vvm::vectorUChar graft(ooffset);
		graft = vvm::lvsl(0, reinterpret_cast<const vvm::vector<T> *>(0)) >= graft;
		vvm::st(vvm::sel(ov, f(iv1, iv2), graft), 0, po);
		// Advance output
		po += step;
	}

	// Do Initial Load
	ifix1 = vvm::lvsl(roffset, pi1); pi1 += step;
	ifix2 = vvm::lvsl(roffset, pi2); pi2 += step;

	// Do middle with vector processor
	// For upper bound, (- step) because want to avoid walking past end
	//                  <= because want to use the middle loop for optimal
	int i;
	for(i = roffset; i <= count - step; i += step)
	{
		// vec_dst(pi, 0x10010100, 0);
		// Load unaligned input 1
		iv1 = ixtr1;
		ixtr1 = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(pi1));
		pi1 += step;
		iv1 = vvm::perm(iv1, ixtr1, ifix1);
		// Load unaligned input 2
		iv2 = ixtr2;
		ixtr2 = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(pi2));
		pi2 += step;
		iv2 = vvm::perm(iv2, ixtr2, ifix2);
		// Do operation
		ov = f(iv1, iv2);
		// Write aligned
		vvm::st(ov, 0, po);
		po += step;
	}

	// Do end with vector processor
	if(i < count)
	{
		vvm::vectorUChar graft(count - i);
		ov = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(po));
		iv1 = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(pi1));
		iv1 = vvm::perm(ixtr1, iv1, ifix1);
		iv2 = vvm::ld(0, reinterpret_cast<const vvm::vector<T> *>(pi2));
		iv2 = vvm::perm(ixtr2, iv2, ifix2);
		graft = vvm::lvsl(0, reinterpret_cast<const vvm::vector<T> *>(0)) < graft;
		ov = vvm::sel(ov, f(iv1, iv2), graft);
		vvm::st(ov, 0, po);
	}
}


} // End of vvis namespace

#endif
