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

	std::cout << "total number of voxels : " << skeleton.true_voxels().size() << std::endl;

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
	for (GRuint i(0); i < skeleton.true_voxels().size(); i++) {
		inside_count += (skeleton.voxel(skeleton.true_voxels()[i]).topological_class_ == BORDER_POINT);
	}
	std::cout << "number of inside points : " << inside_count << std::endl;
}


void BertandStructureTests(){

	VoxelSkeleton* skeleton = VoxelSkeleton::BertrandStructure();
	
	std::cout << "Bertrand's structure has a critical 2-clique at 211-221: "
		<< skeleton->is_critical_2_clique(2, 1, 1, Y_AXIS) << std::endl;

	
	std::cout << "Bertrand's structure has a critical 2-clique at 311-321: "
		<< skeleton->is_critical_2_clique(3, 1, 1, Y_AXIS) << std::endl;

	std::cout << "Bertrand's structure has a critical 2-clique at 321-322: "
		<< skeleton->is_critical_2_clique(3, 2, 1, Z_AXIS) << std::endl;

	std::cout << "Bertrand's structure has a critical 2-clique at 100-110: "
		<< skeleton->is_critical_2_clique(1, 0, 0, Y_AXIS)<<std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 012-022: "
		<< skeleton->is_critical_2_clique(0, 1, 2, Y_AXIS) << std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 022-122: "
		<< skeleton->is_critical_2_clique(0, 2, 2, X_AXIS) << std::endl;
	std::cout << "Bertrand's structure has a critical 2-clique at 211-311: "
		<< skeleton->is_critical_2_clique(2, 1, 1, X_AXIS) << std::endl;


	delete skeleton;

}


void TableLookupVSOnTheFlyCliqueCheck(){
	std::bitset<K2Y_CONFIGURATIONS> bitset_masks(0);

	VoxelSkeleton::precompute_K2_masks(bitset_masks);

	std::cout << std::endl << std::endl << " lookup table : " << std::endl;
	for (GRuint i(0); i < 100; i++) {
		std::cout << " " << bitset_masks[i + 1024 + 128];
	}

	std::cout << "lookup table size : " << sizeof(bitset_masks) << std::endl;

}

void SimpleVoxelTests() {
	VoxelSkeleton* skeleton = VoxelSkeleton::BertrandStructure();

	for (GRuint i(0); i < skeleton->true_voxels().size(); i++) {
		GRuint x, y, z;
		skeleton->voxel_id_to_coordinates(skeleton->true_voxels()[i], x, y, z);
		std::cout << " voxel " << x << " " << y << " " << z << " is simple : " << skeleton->is_simple(x, y, z) << std::endl;
	}

	//std::cout << " voxel  122 is simple : " << skeleton->is_simple(1, 2, 2) << std::endl;
	//not working yet

	delete skeleton;
}


void K1Tests() {
	VoxelSkeleton* skeleton = VoxelSkeleton::BertrandStructure();


	bool matches = skeleton->clique_matches_K1_mask(
		0, 0, 0, 
		0, 1, 0,
		0, 0, 1,
		0, 1, 1,
		 X_AXIS);
	std::cout << " nodes starting at 0 0 0 match the K1 mask : " << matches << std::endl;

	matches = skeleton->clique_matches_K1_mask(
		2, 1, 1,
		3, 1, 1,
		2, 1, 2,
		3, 1, 2,
		Y_AXIS);
	std::cout << " nodes starting at 2 1 1 match the K1 mask : " << matches << std::endl;

	matches = skeleton->clique_matches_K1_mask(
		2, 1, 1,
		3, 1, 1,
		2, 2, 1,
		3, 2, 1,
		Z_AXIS);
	std::cout << " nodes starting at 2 1 1 match the K1 mask : " << matches << std::endl;

	matches = skeleton->clique_matches_K1_mask(
		0, 1, 2,
		1, 1, 2,
		0, 2, 2,
		1, 2, 2,
		Z_AXIS);
	std::cout << " nodes starting at 0 1 2 match the K1 mask : " << matches << std::endl;


	delete skeleton;
}



void CliquesInBertrandStructureTest() {
	VoxelSkeleton* skeleton = VoxelSkeleton::BertrandStructure();

	std::vector<GRuint> critical_3_cliques;
	std::vector<std::vector<GRuint>> critical_2_cliques;
	std::vector<std::vector<GRuint>> critical_1_cliques;
	std::vector<std::vector<GRuint>> critical_0_cliques;


	skeleton->extract_all_cliques(critical_3_cliques, critical_2_cliques, critical_1_cliques, critical_0_cliques);


	GRuint x, y, z;

	std::cout << "critical cliques found in Bertrand structure : " << std::endl << endl;

	std::cout << " critical 3-cliques : " << critical_3_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_3_cliques.size(); i++) {
		skeleton->voxel_id_to_coordinates(critical_3_cliques[i], x, y, z);
		std::cout << " clique " << i << " : ( " << x << " " << y << " " << z << " )" << endl;
	}
	cout << endl;

	std::cout << " critical 2-cliques : " << critical_2_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_2_cliques.size(); i++) {
		cout << "clique " << i << " : " << endl;
		for (GRuint j(0); j < critical_2_cliques[i].size(); j++) {
			skeleton->voxel_id_to_coordinates(critical_2_cliques[i][j], x, y, z);
			std::cout << "( " << x << " " << y << " " << z << " )" << endl;
		}
		cout << endl;
	}
	cout << endl;

	std::cout << " critical 1-cliques : " << critical_1_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_1_cliques.size(); i++) {
		cout << "clique " << i << " : " << endl;
		for (GRuint j(0); j < critical_1_cliques[i].size(); j++) {
			skeleton->voxel_id_to_coordinates(critical_1_cliques[i][j], x, y, z);
			std::cout << "( " << x << " " << y << " " << z << " )" << endl;
		}
		cout << endl;
	}
	cout << endl;

	std::cout << " critical 0-cliques : " << critical_0_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_0_cliques.size(); i++) {
		cout << "clique " << i << " : " << endl;
		for (GRuint j(0); j < critical_0_cliques[i].size(); j++) {
			skeleton->voxel_id_to_coordinates(critical_0_cliques[i][j], x, y, z);
			std::cout << "( " << x << " " << y << " " << z << " )" << endl;
		}
		cout << endl;
	}
	cout << endl;



	delete skeleton;
}


