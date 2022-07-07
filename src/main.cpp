#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdint.h>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

import test;
import vec3;

typedef uint32_t Block;

const int width = 960;
const int height = 540;
const int channels = 4;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct OctreeNode {
	bool allSame;
	Block voxelType;
	uint32_t children[8];
};

Color voxelData[] = {
	{.r=0, .g=0, .b=0, .a=0},
	{.r=0, .g=255, .b=0, .a=255},
};

const int voxelEdgeLength = 256;

Block voxels[voxelEdgeLength][voxelEdgeLength][voxelEdgeLength];

vector<OctreeNode> tree;
uint32_t root;
vec3 cameraPos = {.x = voxelEdgeLength/2, .y = voxelEdgeLength/2, .z = voxelEdgeLength/2};
vec3 cameraAngle = {.x = 2, .y = 0, .z = -M_PI/2};
const float fov = 80*M_PI/180;

void initVoxels() {
	for(int x = 0; x < voxelEdgeLength; x++) {
		for(int y = 0; y < voxelEdgeLength; y++) {
			for(int z = 0; z < voxelEdgeLength; z++) {
				if(y*y < x - z*sin(z)/2) {
					voxels[x][y][z] = 1;
				} else {
					voxels[x][y][z] = 0;
				}
			}
		}
	}
}

uint32_t recursiveInit(int x, int y, int z, int size) {
	OctreeNode node;
	Block type = voxels[x][y][z];
	for(int dx = 0; dx < size; dx++) {
		for(int dy = 0; dy < size; dy++) {
			for(int dz = 0; dz < size; dz++) {
				if(voxels[x+dx][y+dy][z+dz] != type) {
					// Voxels are different → go deeper.
					node.children[0] = recursiveInit(x, y, z, size/2);
					node.children[1] = recursiveInit(x, y, z + size/2, size/2);
					node.children[2] = recursiveInit(x, y + size/2, z, size/2);
					node.children[3] = recursiveInit(x, y + size/2, z + size/2, size/2);
					node.children[4] = recursiveInit(x + size/2, y, z, size/2);
					node.children[5] = recursiveInit(x + size/2, y, z + size/2, size/2);
					node.children[6] = recursiveInit(x + size/2, y + size/2, z, size/2);
					node.children[7] = recursiveInit(x + size/2, y + size/2, z + size/2, size/2);
					node.allSame = false;
					tree.push_back(node);
					return tree.size() - 1;
				}
			}
		}
	}
	node.allSame = true;
	node.voxelType = type;
	tree.push_back(node);
	return tree.size() - 1;
}

void initOctree() {
	root = recursiveInit(0, 0, 0, voxelEdgeLength);
}

uint32_t childIndex(ivec3 pos, int size) {
	pos = pos/size;
	pos = pos & 1;
	return pos.x*4 + pos.y*2 + pos.z;
}

