/*
 *  vvis.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Thu Jul 17 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

/** \mainpage Vectorized Vision (VVIS)
 * Vectorized Vision (VVIS) is a generic vectorized machine vision library,
 * created from the thesis, "Vector Processor Software for Machine Vision",
 * by Bing-Chang Lai, Phillip John McKerrow and Jo Abrantes.
 * \n
 * VVIS has a different division of duty from other generic libraries. The
 * concepts and their duties are:
 * \li Shape Shapes refer to the actual shape of a storage. It is important
 *           because some algorithms only applies to specific shapes.
 * \li Storage Storages refer to how pixels are arranged relative to 
 *			   each other. The pixel arrangement is important for 
 *			   algorithms because it helps to decide what method of 
 *			   processing the storage is best. Three storages are 
 *			   specified: contiguous, unknown and illife. Contiguous 
 *			   and unknown storages are one-dimensional while illife 
 *			   storages are n-dimensional. Only contiguous storage 
 *			   will be handled using the vector processor.
 * \li Iterator The iterator represents a position in a storage.
 *              It also knows how to traverse the storage. In
 *              VVIS, different storages have different iterators.
 * \li Image Images are two-dimensional representations of a scene 
 *           composed of pixels. Pixels are often square and have 
 *           values which represent the light intensity at a point. 
 *           They can be referenced individually via an  x  and  y  index. 
 *           The origin of images in VVIS is located in the top-left 
 *           corner of an image.  x  increases in value from the left 
 *           to right, and  y  increases in value from top to bottom. 
 *           All images in VVIS are rectangular.
 * \li Region Regions represent regions of interest in an image and 
 *            are responsible for ensuring data is kept in a 
 *            particular format. To do this, they might have an 
 *            admit/release mechanism or perhaps perform unaligned 
 *            loads and stores. Like VSIPL, on admittance, the 
 *            portion referred to by the region in the image should 
 *            not be used at all, until it has been released. An 
 *            image can have more than one portion admitted to 
 *            different regions at one time. However, the portions 
 *            are not allowed to overlap.
 * \li Accessor The accessor is responsible for retrieving and writing 
 *              data, and performing any necessary type conversion. It 
 *              contains different read/write functions for scalar and 
 *              vectors.
 * \li Algorithm Algorithms are responsible for coordinating how the 
 *               rest of the concepts are used to solve a problem.
 * \li Functor Functors are responsible for actually performing the
 *             operation.
 */

#ifndef _VVIS_VVIS_H
#define _VVIS_VVIS_H

#include <vvis/config.h>
#include <vvis/macros.h>
#include <vvis/types.h>
#include <vvis/metafunction.h>
#include <vvis/image.h>
#include <vvis/external_image.h>
#include <vvis/functor.h>
#include <vvis/region.h>
#include <vvis/accessor.h>
#include <vvis/algorithm.h>
#include <vvis/impex.h>

#endif