void interiorBlockTest() {
	VoxelSkeleton* skeleton = new VoxelSkeleton(8, 8, 8);

	for (GRuint i(0); i < 4; i++) {
		for (GRuint j(0); j < 3; j++) {
			for (GRuint k(0); k < 3; k++) {
				skeleton->set_voxel(i, j, k);
			}
		}
	}

	std::vector<GRuint> critical_3_cliques;
	std::vector<std::vector<GRuint>> critical_2_cliques;
	std::vector<std::vector<GRuint>> critical_1_cliques;
	std::vector<std::vector<GRuint>> critical_0_cliques;

	
	skeleton->extract_all_cliques(critical_3_cliques, critical_2_cliques, critical_1_cliques, critical_0_cliques);


	GRuint x, y, z;

	std::cout << "critical cliques found in block structure : " << std::endl << endl;

	std::cout << " critical 3-cliques : " << critical_3_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_3_cliques.size(); i++) {
		skeleton->voxel_id_to_coordinates(critical_3_cliques[i], x, y, z);
		std::cout << " clique " << i << " : ( " << x << " " << y << " " << z << " )" << endl;
	}
	cout << endl;

	std::cout << " critical 2-cliques : " << critical_2_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_2_cliques.size(); i++) {
		cout << "clique " << i << " : " << endl;
		for (GRuint j(0); j < critical_2_cliques[i].size(); j++) {
			skeleton->voxel_id_to_coordinates(critical_2_cliques[i][j], x, y, z);
			std::cout << "( " << x << " " << y << " " << z << " )" << endl;
		}
		cout << endl;
	}
	cout << endl;

	std::cout << " critical 1-cliques : " << critical_1_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_1_cliques.size(); i++) {
		cout << "clique " << i << " : " << endl;
		for (GRuint j(0); j < critical_1_cliques[i].size(); j++) {
			skeleton->voxel_id_to_coordinates(critical_1_cliques[i][j], x, y, z);
			std::cout << "( " << x << " " << y << " " << z << " )" << endl;
		}
		cout << endl;
	}
	cout << endl;

	std::cout << " critical 0-cliques : " << critical_0_cliques.size() << std::endl;
	for (GRuint i(0); i < critical_0_cliques.size(); i++) {
		cout << "clique " << i << " : " << endl;
		for (GRuint j(0); j < critical_0_cliques[i].size(); j++) {
			skeleton->voxel_id_to_coordinates(critical_0_cliques[i][j], x, y, z);
			std::cout << "( " << x << " " << y << " " << z << " )" << endl;
		}
		cout << endl;
	}
	cout << endl;



	delete skeleton;
}

void blockCritical2Clique() {
		VoxelSkeleton* skeleton = new VoxelSkeleton(8, 8, 8);

		for (GRuint i(0); i < 4; i++) {
			for (GRuint j(0); j < 3; j++) {
				for (GRuint k(0); k < 3; k++) {
					skeleton->set_voxel(i, j, k);
				}
			}
		}


		std::vector<GRuint> critical_3_cliques;
		std::vector<std::vector<GRuint>> critical_2_cliques;
		std::vector<std::vector<GRuint>> critical_1_cliques;
		std::vector<std::vector<GRuint>> critical_0_cliques;


		skeleton->extract_all_cliques(critical_3_cliques, critical_2_cliques, critical_1_cliques, critical_0_cliques);


		GRuint x, y, z;

		std::cout << " critical 2-cliques : " << critical_2_cliques.size() << std::endl;
		for (GRuint i(0); i < critical_2_cliques.size(); i++) {
			cout << "clique " << i << " : " << endl;
			for (GRuint j(0); j < critical_2_cliques[i].size(); j++) {
				skeleton->voxel_id_to_coordinates(critical_2_cliques[i][j], x, y, z);
				std::cout << "( " << x << " " << y << " " << z << " )" << endl;
			}
			cout << endl;
		}
		cout << endl;

	delete skeleton;
}


int main()
{
	//blockCritical2Clique();

	//interiorBlockTest();
	//CliquesInBertrandStructureTest();

	TableLookupVSOnTheFlyCliqueCheck();

	while (true);
    return 0;
}

