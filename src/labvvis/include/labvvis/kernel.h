/*
 *  kernel.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sat Jan 17 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _LABVVIS_KERNEL_H
#define _LABVVIS_KERNEL_H

namespace labvvis {
	template<typename T> class kernel2d {
	public:
		typedef T value_type;
		typedef const value_type* const_iterator;
		typedef value_type* iterator;
		
		kernel2d()
		: _width(0), _height(0), _left(0), _top(0), _kernel(0) {
		}
		kernel2d(const kernel2d& rhs)
		: _width(0), _height(0), _kernel(0) {
			*this = rhs;
		}
		kernel2d(value_type* kernel, const int width, const int height) {
			allocate(width, height);
			_sum = 0;
			// Copy kernel in
			for(int y = 0, i = 0; y < height; ++y) {
				for(int x = 0; x < width; ++x, ++i) {
					_kernel[i] = kernel[i];
					_sum += kernel[i];
				}
			}
			if(_sum < 1)
				_sum = 1;
			// Assume center is at center
			_top = _height / 2;
			_left = _width / 2;
		}
		~kernel2d() {
			deallocate();
		}
		kernel2d& operator=(const kernel2d& rhs) {
			// Check if the same object
			if(&rhs == this)
				return *this;
			// Deallocate memory, then make copy
			if(rhs._width != _width || rhs._height != _height) {
				deallocate();
				allocate(rhs._width, rhs._height);
				memcpy(_kernel, rhs._kernel, _width*_height*sizeof(value_type));
			}
			// Copy fields
			_sum = rhs._sum;
			_left = rhs._left;
			_top = rhs._top;
			return *this;
		}
	public:
		const int left() const {
			return _left;
		}
		const int top() const {
			return _top;
		}
		const int width() const {
			return _width;
		}
		const int height() const {
			return _height;
		}
	public:
		iterator begin() {
			return _kernel;
		}
		const_iterator begin() const {
			return _kernel;
		}
		iterator end() {
			return &_kernel[_width*_height];
		}
		const_iterator end() const {
			return &_kernel[_width*_height];
		}
		const value_type sum() const {
			return _sum;
		}
	private:
		void allocate(const int width, const int height) {
			// Keep width and height
			_width = width;
			_height = height;
			// Allocate memory for kernel
			_kernel = new value_type[width*height];
		}
		void deallocate() {
			if(_kernel)
				delete[] _kernel;
		}
	private:
		int _width, _height, _left, _top;
		value_type* _kernel;
		value_type _sum;
	};
} // End of vvis namespace

#endif