int main() {
    Color* data = (Color*)malloc(width*height*sizeof(Color));

	initVoxels();
	initOctree();
	
	for(int x = 0; x < width; x++) {
		cout << x << "\n";
		for(int y = 0; y < height; y++) {
			vec3 rayDir = {.x = x-width/2.0f, .y = y-height/2.0f, .z = width/sin(fov)};
			rayDir = {
				.x = rayDir.x*cos(cameraAngle.z) - rayDir.y*sin(cameraAngle.z),
				.y = rayDir.x*sin(cameraAngle.z) + rayDir.y*cos(cameraAngle.z),
				.z = rayDir.z,
			};
			rayDir = {
				.x = rayDir.x*cos(cameraAngle.y) - rayDir.z*sin(cameraAngle.y),
				.y = rayDir.y,
				.z = rayDir.x*sin(cameraAngle.y) + rayDir.z*cos(cameraAngle.y),
			};
			rayDir = {
				.x = rayDir.x,
				.y = rayDir.y*cos(cameraAngle.x) - rayDir.z*sin(cameraAngle.x),
				.z = rayDir.y*sin(cameraAngle.x) + rayDir.z*cos(cameraAngle.x),
			};
			if(rayDir.x == 0) rayDir.x = 1e-6f;
			if(rayDir.y == 0) rayDir.y = 1e-6f;
			if(rayDir.z == 0) rayDir.z = 1e-6f;
			rayDir.normalize();
			cout << rayDir.x << " " << rayDir.y << " " << rayDir.z << "\n";
			
			uint32_t path[32];
			path[0] = root;
			int depth = 0;
			int curSize = voxelEdgeLength;
			
			ivec3 voxelPosition = vec3_to_ivec3(cameraPos.floor());
			while(!tree[path[depth]].allSame) {
				curSize /= 2;
				path[depth + 1] = tree[path[depth]].children[childIndex(voxelPosition, curSize)];
				depth++;
			}
			ivec3 step = rayDir.sign();
			vec3 t1 = (cameraPos.floor() - cameraPos)/rayDir;
			vec3 tDelta = vec3FromFloat(1)/(rayDir);
			vec3 t2 = t1 + tDelta;
			tDelta = tDelta.abs();
			vec3 tLast = max(t1, t2) - tDelta;//*min(curSize - ((voxelPosition*step + 0*(step - 1)/2) & (curSize - 1)), ivec3(2, 1, 1));
			cout << depth << " asa" << " " << tLast.x << " " << tLast.y << " " << tLast.z << " " << curSize << "\n";
			int it = 0;
			float total_t = 0;
			while(voxelData[tree[path[depth]].voxelType].a == 0 && it < 255) {
				cout << voxels[voxelPosition.x & 15][voxelPosition.y & 15][voxelPosition.z & 15] << "\n";
				cout << voxelPosition.x << " " << voxelPosition.y << " " << voxelPosition.z << "\n";
				ivec3 currentSteps = min(ivec3FromInt(curSize) - ((voxelPosition*step + (step - 1)/2) & (curSize - 1)), ivec3(40, 100, 100));
				vec3 NextT = tLast + tDelta*(currentSteps);
				total_t = min(NextT.x, min(NextT.y, NextT.z));
				ivec3 missingSteps = vec3_to_ivec3(((vec3FromFloat(total_t) - tLast)/tDelta + 0.0001f).floor());
				ivec3 nextPosition = voxelPosition + missingSteps*step;
				tLast += tDelta*missingSteps;
				it++;
				//curSize *= 2;
				//depth--;
				// Going up the tree:
				while((nextPosition & curSize) != (voxelPosition & curSize)) {
					curSize *= 2;
					depth--;
				}
				cout << depth << " " << nextPosition.x << " " << nextPosition.y << " " << nextPosition.z << " " << tLast.x << " " << tLast.y << " " << tLast.z << " " << curSize << "\n";
				if(depth < 0) {
					data[x + y*width] = {.r=255, .g=0, .b=0, .a=255};
					goto continue_loop;
				} // Outside the box
				voxelPosition = nextPosition;
				// Going down the tree:
				while(!tree[path[depth]].allSame) {
					curSize /= 2;
					path[depth + 1] = tree[path[depth]].children[childIndex(voxelPosition, curSize)];
					depth++;
				}
			}

			data[x + y*width] = voxelData[tree[path[depth]].voxelType];
			if(total_t == tLast.x) {
				data[x + y*width].r = 127;
			}
			if(total_t == tLast.y) {
				data[x + y*width].g = 127;
			}
			if(total_t == tLast.z) {
				data[x + y*width].b = 127;
			}
			continue_loop:;
		}
	}

    stbi_write_png("../test.png", width, height, channels, data, width * channels);
	free(data);
}

/*
	ivec3 step = ivec3(sign(Direction));
	vec3 t1 = (floor(CameraPosition) - CameraPosition)/Direction;
	vec3 tDelta = 1/(Direction);
	vec3 t2 = t1 + tDelta;
	tDelta = abs(tDelta);
	vec3 tLast = max(t1, t2) - tDelta;//*min(curSize - ((voxelPosition*step + 0*(step - 1)/2) & (curSize - 1)), ivec3(2, 1, 1));
	if(Direction.x == 0) tLast.x = 1.0/0.0;
	if(Direction.y == 0) tLast.y = 1.0/0.0;
	if(Direction.z == 0) tLast.z = 1.0/0.0;
	int It = 0;
	float total_t = 0;
	while(Tree[path[depth]].A == 0 && It < 255) {
		ivec3 currentSteps = min(curSize - ((voxelPosition*step + (step - 1)/2) & (curSize - 1)), ivec3(40, 100, 100));
		vec3 NextT = tLast + tDelta*(currentSteps);
		total_t = min(NextT.x, min(NextT.y, NextT.z));
		ivec3 missingSteps = ivec3(floor((total_t - tLast)/tDelta + 0.0001));
		ivec3 nextPosition = voxelPosition + missingSteps*step;
		tLast += missingSteps*tDelta;
		It++;
		//curSize *= 2;
		//depth--;
		// Going up the tree:
		while((nextPosition & curSize) != (voxelPosition & curSize)) {
			curSize *= 2;
			depth--;
		}
		if(depth < 0) discard; // Outside the box
		voxelPosition = nextPosition;
		// Going down the tree:
		while(Tree[path[depth]].AllSame == 0) {
			curSize /= 2;
			path[depth + 1] = Tree[path[depth]].Children[ChildIndex(voxelPosition, curSize)];
			depth++;
		}
	}
*/