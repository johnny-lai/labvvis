/* Call Library source file */

#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <labvvis/labvvis.h>
#include <labvvis/image_manager.h>
#include <labvvis/object_manager.h>
#include <labvvis/util.h>

#import "SequenceGrabber.h"

#include <Accelerate/Accelerate.h>

#include <labvvis/functor.h>

using namespace lv;

typedef labvvis::hough<UInt8> hough_t;
typedef object_manager<hough_id_t, hough_t> houghs_object_manager;
houghs_object_manager houghs;

#define PRE_FLIGHT(LSTR) \
if(lv::lstrlen(LSTR)) \
return;

#define CHECK_CHANNEL(IID, CID, LSTR) \
if(!image_manager::instance().image_exists(IID)) { \
	lv::to_lstr(LSTR, "Non-existant image referenced"); \
		return; \
} \
if(CID >= image_manager::instance().image(IID).channel_count()) { \
	lv::to_lstr(LSTR, "Non-existant channel referenced"); \
		return; \
}

#define CHECK_CHANNEL_COUNT(IID1, CID1, IID2, CID2, LSTR) { \
	if(CID1 < 0 && CID2 < 0) { \
		int icc = CID1 < 0 ? image_manager::instance().image(IID1).channel_count() : 1; \
		int occ = CID2 < 0 ? image_manager::instance().image(IID2).channel_count() : 1; \
		if(icc != occ) { \
			lv::to_lstr(LSTR, "Number of channels in source and destination images do not match"); \
			return; \
		} \
	} \
}

namespace labvvis {
    template<typename functorT>
    void for_each(const vImage_Buffer &in, functorT f) {
        UInt8 *in_data = (UInt8*)in.data;
        for (int y = 0; y < in.height; ++y) {
            for (int x = 0; x < in.width; ++x) {
                UInt8 in_pixel = in_data[(in.rowBytes * y) + x];
                f(in_pixel);
            }
        }
    }
    
    template<typename functorT>
    void for_each_xy(const vImage_Buffer &in, functorT &f) {
        UInt8 *in_data = (UInt8*)in.data;
        for (int y = 0; y < in.height; ++y) {
            for (int x = 0; x < in.width; ++x) {
                UInt8 in_pixel = in_data[(in.rowBytes * y) + x];
                f(in_pixel, x, y);
            }
        }
    }
    
    template<typename functorT>
    void transform(const vImage_Buffer &in, vImage_Buffer &out, functorT f) {
        UInt8 *out_data = (UInt8*)out.data;
        UInt8 *in_data = (UInt8*)in.data;
        for (int y = 0; y < in.height; ++y) {
            for (int x = 0; x < in.width; ++x) {
                UInt8 in_pixel = in_data[(in.rowBytes * y) + x];
                out_data[(out.rowBytes * y) + x] = f(in_pixel);
            }
        }
    }
    
    template<typename functorT>
    void transform(const vImage_Buffer &in1, const vImage_Buffer &in2, vImage_Buffer &out, functorT f) {
        UInt8 *out_data = (UInt8*)out.data;
        UInt8 *in1_data = (UInt8*)in1.data;
        UInt8 *in2_data = (UInt8*)in2.data;
        for (int y = 0; y < in1.height; ++y) {
            for (int x = 0; x < in1.width; ++x) {
                UInt8 in1_pixel = in1_data[(in1.rowBytes * y) + x];
                UInt8 in2_pixel = in2_data[(in2.rowBytes * y) + x];
                out_data[(out.rowBytes * y) + x] = f(in1_pixel, in2_pixel);
            }
        }
    }
    
