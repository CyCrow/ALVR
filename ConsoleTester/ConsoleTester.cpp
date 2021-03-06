// ConsoleTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>

#include <atltypes.h>
#include <directxmath.h>
struct VRTextureBounds_t
{
	float uMin, vMin;
	float uMax, vMax;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Tex;
	uint32_t View;
};

void InitWarpGeometry2(SimpleVertex *vtx, WORD * idx, float gamma, int sz, VRTextureBounds_t *bounds)
{
	float dx = (1.0f) / (sz - 1);
	float dy = (1.0f) / (sz - 1);

	int sz2 = sz * sz;
	// create vertices
	for (int y = 0; y < sz; ++y)
	{
		float fy = y * dy;
		float gy = 0.5f * powf(1.0f - 2.0f * fabs(fy - 0.5f), gamma);
		if (fy >= 0.5f)
			gy = 1.0f - gy;

		for (int x = 0; x < sz; ++x)
		{
			int pos = y * sz + x;
			float fx = x * dx; // 0-1
			float gx = 0.5f * powf(1.0f - 2.0f * fabs(fx - 0.5f), gamma);
			if (fx >= 0.5f)
				gx = 1.0f - gx;

			vtx[pos].Pos = DirectX::XMFLOAT3(-1.0f + gx, -1.0f + 2 * gy, 0.5f);
			vtx[pos].Tex = DirectX::XMFLOAT2(bounds[0].uMin + (fx * bounds[0].uMax), 1.0f - (bounds[0].vMin + (fy * bounds[0].vMax)));
			vtx[pos].View = 0;

			vtx[pos + sz2].Pos = DirectX::XMFLOAT3(0.0f + gx, -1.0f + 2 * gy, 0.5f);
			vtx[pos + sz2].Tex = DirectX::XMFLOAT2(bounds[1].uMin + (fx * bounds[1].uMax), 1.0f - (bounds[1].vMin + (fy * bounds[1].vMax)));
			vtx[pos + sz2].View = 1; // right eye
		}
	}

	// init triangle index buffer 
	int idxPos = 0;
	for (int y = 0; y < (sz - 1); ++y)
	{
		for (int x = 0; x < (sz - 1); ++x)
		{
			int pos = x * sz + y;

			idx[idxPos++] = pos;
			idx[idxPos++] = pos + 1;
			idx[idxPos++] = pos + sz;
			idx[idxPos++] = pos + 1;
			idx[idxPos++] = pos + sz + 1;
			idx[idxPos++] = pos + sz;
		}
	}
	int idxSz = (sz - 1) * (sz - 1) * 6;
	for (int i = 0; i < idxSz; ++i)
	{
		idx[idxSz + i] = idx[i] + sz2; // right eye
	}
}



int main()
{
    std::cout << "Hello World!\n"; 
	int sz = 5;
	int nIdx = (sz - 1)*(sz - 1) * 12;
	SimpleVertex *vtxBuf = new SimpleVertex[sz * sz * 2];
	WORD *idxBuf = new WORD[nIdx];
	VRTextureBounds_t bound[2];
	bound[0].uMin = bound[0].vMin = bound[1].uMin = bound[1].vMin = 0.0f;
	bound[0].uMax = bound[0].vMax = bound[1].uMax = bound[1].vMax = 1.0f;

	InitWarpGeometry2(vtxBuf, idxBuf, 1.5f, sz, bound);

	for (int y = 0; y < sz; ++y)
	{
		for (int x = 0; x < sz; ++x)
		{
			int pos = y * sz + x;
			int pos2 = pos + sz * sz;
			char buf[1000];
			snprintf(buf, 1000, "%d:(%.3f,%.3f) (%.3f,%.3f) \t", pos,
				vtxBuf[pos].Pos.x, vtxBuf[pos].Pos.y,
				vtxBuf[pos].Tex.x, vtxBuf[pos].Tex.y);

			std::cout << buf;			
		}
		std::cout << "\n";
	}

	for (int i = 0; i < nIdx; ++i)
		std::cout << idxBuf[i] << " ";

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
