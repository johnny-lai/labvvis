/*
 *  conversion.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sun Aug 24 2003.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 */

#ifndef _VVIS_CONVERSION_H
#define _VVIS_CONVERSION_H

#include <ct/enabler.h>
#include <vvis/macros.h>
#include <vvis/types.h>
#include <vvis/quicktime.h>

namespace vvis {
	namespace priv {
		template<int pixelFormat, typename imageT>
		struct gworld_converter {
			// Can convert from any gworld to any 1-channel image
			template<typename T, template<typename> class storageP>
			static void to_image(image<T, storageP>& image, GWorldPtr gworld, const int width, const int height) {
				_VVIS_WARN("Converting GWorld to image using GetCPixel");
				const T t_max = std::numeric_limits<T>::max();
				const float us_max = static_cast<float>(std::numeric_limits<unsigned short>::max());
				CGrafPtr orig_port;
				GDHandle orig_device;
				GetGWorld(&orig_port, &orig_device);
				SetGWorld(gworld, NULL);
				for(int y = 0; y < height; ++y) {
					for(int x = 0; x < width; ++x) {
						RGBColor color;
						GetCPixel(x, y, &color);
						image.pixel(x, y) = static_cast<T>((color.red / us_max) * t_max);
						// TODO: convert properly
					}
				}
				SetGWorld(orig_port, orig_device);
			}
			// Can convert from any 1-channel image to any gworld
			template<typename T, template<typename> class storageP>
			static void to_gworld(const GWorldPtr gworld, const image<T, storageP>& image, const int width, const int height) {
				_VVIS_WARN("Converting image to GWorld using SetCPixel");
				const float t_max = static_cast<float>(std::numeric_limits<T>::max());
				const float us_max = static_cast<float>(std::numeric_limits<unsigned short>::max());
				CGrafPtr orig_port;
				GDHandle orig_device;
				GetGWorld(&orig_port, &orig_device);
				SetGWorld(gworld, NULL);
				for(int y = 0; y < height; ++y) {
					for(int x = 0; x < width; ++x) {
						RGBColor color;
						color.red = static_cast<unsigned short>((image.pixel(x, y) / t_max) * us_max);
						color.green = static_cast<unsigned short>((image.pixel(x, y) / t_max) * us_max);
						color.blue = static_cast<unsigned short>((image.pixel(x, y) / t_max) * us_max);
						SetCPixel(x, y, &color);
					}
				}
				SetGWorld(orig_port, orig_device);
			}
			// Can convert from any gworld to any rgb image
			template<typename T, template<typename> class storageP>
			static void to_image(rgb_image<T, storageP>& image, GWorldPtr gworld, const int width, const int height) {
				_VVIS_WARN("Converting GWorld to rgb_image using GetCPixel");
				const T t_max = std::numeric_limits<T>::max();
				//const unsigned short us_max = static_castMstd::numeric_limits<unsigned short>::max();
				const float us_max = static_cast<float>(std::numeric_limits<unsigned short>::max());
				CGrafPtr orig_port;
				GDHandle orig_device;
				GetGWorld(&orig_port, &orig_device);
				SetGWorld(gworld, NULL);
				for(int y = 0; y < height; ++y) {
					for(int x = 0; x < width; ++x) {
						RGBColor color;
						GetCPixel(x, y, &color);
						image.red(x, y) = static_cast<T>((color.red / us_max) * t_max);
						image.green(x, y) = static_cast<T>((color.green / us_max) * t_max);
						image.blue(x, y) = static_cast<T>((color.blue / us_max) * t_max);
						//cout << color.red << ", " << color.green << ", " << color.blue << " -> "
						//	<< image.red(x, y) << ", " << image.green(x, y) << ", " << image.blue(x, y) << endl;
					}
				}
				SetGWorld(orig_port, orig_device);
			}
			// Can convert from any rgb_image to any gworld
			template<typename T, template<typename> class storageP>
			static void to_gworld(const GWorldPtr gworld, const rgb_image<T, storageP>& image, const int width, const int height) {
				_VVIS_WARN("Converting rgb_image to GWorld using SetCPixel");
				const float t_max = static_cast<float>(std::numeric_limits<T>::max());
				const float us_max = static_cast<float>(std::numeric_limits<unsigned short>::max());
				CGrafPtr orig_port;
				GDHandle orig_device;
				GetGWorld(&orig_port, &orig_device);
				SetGWorld(gworld, NULL);
				for(int y = 0; y < height; ++y) {
					for(int x = 0; x < width; ++x) {
						RGBColor color;
						color.red = static_cast<unsigned short>((image.red(x, y) / t_max) * us_max);
						color.green = static_cast<unsigned short>((image.green(x, y) / t_max) * us_max);
						color.blue = static_cast<unsigned short>((image.blue(x, y) / t_max) * us_max);
						SetCPixel(x, y, &color);
					}
				}
				SetGWorld(orig_port, orig_device);
			}
		};

#if 0
		// Store a vvm::vector to an unaligned location in memory
		inline void store_unaligned(const vvm::vector_uint8& v, unsigned char *p)
		{
			// Load the surrounding areas
			vvm::vector_uint8 low = vvm::load(p, 0);
			vvm::vector_uint8 high = vvm::load(p, 16);
			// Prepare the constants that we need
			vvm::vector_uint8 pv = vvm::lvsl(p, 0);
			vvm::vector_uint8 offset(vvm::uoffset(p));
			// Insert our data into the low and 
			// high vectors
			low = vvm::select(pv >= offset, low, v);
			high = vvm::select(pv < offset, v, high);
			// Store the two aligned result
			// vectors
			vvm::store(low, p, 0);
			vvm::store(high, p, 16);
		}
		//- gworld_converter<k32ARGBPixelFormat, rgb_image<uint8, storageP> > --
		/** GWorld converter for k32ARGBPixelFormats
		 * Inherits from gworld_converter<0> to get default converters
		 */
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, rgb_image<uint8, storageP> > {
			static void to_image(rgb_image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char *>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				vvm::vector_uint32 argb1, argb2, argb;
				vvm::vector_uint32 c8(8), c16(16);
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					const int offset = vvm::uoffset(addr);
					// Calculate left/right edges
					const int left_edge = (vvm::uoffset(addr) != 0) ? VVM_SCALAR_COUNT : 0;
					const int right_edge = (width - ((width - left_edge) % VVM_SCALAR_COUNT)) + left_edge;
					
					int i = -offset, x = 0; // Use -offset so that addr[i] is an aligned address
					for(; x < left_edge; ++x, i += 4) {
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					argb2 = vvm::load(reinterpret_cast<vvm::uint32*>(&addr[i]));
					for(; x < right_edge; x += VVM_SCALAR_COUNT, i += 4*VVM_SCALAR_COUNT) {
						argb1 = argb2;
						argb2 = vvm::load(reinterpret_cast<vvm::uint32*>(&addr[i]) + VVM_SCALAR_COUNT);
						argb = vvm::load(argb1, argb2, offset);
						vvm::store(vvm::vector_cast<vvm::vector_uint8>(argb), &image.blue(x, y));
						vvm::store(vvm::vector_cast<vvm::vector_uint8>(argb >> c8), &image.green(x, y));
						vvm::store(vvm::vector_cast<vvm::vector_uint8>(argb >> c16), &image.red(x, y));
					}
					for(; x < width; ++x, i += 4) {
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					addr += row_bytes;
				}
			}
			static void to_gworld(const GWorldPtr gworld, const rgb_image<uint8, storageP>& image,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				char* addr = GetPixBaseAddr(pixmap);
				long row_bytes = get_row_bytes(pixmap);
				vvm::vector_uint8 rb1, rb2, ag1, ag2;
				vvm::vector_uint8 r, g, b;
				vvm::vector_uint8 a(0xff); // Opaque
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					const int right_edge = width - (width % VVM_SCALAR_COUNT);
					
					int x = 0, i = 0;
					for(; x < right_edge; x += VVM_SCALAR_COUNT, i += 4*VVM_SCALAR_COUNT) {
						r = vvm::load(&image.red(x, y));
						g = vvm::load(&image.green(x, y));
						b = vvm::load(&image.blue(x, y));
						rb1 = vvm::mergeh(r, b);
						rb2 = vvm::mergel(r, b);
						ag1 = vvm::mergeh(a, g);
						ag2 = vvm::mergel(a, g);
						store_unaligned(vvm::mergeh(ag1, rb1), reinterpret_cast<uint8*>(&addr[i]));
						store_unaligned(vvm::mergel(ag1, rb1), reinterpret_cast<uint8*>(&addr[i+VVM_SCALAR_COUNT]));
						store_unaligned(vvm::mergeh(ag2, rb2), reinterpret_cast<uint8*>(&addr[i+2*VVM_SCALAR_COUNT]));
						store_unaligned(vvm::mergel(ag2, rb2), reinterpret_cast<uint8*>(&addr[i+3*VVM_SCALAR_COUNT]));
					}
					for(; x < width; ++x, i += 4) {
						addr[i+1] = image.red(x, y);
						addr[i+2] = image.green(x, y);
						addr[i+3] = image.blue(x, y);
					}
					addr += row_bytes;
				}
			}
		};
#elif 0
		// Store a vector to an unaligned 
		// location in memory
		inline void store_unaligned(__vector unsigned char v, unsigned char *p)
		{
			// Load the surrounding areas
			__vector unsigned char low = vec_ld(0, p);
			__vector unsigned char high = vec_ld(16, p);
			// Prepare the constants that we need
			__vector unsigned char pv = vec_lvsr(0, (int *)p);
			__vector unsigned char oxFF = vec_splat_u8(-1);
			__vector unsigned char ox00 = vec_splat_u8(0);
			// Make a mask for which parts of 
			// vector to swap out
			__vector unsigned char m = vec_perm(ox00, oxFF, pv);
			// Right rotate out input data
			v = vec_perm(v, v, pv);
			// Insert our data into the low and 
			// high vectors
			low = vec_sel(low, v, m);
			high = vec_sel(v, high, m);
			// Store the two aligned result
			// vectors
			vec_st(low, 0, p);
			vec_st(high, 16, p);
		}
		//= AltiVec - image ====================================================
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, image<uint8, storageP> > {
			static void to_image(image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				LockPixels(pixmap);
				unsigned char* addr = reinterpret_cast<unsigned char*>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				__vector unsigned char c16 = (__vector unsigned char)(16);
				__vector unsigned char c8 = (__vector unsigned char)(8);
				__vector unsigned int argb1, argb2, argb3, argb4, argb5;
				__vector unsigned short gb1, gb2, ar1, ar2;
				__vector unsigned char r, g, b;
				__vector unsigned char result;
				// Assuming ARGB
				//const int left_edge = VVM_SCALAR_COUNT;
				const int right_edge = width - VVM_SCALAR_COUNT;
				for(int y = 0; y < height; ++y) {
					const int left_edge = (vvm::uoffset(addr) != 0) ? VVM_SCALAR_COUNT : 0;
					const int right_edge = (width - ((width - left_edge) % VVM_SCALAR_COUNT)) + left_edge;
					
					__vector unsigned char vmask = vec_lvsr(0, addr);
					// Do first 16 with scalar processor to avoid loading head
					int i = 0, x = 0;
					for(; x < left_edge && x < width; ++x, i += 4) {
						// Green is counted twice to be consistent with
						// vector program
						image.pixel(x, y) = (addr[i+1] + addr[i+2] + addr[i+2] + addr[i+3]) / 4;
					}
					// Use -48 to prevent loading past end
					// -32 will leave 16 at end
					argb5 = vec_ld(0, (unsigned int*)&addr[i]);
					for(; x < right_edge; x += 16, i += 4*16) {
						unsigned int* iaddr = (unsigned int*)&addr[i];
						argb1 = argb5;
						argb2 = vec_ld(16, iaddr);
						argb3 = vec_ld(32, iaddr);
						argb4 = vec_ld(48, iaddr);
						argb5 = vec_ld(64, iaddr);
						// Do shift
						argb1 = vec_perm(argb1, argb2, vmask);
						argb2 = vec_perm(argb2, argb3, vmask);
						argb3 = vec_perm(argb3, argb4, vmask);
						argb4 = vec_perm(argb4, argb5, vmask);
						// Dechunkify
						gb1 = vec_pack(argb1, argb2);
						gb2 = vec_pack(argb3, argb4);
						b = vec_pack(gb1, gb2);
						g = vec_pack(vec_sro(gb1, c8), vec_sro(gb2, c8));
						ar1 = vec_pack(vec_sro(argb1, c16), vec_sro(argb2, c16));
						ar2 = vec_pack(vec_sro(argb3, c16), vec_sro(argb4, c16));
						r = vec_pack(ar1, ar2);
						// Gray is calculated by averaging. Use g twice
						result = vec_avg(vec_avg(r, b), g);
						vec_st(result, 0, &image.pixel(x, y));
					}
					// Do last sixteen
					for(; x < width; ++x, i += 4) {
						image.pixel(x, y) = (addr[i+1] + addr[i+2] + addr[i+2] + addr[i+3]) / 4;
					}
					addr += row_bytes;
				}
				UnlockPixels(pixmap);
			}
			static void to_gworld(const GWorldPtr gworld, const image<uint8, storageP>& image,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char*>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				__vector unsigned char rb1, rb2, ag1, ag2, tmp;
				__vector unsigned char g;   // Only gray
				__vector unsigned char a = (__vector unsigned char)(0xff); // Opaque
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					const int right_edge = width - (width % 16);
					
					int i = 0, x = 0;
					for(; x < right_edge; x += 16, i += 4*16) {
						g = vec_ld(0, &image.pixel(x, y));
						rb1 = vec_mergeh(g, g);
						rb2 = vec_mergel(g, g);
						ag1 = vec_mergeh(a, g);
						ag2 = vec_mergel(a, g);
						// For unknown reasons, GCC 4.0 does not accept
						//   store_unaligned(vec_mergeh(ag1, rb1) ...)
						store_unaligned((tmp = vec_mergeh(ag1, rb1)), reinterpret_cast<unsigned char*>(&addr[i]));
						store_unaligned((tmp = vec_mergel(ag1, rb1)), reinterpret_cast<unsigned char*>(&addr[i+16]));
						store_unaligned((tmp = vec_mergeh(ag2, rb2)), reinterpret_cast<unsigned char*>(&addr[i+32]));
						store_unaligned((tmp = vec_mergel(ag2, rb2)), reinterpret_cast<unsigned char*>(&addr[i+48]));
					}
					for(; x < width; ++x, i += 4) {
						addr[i] = 0xff;
						addr[i+1] = addr[i+2] = addr[i+3] = image.pixel(x,y);
					}
					addr += row_bytes;
				}
			}
		};
		//= AltiVec - rgb_image ================================================
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, rgb_image<uint8, storageP> > {
			static void to_image(rgb_image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char*>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				__vector unsigned char c16 = (__vector unsigned char)(16);
				__vector unsigned char c8 = (__vector unsigned char)(8);
				__vector unsigned int argb1, argb2, argb3, argb4, argb5;
				__vector unsigned short gb1, gb2, ar1, ar2;
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					// Calculate left/right edges
					const int left_edge = (vvm::uoffset(addr) != 0) ? VVM_SCALAR_COUNT : 0;
					const int right_edge = (width - ((width - left_edge) % VVM_SCALAR_COUNT)) + left_edge;
					
					int i = 0, x = 0;
					for(; x < left_edge; ++x, i += 4) {
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					__vector unsigned char vmask = vec_lvsl(0, addr);
					argb5 = vec_ld(0, (unsigned int*)&addr[i]);
					for(; x < right_edge; x += 16, i += 4*16) {
						unsigned int* iaddr = (unsigned int*)&addr[i];
						argb1 = argb5;
						argb2 = vec_ld(16, iaddr);
						argb3 = vec_ld(32, iaddr);
						argb4 = vec_ld(48, iaddr);
						argb5 = vec_ld(64, iaddr);
						// Do shift
						argb1 = vec_perm(argb1, argb2, vmask);
						argb2 = vec_perm(argb2, argb3, vmask);
						argb3 = vec_perm(argb3, argb4, vmask);
						argb4 = vec_perm(argb4, argb5, vmask);
						// Chunkify
						gb1 = vec_pack(argb1, argb2);
						gb2 = vec_pack(argb3, argb4);
						vec_st(vec_pack(gb1, gb2), 0, &image.blue(x, y));
						vec_st(vec_pack(vec_sro(gb1, c8), vec_sro(gb2, c8)), 0, &image.green(x, y));
						ar1 = vec_pack(vec_sro(argb1, c16), vec_sro(argb2, c16));
						ar2 = vec_pack(vec_sro(argb3, c16), vec_sro(argb4, c16));
						vec_st(vec_pack(ar1, ar2), 0, &image.red(x, y));
					}
					for(; x < width; ++x, i += 4) {
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					addr += row_bytes;
				}
			}
			static void to_gworld(const GWorldPtr gworld, const rgb_image<uint8, storageP>& image,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char *>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				__vector unsigned char rb1, rb2, ag1, ag2, tmp;
				__vector unsigned char r, g, b;
				__vector unsigned char a = (__vector unsigned char)(0xff); // Opaque
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					// Calculate left/right edges
					const int right_edge = width - (width % 16);
					
					int i = 0, x = 0;
					for(; x < right_edge; x += 16, i += 4*16) {
						r = vec_ld(0, &image.red(x, y));
						g = vec_ld(0, &image.green(x, y));
						b = vec_ld(0, &image.blue(x, y));
						rb1 = vec_mergeh(r, b);
						rb2 = vec_mergel(r, b);
						ag1 = vec_mergeh(a, g);
						ag2 = vec_mergel(a, g);
						store_unaligned((tmp = vec_mergeh(ag1, rb1)), reinterpret_cast<unsigned char*>(&addr[i]));
						store_unaligned((tmp = vec_mergel(ag1, rb1)), reinterpret_cast<unsigned char*>(&addr[i+16]));
						store_unaligned((tmp = vec_mergeh(ag2, rb2)), reinterpret_cast<unsigned char*>(&addr[i+32]));
						store_unaligned((tmp = vec_mergel(ag2, rb2)), reinterpret_cast<unsigned char*>(&addr[i+48]));
					}
					for(; x < width; ++x, i += 4) {
						addr[i] = 0xff;
						addr[i+1] = image.red(x, y);
						addr[i+2] = image.green(x, y);
						addr[i+3] = image.blue(x, y);
					}
					addr += row_bytes;
				}
			}
		};
		//= AltiVec - rgba_image ===============================================
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, rgba_image<uint8, storageP> > {
			static void to_image(rgba_image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char *>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				__vector unsigned char c16 = (__vector unsigned char)(16);
				__vector unsigned char c8 = (__vector unsigned char)(8);
				__vector unsigned int argb1, argb2, argb3, argb4, argb5;
				__vector unsigned short gb1, gb2, ar1, ar2;
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					// Calculate left/right edges
					const int left_edge = (vvm::uoffset(addr) != 0) ? VVM_SCALAR_COUNT : 0;
					const int right_edge = (width - ((width - left_edge) % VVM_SCALAR_COUNT)) + left_edge;
					
					int i = 0, x = 0;
					for(; x < left_edge; ++x, i += 4) {
						image.alpha(x, y) = addr[i];
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					__vector unsigned char vmask = vec_lvsl(0, addr);
					argb5 = vec_ld(0, (unsigned int*)&addr[i]);
					for(; x < right_edge; x += 16, i += 4*16) {
						unsigned int* iaddr = (unsigned int*)&addr[i];
						argb1 = argb5;
						argb2 = vec_ld(16, iaddr);
						argb3 = vec_ld(32, iaddr);
						argb4 = vec_ld(48, iaddr);
						argb5 = vec_ld(64, iaddr);
						// Do shift
						argb1 = vec_perm(argb1, argb2, vmask);
						argb2 = vec_perm(argb2, argb3, vmask);
						argb3 = vec_perm(argb3, argb4, vmask);
						argb4 = vec_perm(argb4, argb5, vmask);
						// Chunkify
						gb1 = vec_pack(argb1, argb2);
						gb2 = vec_pack(argb3, argb4);
						vec_st(vec_pack(gb1, gb2), 0, &image.blue(x, y));
						vec_st(vec_pack(vec_sro(gb1, c8), vec_sro(gb2, c8)), 0, &image.green(x, y));
						ar1 = vec_pack(vec_sro(argb1, c16), vec_sro(argb2, c16));
						ar2 = vec_pack(vec_sro(argb3, c16), vec_sro(argb4, c16));
						vec_st(vec_pack(ar1, ar2), 0, &image.red(x, y));
						vec_st(vec_pack(vec_sro(ar1, c8), vec_sro(ar2, c8)), 0, &image.alpha(x, y));
					}
					for(; x < width; ++x, i += 4) {
						image.alpha(x, y) = addr[i];
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					addr += row_bytes;
				}
			}
			static void to_gworld(const GWorldPtr gworld, const rgba_image<uint8, storageP>& image,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char *>(GetPixBaseAddr(pixmap));
				long row_bytes = GetPixRowBytes(pixmap);
				__vector unsigned char rb1, rb2, ag1, ag2, tmp;
				__vector unsigned char r, g, b, a;
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					// Calculate left/right edges
					const int right_edge = width - (width % 16);
					
					int i = 0, x = 0;
					for(; x < right_edge; x += 16, i += 4*16) {
						a = vec_ld(0, &image.alpha(x, y));
						r = vec_ld(0, &image.red(x, y));
						g = vec_ld(0, &image.green(x, y));
						b = vec_ld(0, &image.blue(x, y));
						rb1 = vec_mergeh(r, b);
						rb2 = vec_mergel(r, b);
						ag1 = vec_mergeh(a, g);
						ag2 = vec_mergel(a, g);
						store_unaligned((tmp = vec_mergeh(ag1, rb1)), reinterpret_cast<unsigned char*>(&addr[i]));
						store_unaligned((tmp = vec_mergel(ag1, rb1)), reinterpret_cast<unsigned char*>(&addr[i+16]));
						store_unaligned((tmp = vec_mergeh(ag2, rb2)), reinterpret_cast<unsigned char*>(&addr[i+32]));
						store_unaligned((tmp = vec_mergel(ag2, rb2)), reinterpret_cast<unsigned char*>(&addr[i+48]));
					}
					for(; x < width; ++x, i += 4) {
						addr[i] = image.alpha(x, y);
						addr[i+1] = image.red(x, y);
						addr[i+2] = image.green(x, y);
						addr[i+3] = image.blue(x, y);
					}
					addr += row_bytes;
				}
			}
		};
#else
		// GetPixBaseAddr conversion
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, image<uint8, storageP> > {
			// Can convert from k32ARGBPixelFormat to any image with components having 1 byte
			static void to_image(image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				uint8* addr = reinterpret_cast<uint8*>(GetPixBaseAddr(pixmap));
				long row_bytes = get_row_bytes(pixmap);
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					for(int x = 0, i = 0; x < width; ++x, i += 4) {
						image.pixel(x, y) = (addr[i+1] + addr[i+2] + addr[i+2] + addr[i+3]) / 4;
					}
					addr += row_bytes;
				}
			}
			// Can convert from image<uint8, ?> to k32ARGBPixelFormat
			static void to_gworld(const GWorldPtr gworld, const image<uint8, storageP>& image,
			const int width, const int height) {
				CGrafPtr orig_port;
				GDHandle orig_device;
				GetGWorld(&orig_port, &orig_device);
				SetGWorld(gworld, NULL);

				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char*>(GetPixBaseAddr(pixmap));
				long row_bytes = get_row_bytes(pixmap);
				for(int y = 0; y < height; ++y) {
					for(int x = 0, i = 0; x < width; ++x, i += 4) {
						addr[i] = 0xff; // Opaque
						addr[i+1] = image.pixel(x, y);
						addr[i+2] = image.pixel(x, y);
						addr[i+3] = image.pixel(x, y);
					}
					addr += row_bytes;
				}
				SetGWorld(orig_port, orig_device);
			}
		};
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, rgb_image<uint8, storageP> > {
			// Can convert from k32ARGBPixelFormat to any rgb_image with components having 1 byte
			static void to_image(rgb_image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				uint8* addr = reinterpret_cast<uint8*>(GetPixBaseAddr(pixmap));
				long row_bytes = get_row_bytes(pixmap);
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					for(int x = 0, i = 0; x < width; ++x, i += 4) {
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					addr += row_bytes;
				}
			}
			// Can convert from rgb_image<uint8, ?> to k32ARGBPixelFormat
			static void to_gworld(const GWorldPtr gworld, const rgb_image<uint8, storageP>& image,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char*>(GetPixBaseAddr(pixmap));
				long row_bytes = get_row_bytes(pixmap);
				for(int y = 0; y < height; ++y) {
					for(int x = 0, i = 0; x < width; ++x, i += 4) {
						addr[i] = 0xff; // Opaque
						addr[i+1] = image.red(x, y);
						addr[i+2] = image.green(x, y);
						addr[i+3] = image.blue(x, y);
					}
					addr += row_bytes;
				}
			}
		};
		template<
			template<typename> class storageP
		> struct gworld_converter<k32ARGBPixelFormat, rgba_image<uint8, storageP> > {
			// Can convert from k32ARGBPixelFormat to any rgba_image with components having 1 byte
			static void to_image(rgba_image<uint8, storageP>& image, GWorldPtr gworld,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				uint8* addr = reinterpret_cast<uint8*>(GetPixBaseAddr(pixmap));
				long row_bytes = get_row_bytes(pixmap);
				// Assuming ARGB
				for(int y = 0; y < height; ++y) {
					for(int x = 0, i = 0; x < width; ++x, i += 4) {
						image.alpha(x, y) = addr[i];
						image.red(x, y) = addr[i+1];
						image.green(x, y) = addr[i+2];
						image.blue(x, y) = addr[i+3];
					}
					addr += row_bytes;
				}
			}
			// Can convert from rgba_image<uint8, ?> to k32ARGBPixelFormat
			static void to_gworld(const GWorldPtr gworld, const rgba_image<uint8, storageP>& image,
			const int width, const int height) {
				PixMapHandle pixmap = GetGWorldPixMap(gworld);
				unsigned char* addr = reinterpret_cast<unsigned char*>(GetPixBaseAddr(pixmap));
				long row_bytes = get_row_bytes(pixmap);
				for(int y = 0; y < height; ++y) {
					for(int x = 0, i = 0; x < width; ++x, i += 4) {
						addr[i] = image.alpha(x, y);
						addr[i+1] = image.red(x, y);
						addr[i+2] = image.green(x, y);
						addr[i+3] = image.blue(x, y);
					}
					addr += row_bytes;
				}
			}
		};