    template<typename functorT>
    void convolute(const vImage_Buffer& in, vImage_Buffer& out,functorT f) {
        const UInt8 *in_data = (UInt8*)in.data;
        UInt8 *out_data = (UInt8*)out.data;
        
        // Get Kernel Width and Height
        const int kernel_width = f.kernel().width();
        const int kernel_height = f.kernel().height();
        const int height = in.height - f.kernel().height() + 1;
        // Work across Rows
        for(int y = 0; y < height; ++y) {
            // Get beginning and end of row
            const UInt8 *pi = in_data + (in.rowBytes * y);
            const UInt8 *end = (pi + in.width) - (kernel_width - 1);
            UInt8 *po = out_data + (out.rowBytes * y);
            // Apply functor
            for(int i = 0; pi != end; ++i, ++pi, ++po) {
                f.reset();
                for(int ky = 0; ky < kernel_height; ++ky) {
                    // Move to appropriate position
                    const UInt8 *itr = in_data + (in.rowBytes * (y + ky)) + i;
                    for(int kx = 0; kx < kernel_width; ++kx) {
                        f.accumulate(itr[kx]);
                    }
                }
                *po = f.scalar_result();
            }
        }
    }
}

//=
#if 0
// For sequence grabber
int32 get_address(const image_id idnt) {
	return (int32)(&(image_manager::instance().image(idnt)));
}
#endif
//= Image Allocation/Deallocation ==============================================
image_id new_image(const int width, const int height, const int channel_count, LStrHandle error) {
	// Skip if there was an error
	if(lv::lstrlen(error))
		return -1;
	
    image_id new_image_id = image_manager::instance().new_image(width, height, channel_count);
    if (new_image_id == -1) {
        lv::to_lstr(error, "Failed to create image. Only 4-channel images currently supported.");
    }
    return new_image_id;
}

void delete_image(const image_id idnt, LStrHandle error) {
	if(lv::lstrlen(error))
		return;
	image_manager::instance().delete_image(idnt);
}

int allocated_count() {
	return image_manager::instance().allocated_count();
}

/*
image_id new_subimage_channel(const image_id original_id, const lv::image_type type, LStrHandle error) {
	// Skip if there was an error
	if(lv::lstrlen(error))
		return -1;
	
	image_manager &manager = image_manager::instance();
	lv::image lvimg = manager.image(original_id);
	if(lvimg.type() != rgba) {
		lv::to_lstr(error, "Only RGBA Images have channels");
		return -1;
	} else {
		return manager.new_subimage_channel(original_id, type);
	}
}
*/

//= Image Conversion ===========================================================
void convert_from_pixmap(const unsigned long *img, const image_id idnt, const channel_id cid, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(idnt, cid, error);
	
	lv::image &lvimg = image_manager::instance().image(idnt);

	// Convert vimg to picture
	const int h = lvimg.height();
	const int w = lvimg.width();
	for(int y = 0; y < h; ++y) {
		const int offset = y * w;
		for(int x= 0; x < w; ++x) {
			UInt8 v[3];
			v[0] = (img[offset + x] & 0xff0000) >> 16;
			v[1] = (img[offset + x] & 0xff00) >> 8;
			v[2] = (img[offset + x] & 0xff);
			// Convert to pixel
			switch(lvimg.channel_count()) {
				case 1:
					lvimg.pixel(0, x, y) = (v[0] + v[1] + v[1] + v[2]) / 4;
					break;
				case 4:
					if(cid >= 0)
						lvimg.pixel(0, x, y) = v[cid];
					else {
						lvimg.pixel(0, x, y) = v[0]; // R
						lvimg.pixel(1, x, y) = v[1]; // G
						lvimg.pixel(2, x, y) = v[2]; // B
						lvimg.pixel(3, x, y) = 0xff; // A
					}
					break;
			}
		}
	}
}

