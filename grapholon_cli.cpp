//Copyright (c) 2018 Valentin NIGOLIAN
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//
//

// grapholon.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <bitset>
#include <ctime>

#include "SkeletalGraph.hpp"
#include "VoxelSkeleton.hpp"

using namespace std;
using namespace grapholon;




void ExtractGraphFromTestStructure() {
	VoxelSkeleton* skeleton = new VoxelSkeleton(10, 10, 10);

	skeleton->set_voxel(0, 4, 0);
	
	skeleton->set_voxel(1, 3, 0);
	
	skeleton->set_voxel(2, 2, 0);
	skeleton->set_voxel(2, 6, 0);

	skeleton->set_voxel(3, 0, 0);
	skeleton->set_voxel(3, 1, 0);
	skeleton->set_voxel(3, 2, 0);
	skeleton->set_voxel(3, 6, 0);

	skeleton->set_voxel(4, 3, 0);
	skeleton->set_voxel(4, 4, 0);
	skeleton->set_voxel(4, 5, 0);

	skeleton->set_voxel(5, 6, 0);

	skeleton->set_voxel(6, 6, 0);


	SkeletalGraph* graph = skeleton->extract_skeletal_graph();

	graph->print_composition();

	delete graph;
	delete skeleton;
}


void ExtractGraphFromTestStructure2() {
	VoxelSkeleton* skeleton = new VoxelSkeleton(10, 10, 10);

	skeleton->set_voxel(2, 3, 0);
	skeleton->set_voxel(2, 4, 0);

	skeleton->set_voxel(3, 2, 0);
	skeleton->set_voxel(3, 5, 0);

	skeleton->set_voxel(4, 2, 0);
	skeleton->set_voxel(4, 4, 0);
	//skeleton->set_voxel(4, 7, 0);

	skeleton->set_voxel(5, 3, 0);

	skeleton->set_voxel(6, 2, 0);
	skeleton->set_voxel(6, 4, 0);
	skeleton->set_voxel(6, 5, 0);
	skeleton->set_voxel(6, 6, 0);

	skeleton->set_voxel(7, 2, 0);
	skeleton->set_voxel(7, 7, 0);

	skeleton->set_voxel(8, 1, 0);
	skeleton->set_voxel(8, 3, 0);

	SkeletalGraph* graph = skeleton->extract_skeletal_graph();

	graph->print_composition();

	delete graph;
	delete skeleton;
}

int main()
{
	ExtractGraphFromTestStructure();

	ExtractGraphFromTestStructure2();

    return 0;
}
