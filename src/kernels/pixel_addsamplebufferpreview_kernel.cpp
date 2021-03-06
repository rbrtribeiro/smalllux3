#include "luxrays/kernels/kernels.h"
std::string luxrays::KernelSource_Pixel_AddSampleBufferPreview = 
"/***************************************************************************\n"
" *   Copyright (C) 1998-2010 by authors (see AUTHORS.txt )                 *\n"
" *                                                                         *\n"
" *   This file is part of LuxRays.                                         *\n"
" *                                                                         *\n"
" *   LuxRays is free software; you can redistribute it and/or modify       *\n"
" *   it under the terms of the GNU General Public License as published by  *\n"
" *   the Free Software Foundation; either version 3 of the License, or     *\n"
" *   (at your option) any later version.                                   *\n"
" *                                                                         *\n"
" *   LuxRays is distributed in the hope that it will be useful,            *\n"
" *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *\n"
" *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *\n"
" *   GNU General Public License for more details.                          *\n"
" *                                                                         *\n"
" *   You should have received a copy of the GNU General Public License     *\n"
" *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *\n"
" *                                                                         *\n"
" *   LuxRays website: http://www.luxrender.net                             *\n"
" ***************************************************************************/\n"
"\n"
"// NOTE: this kernel assume samples do not overlap\n"
"\n"
"int Ceil2Int(const float val) {\n"
"	return (int)ceil(val);\n"
"}\n"
"\n"
"int Floor2Int(const float val) {\n"
"	return (int)floor(val);\n"
"}\n"
"\n"
"void AddSample(__global SamplePixel *sp, const float4 sample) {\n"
"    float4 weight = (float4)(sample.w, sample.w, sample.w, 1.f);\n"
"    __global float4 *p = (__global float4 *)sp;\n"
"    *p += weight * sample;\n"
"}\n"
"\n"
"__kernel __attribute__((reqd_work_group_size(64, 1, 1))) void PixelAddSampleBufferPreview(\n"
"	const unsigned int width,\n"
"	const unsigned int height,\n"
"	__global SamplePixel *sampleFrameBuffer,\n"
"	const unsigned int sampleCount,\n"
"	__global SampleBufferElem *sampleBuff) {\n"
"	const unsigned int index = get_global_id(0);\n"
"	if (index >= sampleCount)\n"
"		return;\n"
"\n"
"	const float splatSize = 2.0f;\n"
"	__global SampleBufferElem *sampleElem = &sampleBuff[index];\n"
"\n"
"	const float dImageX = sampleElem->screenX - 0.5f;\n"
"	const float dImageY = sampleElem->screenY - 0.5f;\n"
"    const float4 sample = (float4)(sampleElem->radiance.r, sampleElem->radiance.g, sampleElem->radiance.b, 0.01f);\n"
"\n"
"	int x0 = Ceil2Int(dImageX - splatSize);\n"
"	int x1 = Floor2Int(dImageX + splatSize);\n"
"	int y0 = Ceil2Int(dImageY - splatSize);\n"
"	int y1 = Floor2Int(dImageY + splatSize);\n"
"\n"
"	x0 = max(x0, 0);\n"
"	x1 = min(x1, (int)width - 1);\n"
"	y0 = max(y0, 0);\n"
"	y1 = min(y1, (int)height - 1);\n"
"\n"
"	for (int y = y0; y <= y1; ++y) {\n"
"        const unsigned int offset = y * width;\n"
"\n"
"		for (int x = x0; x <= x1; ++x)\n"
"            AddSample(&sampleFrameBuffer[offset + x], sample);\n"
"	}\n"
"}\n"
;