void convert_to_pixmap(const image_id idnt, const channel_id cid, unsigned long *img, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(idnt, cid, error);
	
	lv::image &lvimg = image_manager::instance().image(idnt);

	// Convert vimg to picture
	const int h = lvimg.height();
	const int w = lvimg.width();
	for(int y = 0; y < h; ++y) {
		const int offset = y * w;	// LabVIEW image
		for(int x= 0; x < w; ++x) {
			// Convert to pixel
			switch(lvimg.channel_count()) {
				case 1: {
					unsigned char gray = lvimg.pixel(0, x, y);
					img[offset + x] = (gray << 16) + 
						(gray << 8) +
						(gray);
				}	break;
				case 4:
					if(cid >= 0) {
						img[offset + x] = lvimg.pixel(cid, x, y) << (16 - cid*8);
					} else {
						img[offset + x] = (lvimg.pixel(0, x, y) << 16) +
							(lvimg.pixel(1, x, y) << 8) +
							(lvimg.pixel(2, x, y));
					}
					break;
			}
		}
	}
}

//= Image Initialisation =======================================================
void fill(const image_id o, const channel_id och, const UInt8 c, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(o, och, error);
	
	// Extract image
	image_manager &manager = image_manager::instance(); \
	lv::image &out_image = manager.image(o);
	
	// Perform the operation
	if(och == -1) {
		for(int i = 0; i < out_image.channel_count(); ++i) {
			vImage_Buffer &ch = out_image[i];
			labvvis::transform(ch, ch, labvvis::constant<UInt8>(c));
		}
	} else {
		vImage_Buffer &ch = out_image[och];
		labvvis::transform(ch, ch, labvvis::constant<UInt8>(c));
	}
}

void fill_rect(const image_id o, const channel_id cid, const UInt8 c,
			   const int ix, const int iy, const int width, const int height,
			   LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(o, cid, error);

	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &oimg = manager.image(o);
	
	// Perform the operation
	int ex = ix + width;
	int ey = iy + height;
	if(ex > oimg.width())
		ex = oimg.width();
	if(ey > oimg.height())
		ey = oimg.height();
	if(cid >= 0) {
		for(int y = iy; y < ey; ++y) {
			for(int x = ix; x < ex; ++x) {
				oimg.pixel(cid, x, y) = c;
			}
		}
	} else {
		for(int i = 0; i < oimg.channel_count(); ++i) {
			for(int y = iy; y < ey; ++y) {
				for(int x = ix; x < ex; ++x) {
					oimg.pixel(cid, x, y) = c;
				}
			}
		}
	}
}
/*
void copy(const image_id in, const channel_id icid, const image_id out, const channel_id ocid, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(in, icid, error);
	CHECK_CHANNEL(out, ocid, error);
	CHECK_CHANNEL_COUNT(in, icid, out, ocid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &iimg = manager.image(in);
	lv::image &oimg = manager.image(out);
	
	if(icid >= 0 && ocid >= 0)
		oimg[ocid] = iimg[icid];
	else {
		for(int i = 0; i < iimg.channel_count(); ++i) {
			oimg[ocid] = iimg[icid];
		}
	}
}
*/

