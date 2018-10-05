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

#include "stdafx.h"
#include <iostream>
#include <bitset>

#include "VoxelSkeleton.hpp"

using namespace std;
using namespace grapholon;


void sand_box() {
	GRuint w(100), h(100), s(100);

	VoxelSkeleton skeleton(w, h, s);

	skeleton.generate_random_skeleton_like(5000, 15164);

	skeleton.compute_voxel_attributes();

	std::cout << "total number of voxels : " << skeleton.true_voxels_.size() << std::endl;

	//voxel list
	/*for (GRuint i(0); i< skeleton.true_voxels_.size(); i++){
	GRuint voxel_id = skeleton.true_voxels_[i];
	GRuint x, y, z;
	skeleton.voxel_id_to_coordinates(voxel_id,x,y,z);
	if (true || skeleton.voxels_[voxel_id].topological_class_ == INTERIOR_POINT) {
	std::cout << " voxel " << voxel_id << " at : " << x << ", " << y << ", " << z << " : " << skeleton.voxels_[voxel_id].topological_class_ << std::endl;
	}
	}*/

	//ids list
	/*for (GRuint i(0); i < skeleton.true_voxels_.size(); i++) {
	std::cout << " " << skeleton.true_voxels_[i];
	}
	std::cout << endl;
	*/

	//inside count
	GRuint inside_count(0);
	for (GRuint i(0); i < skeleton.true_voxels_.size(); i++) {
		inside_count += (skeleton.voxels_[skeleton.true_voxels_[i]].topological_class_ == BORDER_POINT);
	}
	std::cout << "number of inside points : " << inside_count << std::endl;

}

void BertandStructureTests(){


	VoxelSkeleton* skeleton = VoxelSkeleton::BertrandStructure();
	
	std::cout << "Bertrand's structure has a critical 2-clique at 211-221: "
		<< skeleton->is_critical_2clique(2, 1, 1, 2, 2, 1) << std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 221-211: "
		<< skeleton->is_critical_2clique(2, 2, 1, 2, 1, 1) << std::endl;

	
	std::cout << "Bertrand's structure has a critical 2-clique at 311-321: "
		<< skeleton->is_critical_2clique(3, 1, 1, 3, 2, 1) << std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 321-311: "
		<< skeleton->is_critical_2clique(3, 2, 1, 3, 1, 1) << std::endl;

	std::cout << "Bertrand's structure has a critical 2-clique at 321-322: "
		<< skeleton->is_critical_2clique(3, 2, 1, 3, 2, 2) << std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 322-321: "
		<< skeleton->is_critical_2clique(3, 2, 2, 3, 2, 1) << std::endl;

	std::cout << "Bertrand's structure has a critical 2-clique at 022-122: "
		<< skeleton->is_critical_2clique(0, 2, 2, 1, 2, 2)<<std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 122-022: "
		<< skeleton->is_critical_2clique(1, 2, 2, 0, 2, 2) << std::endl;
	

	delete skeleton;

}


void TableLookupVSOnTheFlyCliqueCheck(){
	std::bitset<K2Y_CONFIGURATIONS> bitset_masks(0);

	VoxelSkeleton::PrecomputeK2YMasks(bitset_masks);
	std::cout << std::endl << std::endl << " lookup table : " << std::endl;
	for (GRuint i(0); i < 100; i++) {
		std::cout << " " << bitset_masks[i];
	}

}


int main()
{

	BertandStructureTests();

	while (true);
    return 0;
}