#endif
	} // End of priv namespace
	
	// Image conversion function
	template<typename imageT> imageT image_cast(GWorldPtr gworld) {
		PixMapHandle pixmap = GetGWorldPixMap(gworld);
		Rect& r = (**pixmap).bounds;
		//GetPixBounds(pixmap, &r);
		const int width = r.right - r.left;
		const int height = r.bottom - r.top;
		imageT ret(width, height);
		// Lock the pixmap
		if(!LockPixels(pixmap)) {
			_VVIS_ERROR("Unable to LockPixels while trying to perform image_cast");
			return ret;
		}
		// Perform conversion
		switch((**pixmap).pixelFormat) {
		case k32ARGBPixelFormat:
			priv::gworld_converter<k32ARGBPixelFormat, imageT>::to_image(ret, gworld, width, height);
			break;
		default:
			priv::gworld_converter<0, imageT>::to_image(ret, gworld, width, height);
		}
		// Unlock the pixmap
		UnlockPixels(pixmap);
		return ret;
	}

	template<typename imageT>
	OSErr draw_image(GWorldPtr gworld, const imageT& img) {
		// Get Pixmap
		PixMapHandle pixmap = GetGWorldPixMap(gworld);
		// Lock the pixmap
		if(!LockPixels(pixmap)) {
			_VVIS_ERROR("Unable to LockPixels while trying to perform draw_image");
			return cNoMemErr; // QuickDraw Error: Unable to allocate memory for structures
		}
		// Perform conversion
		switch((**pixmap).pixelFormat) {
		case k32ARGBPixelFormat:
			priv::gworld_converter<k32ARGBPixelFormat, imageT>::to_gworld(gworld, img, img.width(), img.height());
			break;
		default:
			priv::gworld_converter<0, imageT>::to_gworld(gworld, img, img.width(), img.height());
		}
		// Unlock the pixmap
		UnlockPixels(pixmap);
		return noErr;
	}
	
	template<typename imageT>
	OSErr create_gworld(const imageT& img, GWorldPtr* gworld, int pixel_format = k32ARGBPixelFormat) {
		OSErr err;
		// Create new GWorld
		Rect r;
		SetRect(&r, 0, 0, img.width(), img.height());
		if((err = QTNewGWorld(gworld, pixel_format, &r, 0, NULL, 0)) != noErr) {
			return err;
		}
		// Draw Image
		if((err = draw_image(*gworld, img)) != noErr) {
			return err;
		}
		return noErr; 
	}
} // End of vvis namespace

#endif