//= Image Information ==========================================================
void image_size(const image_id o, int *width, int *height, LStrHandle error) {
	PRE_FLIGHT(error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	if(manager.image_exists(o)) {
		lv::image &img = manager.image(o);
		*width = img.width();
		*height = img.height();
	} else {
		lv::to_lstr(error, "Non-existant image referenced");
	}
}


void get_pixel(const image_id iid, const channel_id cid, const int x, const int y, UInt8 *value, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(iid, cid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &img = manager.image(iid);
	int real_cid = cid;
	if(cid < 0) {
		if(img.channel_count() > 1) {
			lv::to_lstr(error, "An images with a single channel was expected");
			return;
		} else {
			real_cid = 0;
		}
	}
	
	*value = img.pixel(real_cid, x, y);
}

void get_pixel_rgba(const image_id iid, const channel_id cid, const int x, const int y,
					UInt8 *r, UInt8 *g, UInt8 *b, UInt8 *a, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(iid, cid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &img = manager.image(iid);
	if(cid > 0 || img.channel_count() != 4) {
		lv::to_lstr(error, "A RGBA image was expected");
		return;
	} else {
		*r = img.pixel(0, x, y);
		*g = img.pixel(1, x, y);
		*b = img.pixel(2, x, y);
		*a = img.pixel(3, x, y);
	}
}

void set_pixel(const image_id iid, const channel_id cid, const int x, const int y, const UInt8 value, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(iid, cid, error);

	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &img = manager.image(iid);
	int real_cid = cid;
	if(cid < 0) {
		if(img.channel_count() > 1) {
			lv::to_lstr(error, "An images with a single channel was expected");
			return;
		} else {
			real_cid = 0;
		}
	}
	
	img.pixel(real_cid, x, y) = value;
}

void set_pixel_rgba(const image_id iid, const channel_id cid, const int x, const int y,
					const UInt8 r, const UInt8 g, const UInt8 b, const UInt8 a,
					LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(iid, cid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &img = manager.image(iid);
	if(cid > 0 || img.channel_count() != 4) {
		lv::to_lstr(error, "A RGBA image was expected");
		return;
	} else {
		img.pixel(0, x, y) = r;
		img.pixel(1, x, y) = g;
		img.pixel(2, x, y) = b;
		img.pixel(3, x, y) = a;
	}
}

void count_pixels(const image_id iid, const channel_id cid, const UInt8 pixel, unsigned int *count, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(iid, cid, error);
	
	// Extract image
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &img = manager.image(iid);
	int real_cid = cid;
	if(cid < 0) {
		if(img.channel_count() > 1) {
			lv::to_lstr(error, "An images with a single channel was expected");
			return;
		} else {
			real_cid = 0;
		}
	}
	
	labvvis::count_if<UInt8, labvvis::equal_to<UInt8> > c(labvvis::equal_to<UInt8>(), pixel);
	labvvis::for_each(img[real_cid], c);
	*count = c.count();
}

//= Image Operations ===========================================================
void unary_operation(const unary_functions f, const image_id i1, const channel_id c1,
					 const image_id o, const channel_id co, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, c1, error);
	CHECK_CHANNEL(o, co, error);
	
#define PERFORM_UNARY_OP(F) { \
	lv::image &img1 = manager.image(i1); \
	lv::image &imgo = manager.image(o);	\
	if(c1 >= 0 && co >= 0) { \
		labvvis::transform(img1[c1], imgo[co], labvvis::F<>()); \
	} else { \
		for(int i = 0; i < img1.channel_count(); ++i) { \
            labvvis::transform(img1[i], imgo[i], labvvis::F<>()); \
		} \
	} \
}
	image_manager &manager = image_manager::instance();
	switch(f) {
		case bitwise_not: PERFORM_UNARY_OP(bitwise_not); break;
		case copy: PERFORM_UNARY_OP(noop); break;
	}
#undef PERFORM_UNARY_OP
}

void binary_operation(const binary_functions f, const image_id i1, const channel_id c1,
					  const image_id i2, const channel_id c2,
					  const image_id o, const channel_id co, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, c1, error);
	CHECK_CHANNEL(i2, c2, error);
	CHECK_CHANNEL(o, co, error);

#define PERFORM_BINARY_OP(F) { \
	lv::image &img1 = manager.image(i1); \
	lv::image &img2 = manager.image(i2); \
	lv::image &imgo = manager.image(o);	\
	if(c1 >= 0 && c2 >= 0 && co >= 0) { \
        labvvis::transform(img1[c1], img2[c2], imgo[co], labvvis::F<>()); \
	} else { \
		for(int i = 0; i < img1.channel_count(); ++i) { \
            labvvis::transform(img1[i], img2[i], imgo[i], labvvis::F<>()); \
		} \
	} \
}
	image_manager &manager = image_manager::instance();
	switch(f) {
		case plus_saturated: PERFORM_BINARY_OP(plus_saturated); break;
		case minus_saturated: PERFORM_BINARY_OP(minus_saturated); break;
		case multiplies: PERFORM_BINARY_OP(multiplies); break;
		case divides: PERFORM_BINARY_OP(divides); break;
		case modulus: PERFORM_BINARY_OP(modulus); break;
		case equal_to: PERFORM_BINARY_OP(equal_to); break;
		case not_equal_to: PERFORM_BINARY_OP(not_equal_to); break;
		case greater: PERFORM_BINARY_OP(greater); break;
		case greater_equal: PERFORM_BINARY_OP(greater_equal); break;
		case less: PERFORM_BINARY_OP(less); break;
		case less_equal: PERFORM_BINARY_OP(less_equal); break;
		case logical_and: PERFORM_BINARY_OP(logical_and); break;
		case logical_or: PERFORM_BINARY_OP(logical_or); break;
		case bitwise_and: PERFORM_BINARY_OP(bitwise_and); break;
		case bitwise_or: PERFORM_BINARY_OP(bitwise_or); break;
	}
#undef PERFORM_BINARY_OP
}

