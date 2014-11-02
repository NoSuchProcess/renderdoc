/******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Baldur Karlsson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

layout (binding = 1) uniform sampler1D tex1D;
layout (binding = 2) uniform sampler2D tex2D;
layout (binding = 3) uniform sampler3D tex3D;
layout (binding = 4) uniform samplerCube texCube;
layout (binding = 5) uniform sampler1DArray tex1DArray;
layout (binding = 6) uniform sampler2DArray tex2DArray;
layout (binding = 7) uniform samplerCubeArray texCubeArray;

layout (binding = 9) uniform usampler1D texUInt1D;
layout (binding = 10) uniform usampler2D texUInt2D;
layout (binding = 11) uniform usampler3D texUInt3D;
layout (binding = 13) uniform usampler1DArray texUInt1DArray;
layout (binding = 14) uniform usampler2DArray texUInt2DArray;

layout (binding = 16) uniform isampler1D texSInt1D;
layout (binding = 17) uniform isampler2D texSInt2D;
layout (binding = 18) uniform isampler3D texSInt3D;
layout (binding = 20) uniform isampler1DArray texSInt1DArray;
layout (binding = 21) uniform isampler2DArray texSInt2DArray;

vec3 CalcCubeCoord(vec2 uv, int face)
{
	// Map UVs to [-0.5, 0.5] and rotate
	uv -= vec2(0.5);
	vec3 coord;
	if (face == CUBEMAP_FACE_POS_X)
		coord = vec3(0.5, uv.y, -uv.x);
	else if (face == CUBEMAP_FACE_NEG_X)
		coord = vec3(-0.5, -uv.y, uv.x);
	else if (face == CUBEMAP_FACE_POS_Y)
		coord = vec3(uv.x, 0.5, uv.y);
	else if (face == CUBEMAP_FACE_NEG_Y)
		coord = vec3(uv.x, -0.5, -uv.y);
	else if (face == CUBEMAP_FACE_POS_Z)
		coord = vec3(uv.x, -uv.y, 0.5);
	else // face == CUBEMAP_FACE_NEG_Z
		coord = vec3(-uv.x, -uv.y, -0.5);
	return coord;
}

uvec4 SampleTextureUInt4(vec2 pos, int type, bool flipY, int mipLevel, float slice)
{
	uvec4 col;
	if (type == RESTYPE_TEX1D)
	{
		int size = textureSize(texUInt1D, mipLevel);

		col = texelFetch(texUInt1D, int(pos.x), mipLevel);
	}
	else if (type == RESTYPE_TEX1DARRAY)
	{
		ivec2 size = textureSize(texUInt1DArray, mipLevel);

		col = texelFetch(texUInt1DArray, ivec2(pos.x, slice), mipLevel);
	}
	else if (type == RESTYPE_TEX2D)
	{
		ivec2 size = textureSize(texUInt2D, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		col = texelFetch(texUInt2D, ivec2(pos), mipLevel);
	}
	else if (type == RESTYPE_TEX2DARRAY)
	{
		ivec3 size = textureSize(texUInt2DArray, mipLevel);
		
		if (flipY)
			pos.y = size.y - pos.y;

		col = texelFetch(texUInt2DArray, ivec3(pos, slice), mipLevel);
	}
	else // if (type == RESTYPE_TEX3D)
	{
		ivec3 size = textureSize(texUInt3D, mipLevel);
		
		if (flipY)
			pos.y = size.y - pos.y;

		col = texelFetch(texUInt3D, ivec3(pos, slice), mipLevel);
	}
	
	return col;
}

ivec4 SampleTextureSInt4(vec2 pos, int type, bool flipY, int mipLevel, float slice)
{
	ivec4 col;
	if (type == RESTYPE_TEX1D)
	{
		int size = textureSize(texSInt1D, mipLevel);

		col = texelFetch(texSInt1D, int(pos.x), mipLevel);
	}
	else if (type == RESTYPE_TEX1DARRAY)
	{
		ivec2 size = textureSize(texSInt1DArray, mipLevel);

		col = texelFetch(texSInt1DArray, ivec2(pos.x, slice), mipLevel);
	}
	else if (type == RESTYPE_TEX2D)
	{
		ivec2 size = textureSize(texSInt2D, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		col = texelFetch(texSInt2D, ivec2(pos), mipLevel);
	}
	else if (type == RESTYPE_TEX2DARRAY)
	{
		ivec3 size = textureSize(texSInt2DArray, mipLevel);
		
		if (flipY)
			pos.y = size.y - pos.y;

		col = texelFetch(texSInt2DArray, ivec3(pos, slice), mipLevel);
	}
	else // if (type == RESTYPE_TEX3D)
	{
		ivec3 size = textureSize(texSInt3D, mipLevel);
		
		if (flipY)
			pos.y = size.y - pos.y;

		col = texelFetch(texSInt3D, ivec3(pos, slice), mipLevel);
	}
	
	return col;
}


vec4 SampleTextureFloat4(vec2 pos, int type, bool flipY, bool linearSample, int mipLevel, float slice)
{
	vec4 col;
	if (type == RESTYPE_TEX1D)
	{
		int size = textureSize(tex1D, mipLevel);

		if (linearSample)
			col = texture(tex1D, pos.x / size);
		else
			col = texelFetch(tex1D, int(pos.x), mipLevel);
	}
	else if (type == RESTYPE_TEX1DARRAY)
	{
		ivec2 size = textureSize(tex1DArray, mipLevel);

		if (linearSample)
			col = texture(tex1DArray, vec2(pos.x / size.x, slice));
		else
			col = texelFetch(tex1DArray, ivec2(pos.x, slice), mipLevel);
	}
	else if (type == RESTYPE_TEX2D)
	{
		ivec2 size = textureSize(tex2D, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		if (linearSample)
			col = texture(tex2D, pos / size);
		else
			col = texelFetch(tex2D, ivec2(pos), mipLevel);
	}
	else if (type == RESTYPE_TEX2DARRAY)
	{
		ivec3 size = textureSize(tex2DArray, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		if (linearSample)
			col = texture(tex2DArray, vec3(pos / size.xy, slice));
		else
			col = texelFetch(tex2DArray, ivec3(pos, slice), mipLevel);
	}
	else if (type == RESTYPE_TEX3D)
	{
		ivec3 size = textureSize(tex3D, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		if (linearSample)
			col = texture(tex3D, vec3(pos / size.xy, slice));
		else
			col = texelFetch(tex3D, ivec3(pos, slice), mipLevel);
	}
	else if (type == RESTYPE_TEXCUBE)
	{
		ivec2 size = textureSize(texCube, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		vec3 cubeCoord = CalcCubeCoord(pos / size, int(slice));

		if (linearSample)
			col = texture(texCube, cubeCoord);
		else
			col = textureLod(texCube, cubeCoord, mipLevel);
	}
	else // type == RESTYPE_TEXCUBEARRAY
	{
		ivec3 size = textureSize(texCubeArray, mipLevel);

		if (flipY)
			pos.y = size.y - pos.y;

		vec3 cubeCoord = CalcCubeCoord(pos / size.xy, int(slice) % 6);
		vec4 arrayCoord = vec4(cubeCoord, int(slice) / 6);

		if (linearSample)
			col = texture(texCubeArray, arrayCoord);
		else
			col = textureLod(texCubeArray, arrayCoord, mipLevel);
	}
	
	return col;
}