void equalize(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid,
			  const UInt8 min, const UInt8 max, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	CHECK_CHANNEL(o, ocid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	lv::image &out_image = manager.image(o);
	
	// Perform operation
	if(icid >= 0 && ocid >= 0) {
        labvvis::transform(src_image[icid], out_image[ocid], labvvis::equalize<UInt8>(min, max));
	} else {
		for(int i = 0; i < src_image.channel_count(); ++i) {
            labvvis::transform(src_image[i], out_image[i], labvvis::equalize<UInt8>(min, max));
		}
	}
}

void threshold(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid, 
			   const int value, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	CHECK_CHANNEL(o, ocid, error);

	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	lv::image &out_image = manager.image(o);

	// Perform operation
	if(icid >= 0 && ocid >= 0) {
		labvvis::transform(src_image[icid], out_image[ocid],
						std::bind2nd(labvvis::greater<UInt8>(), value));
	} else {
		for(int i = 0; i < src_image.channel_count(); ++i) {
            labvvis::transform(src_image[i], out_image[i],
						std::bind2nd(labvvis::greater<UInt8>(), value));
		}
	}
}

void linear_filter(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid,
				SInt8 mask[], const int kw, const int kh, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	CHECK_CHANNEL(o, ocid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	lv::image &out_image = manager.image(o);
	
	// Perform operation
	if(icid >= 0 && ocid >= 0) {
		labvvis::kernel2d<SInt8> kernel(mask, kw, kh);
		labvvis::convolute(src_image[icid], out_image[ocid],
                           labvvis::linear_filter<SInt8, UInt8>(kernel));
	} else {
		for(int i = 0; i < src_image.channel_count(); ++i) {
            labvvis::kernel2d<SInt8> kernel(mask, kw, kh);
            labvvis::convolute(src_image[i], out_image[i],
                               labvvis::linear_filter<SInt8, UInt8>(kernel));
		}
	}
}

void max_filter(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid,
				SInt8 mask[], const int kw, const int kh, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	CHECK_CHANNEL(o, ocid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	lv::image &out_image = manager.image(o);
	
	// Perform operation
	if(icid >= 0 && ocid >= 0) {
        labvvis::kernel2d<SInt8> kernel(mask, kw, kh);
        labvvis::convolute(src_image[icid], out_image[ocid],
                           labvvis::max_filter<SInt8, UInt8>(kernel));
	} else {
		for(int i = 0; i < src_image.channel_count(); ++i) {
			labvvis::kernel2d<SInt8> kernel(mask, kw, kh);
            labvvis::convolute(src_image[i], out_image[i],
                               labvvis::max_filter<SInt8, UInt8>(kernel));
		}
	}
}

namespace lv {
	struct histogram {
		histogram(int *counts)
			: _counts(counts) {
				for(int i = 0; i < 256; ++i) {
					_counts[i] = 0;
				}
			}
		void operator()(const UInt8 p) {
			++_counts[p];
		}
	private:
        int *_counts;
	};
} // End of lv namespace
/*
namespace vvis {
	template<> struct is_vectorised_functor<lv::histogram> {
		static const bool value = false;
	};
} // End of vvis namespace
*/
void histogram(const image_id i1, const channel_id icid, int counts[], const int counts_size, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	
	// Check icid
	if(icid < 0) {
		lv::to_lstr(error, "Invalid channel requested. Histogram operates only on single-channel images");
		return;
	}
	
	// Check count size
	if(counts_size != 256) {
		lv::to_lstr(error, "Array of size 256 is expected");
		return;
	}
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	
	// Perform operation
	lv::histogram h(counts);
	labvvis::for_each(src_image[icid], h);
}


#if 0
namespace priv {
	UInt8 pixel(const one_channel &a, const int x, const int y) {
		if(x < 0 || y < 0 || x >= a.width() || y >= a.height()) {
			return 0;
		} else {
			return a.pixel(x, y) > 0 ? 1 : 0;
		}
	}
	// 4 2
	// 8 0 <- x,y
	int window_state(const one_channel &a, const int x, const int y) {
		return pixel(a, x, y) +
			2*pixel(a, x, y-1) +
			4*pixel(a, x-1, y-1) +
			8*pixel(a, x-1, y);
	}
}

std::map<UInt8, vvis::rect>
_grow_blob(const one_channel &a, one_channel &o, const int blob_start, const int blob_step) {
	std::map<UInt8, vvis::rect> blobs;	// Blob number : First row
	int current_blob = blob_start - blob_step;
	// Fill out with 0
	vvis::transform(o, pixel_accessor_1ch(), o, pixel_accessor_1ch(), vvis::constant<UInt8>(0));
	for(int y = 0; y < a.height(); ++y) {
		for(int x = 0; x < a.width(); ++x) {
			int window_state = priv::window_state(a, x, y);
			switch(window_state) {
				case 0:
				case 2:
				case 6:
				case 8:
				case 10:
				case 12:
					// 1 Connect to background blob
					break;
				case 1:
					// 2 Start new object blob
					current_blob += blob_step;
					o.pixel(x, y) = current_blob;
					blobs[current_blob] = vvis::rect(x, y, 1, 1);
					break;
				case 3:
				case 7:
				case 15: {
					// 3 Connect to object blob (all cases assumed to have previous on top)
					o.pixel(x, y) = o.pixel(x, y-1);
					// Adjust rect
					vvis::rect &r = blobs[o.pixel(x, y)];
					if(x < r.left)
						r.left = x;
					int w = x - r.left + 1;
					if(w > r.width)
						r.width = w;
				}	break;
				case 9:
				case 13: {
					// 3 Connect to object blob (all cases assumed to have previous in front)
					o.pixel(x, y) = o.pixel(x-1, y);
					// Adjust rect
					vvis::rect &r = blobs[o.pixel(x, y)];
					if(x < r.left)
						r.left = x;
					int w = x - r.left + 1;
					if(w > r.width)
						r.width = w;
				}	break;					
				case 4: {
					// 4 End of object blob, connect to background blob and merge background blobs
					vvis::rect &r = blobs[o.pixel(x-1, y-1)];
					r.height = y - r.top + 1;
				}	break;
				case 5: {
					// 5 Start new object blob and end old object blob
					vvis::rect &r = blobs[o.pixel(x-1, y-1)];
					r.height = y - r.top + 1;
					
					current_blob += blob_step;
					o.pixel(x, y) = current_blob;
					blobs[current_blob] = vvis::rect(x, y, 1, 1);
				}	break;
				case 11: {
					// 6 End of background blob, connect to object blob and merge object blobs
					UInt8 this_blob = o.pixel(x-1, y);
					// y must be >0 since there is a 1 above
					UInt8 to_replace = o.pixel(x, y-1);
					vvis::rect &old_r = blobs[to_replace];
					//blobs[this_blob] = old_r;
					for(int y_ = old_r.top; y_ <= y; ++y_) {
						for(int x_ = old_r.left; x_ < (old_r.left + old_r.width); ++x_) {
							if(o.pixel(x_, y_) == to_replace) {
								o.pixel(x_, y_) = this_blob;
							}
						}
					}
					blobs.erase(to_replace);
					o.pixel(x, y) = this_blob;
					// Adjust rect
					vvis::rect &r = blobs[this_blob];
					if(x < r.left)
						r.left = x;
					int w = x - r.left + 1;
					if(w > r.width)
						r.width = w;
				}	break;
				case 14:
					// 7 Start new background blob
					break;
			}
		}
	}
	return blobs;
}

void grow_blob(const image_id i1, const channel_id icid,
			   const image_id o, const channel_id ocid,
			   const int blob_start, const int blob_step, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	CHECK_CHANNEL(o, ocid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	lv::image &out_image = manager.image(o);
	
	// Perform operation
	std::map<UInt8, vvis::rect> blobs =
		_grow_blob(src_image[icid], out_image[ocid], blob_start, blob_step);
}
#endif

//==============================================================================
// Hough's Transform
//------------------------------------------------------------------------------
void hough_transform(const image_id i1, const channel_id icid, hough_id_t *hough_id, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(i1, icid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &src_image = manager.image(i1);
	
	// Get a hough object
	std::cerr << "Hough_id in: " << *hough_id << std::endl;
	if(*hough_id == 0) {
		*hough_id = houghs.new_object(hough_t());
	}
	std::cerr << "Hough_id out: " << *hough_id << std::endl;
	houghs_object_manager::iterator i = houghs.find(*hough_id);
	if(i == houghs.end()) {
		// No object gound
		lv::to_lstr(error, "Non-existant Hough's Transform result object referenced");
		return;
	}
	
	// Perform operation
	// Create a hough tranform object for image img
	labvvis::for_each_xy(src_image[icid], i->second);
}

void get_hough_lines(const hough_id_t hough_id, const unsigned short threshold, hough_lines_handle hlines_hdl, 
unsigned short *lines_count, LStrHandle error) {
	std::cerr << "Hough_id in: " << hough_id << std::endl;
	houghs_object_manager::iterator h = houghs.find(hough_id);
	if(h == houghs.end()) {
		// No object gound
		lv::to_lstr(error, "Non-existant Hough's Transform result object referenced");
		return;
	}
	
	hough_lines &hl = **((hough_lines_handle)hlines_hdl);
	std::vector<hough_t::value_type> lines = h->second.result(threshold);
	*lines_count = lines.size();
	int j = 0;
	for(std::vector<hough_t::value_type>::const_iterator i = lines.begin();
	i != lines.end(); ++i, ++j) {
		if(j >= hl.dimSize) {
			break;
		}
		hl.ALineFromHoughSTransform[j].r = i->r;
		hl.ALineFromHoughSTransform[j].q = i->q;
		hl.ALineFromHoughSTransform[j].count = i->count;
	}
}

void delete_hough(int hough_id, LStrHandle error) {
	houghs_object_manager::iterator i = houghs.find(hough_id);
	if(i != houghs.end()) {
		houghs.erase(i);
	}
}

void start_sequence_grabber(const int width, const int height, LStrHandle error) {
	PRE_FLIGHT(error);
    SequenceGrabber *grabber = [SequenceGrabber sharedInstance];
    NSError *e = nil;
    [grabber startCaptureAtSize:NSMakeSize(width, height) error:&e];
    if (e != nil) {
        lv::to_lstr(error, "Failed to start capture");
    }
}

void grab_sequence_grabber(const image_id o, const channel_id ocid, LStrHandle error) {
	PRE_FLIGHT(error);
	CHECK_CHANNEL(o, ocid, error);
	
	// Extract image
	image_manager &manager = image_manager::instance();
	lv::image &image = manager.image(o);
    
    SequenceGrabber *grabber = [SequenceGrabber sharedInstance];
    NSError *e = nil;
    [grabber waitUntilFrameCapturedTo:&image error:&e];
}

void stop_sequence_grabber(LStrHandle error) {
    SequenceGrabber *grabber = [SequenceGrabber sharedInstance];
    [grabber stopCapture];
}

