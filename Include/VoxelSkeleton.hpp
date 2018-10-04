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
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "GrapholonTypes.hpp"
#include "common.hpp"

namespace grapholon {

	enum VoxelState {
		visible = 0,
		fixed = 1,
		hidden = 2
	};

	struct Voxel {
		int value;
		int threshold;
		VoxelState state;
	};

	enum Clique{ NO_CLIQUE, CLIQUE3, CLIQUE2, CLIQUE1, CLIQUE0 };
	enum TopologicalClass{UNCLASSIFIED, INTERIOR_POINT, ISOLATED_POINT, BORDER_POINT, CURVES_POINT, CURVE_JUNCTION, SURFACE_CURVES_JUNCTION, SURFACE_JUNCTION, SURFACES_CURVE_JUNCTION};
	
	struct SkeletonVoxel {
		bool value_ = false;
		TopologicalClass topological_class_ = UNCLASSIFIED;
	};


	//todo
	class zero_neighbour_iterator {

	};


	class VoxelSkeleton {
	public:

		const GRuint width_;
		const GRuint height_;
		const GRuint slice_;

		const GRuint nb_voxels_;

		SkeletonVoxel* voxels_;

		std::vector<GRuint> true_voxels_;

		VoxelSkeleton(GRuint width, GRuint height, GRuint slice) : width_(width), height_(height), slice_(slice), nb_voxels_(width*height*slice) {
			voxels_ = (SkeletonVoxel*)calloc(nb_voxels_, sizeof(SkeletonVoxel));
			memset(voxels_, 0, nb_voxels_ *sizeof(SkeletonVoxel));
		}

		~VoxelSkeleton(){
			free(voxels_);
		}

		SkeletonVoxel& voxel(GRuint id) {
			return voxels_[id];
		}

		const SkeletonVoxel& voxel(GRuint id) const {
			return voxels_[id];
		}

		SkeletonVoxel& voxel(GRuint x, GRuint y, GRuint z) {
			return voxels_[voxel_coordinates_to_id(x, y, z)];
		}

		const SkeletonVoxel& voxel(GRuint x, GRuint y, GRuint z) const{
			return voxels_[voxel_coordinates_to_id(x, y, z)];
		}

		const std::vector<GRuint>& true_voxels()const {
			return true_voxels_;
		}


		GRuint voxel_coordinates_to_id(GRuint x, GRuint y, GRuint z) const {
			return x + (y + z * height_) * width_;
		}

		/**
		current convention : if the relative coordinates are out-of-bounds they're clamped to within-bounds coordinates
		e.g. (-1, 3, 4) -> (0, 3, 4);  (5, 2, slice + 3) -> (5, 2, slice - 1)*/
		GRuint voxel_coordinates_and_relative_coordinates_to_id(GRuint x, GRuint y, GRuint z, GRint rel_x, GRint rel_y, GRint rel_z) const {
			GRint final_x = x + rel_x;
			GRint final_y = x + rel_y;
			GRint final_z = x + rel_z;

			final_x = MAX(0, MIN(final_x, (GRint)width_ - 1));
			final_y = MAX(0, MIN(final_y, (GRint)height_ - 1));
			final_z = MAX(0, MIN(final_z, (GRint)slice_ - 1));

			return voxel_coordinates_to_id(final_x, final_y, final_z);
		}
		
		void voxel_id_to_coordinates(GRuint id, GRuint& x, GRuint& y, GRuint& z) const {
			z = id / (width_ * height_);
			GRint rem = id % (width_ * height_);
			y = rem / width_;
			x = rem % width_;
		}

		GRuint voxel_id_and_relative_coordinates_to_id(GRuint id, GRint rel_x, GRint rel_y, GRint rel_z) {
			GRuint x, y, z;
			voxel_id_to_coordinates(id, x, y, z);
			return voxel_coordinates_and_relative_coordinates_to_id(x, y, z, rel_x, rel_y, rel_z);
		}

		bool set_voxel(GRuint id, bool value = true) {
			if (id >= nb_voxels_) {
				return false;
			}

			voxels_[id].value_ = value;
			voxels_[id].topological_class_ = UNCLASSIFIED;

			if (value) {
				true_voxels_.push_back(id);
			}
			else {
				true_voxels_.erase(std::remove(true_voxels_.begin(), true_voxels_.end(), id), true_voxels_.end());
			}

/*
std::cout << "set voxel : " << id << ". true voxel list is now : " << std::endl;
for (GRuint i(0); i < true_voxels_.size(); i++) {
	std::cout << " " << true_voxels_[i];
}
std::cout << std::endl;
*/

return true;
		}

		bool set_voxel(GRuint x, GRuint y, GRuint z, bool value = true) {
			return set_voxel(voxel_coordinates_to_id(x, y, z), value);
		}

		bool match_mask(GRuint x, GRuint y, GRuint z, GRuint mask_width, GRuint mask_height, GRuint mask_slice, SkeletonVoxel*** mask) {

			return true;
		}


		//only does interior and border points for now
		void compute_voxel_attributes() {

			for (GRuint i(0); i < true_voxels_.size(); i++) {
				GRuint voxel_id = true_voxels_[i];

				if (voxels_[voxel_id].value_) {
					GRuint x, y, z;
					voxel_id_to_coordinates(voxel_id, x, y, z);

					//first check if it's a border (incomplete, only checks for voxels on the boundary)
					if (x == 0 || y == 0 || z == 0 || x == width_ - 1 || y == height_ - 1 || z == slice_ - 1) {
						voxels_[voxel_id].topological_class_ = BORDER_POINT;
					}
					else if (
						voxels_[voxel_coordinates_to_id(x + 1, y, z)].value_
						&& voxels_[voxel_coordinates_to_id(x - 1, y, z)].value_
						&& voxels_[voxel_coordinates_to_id(x, y + 1, z)].value_
						&& voxels_[voxel_coordinates_to_id(x, y - 1, z)].value_
						&& voxels_[voxel_coordinates_to_id(x, y, z + 1)].value_
						&& voxels_[voxel_coordinates_to_id(x, y, z - 1)].value_) {

						voxels_[voxel_id].topological_class_ = INTERIOR_POINT;
					}
					else {
						voxels_[voxel_id].topological_class_ = UNCLASSIFIED;
					}
				}
			}
		}

		bool are_0adjacent(GRuint id, GRuint id2) {
			return id + 1 + width_ + width_ * height_ == id2
				|| id + 1 + width_ - width_ * height_ == id2
				|| id + 1 - width_ + width_ * height_ == id2
				|| id + 1 - width_ - width_ * height_ == id2
				|| id - 1 + width_ + width_ * height_ == id2
				|| id - 1 + width_ - width_ * height_ == id2
				|| id - 1 - width_ + width_ * height_ == id2
				|| id - 1 - width_ - width_ * height_ == id2
				|| are_1adjacent(id, id2);
		}

		bool are_1adjacent(GRuint id, GRuint id2) {
			return id + 1 + width_ == id2
				|| id + 1 - width_ == id2
				|| id - 1 + width_ == id2
				|| id - 1 - width_ == id2
				|| id + 1 + width_ * height_ == id2
				|| id + 1 - width_ * height_ == id2
				|| id - 1 + width_ * height_ == id2
				|| id - 1 - width_ * height_ == id2
				|| id + width_ + width_ * height_ == id2
				|| id + width_ - width_ * height_ == id2
				|| id - width_ + width_ * height_ == id2
				|| id - width_ - width_ * height_ == id2
				|| are_2adjacent(id, id2);
		}

		bool are_2adjacent(GRuint id, GRuint id2) {
			return id + 1 == id2
				|| id - 1 == id2
				|| id + width_ == id2
				|| id - width_ == id2
				|| id + width_ * height_ == id2
				|| id - width_ * height_ == id2;
		}
		bool are_0adjacent(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {
			return are_0adjacent(voxel_coordinates_to_id(x, y, z), voxel_coordinates_to_id(x2, y2, z2));
		}
		bool are_1adjacent(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {
			return are_1adjacent(voxel_coordinates_to_id(x, y, z), voxel_coordinates_to_id(x2, y2, z2));
		}
		bool are_2adjacent(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {
			return are_2adjacent(voxel_coordinates_to_id(x, y, z), voxel_coordinates_to_id(x2, y2, z2));
		}

		//NOTE : this is very greedy (O(n^2), n = voxels_ids.size()) but it is not used at runtime so it's fine 
		bool is_0connected(std::vector<GRuint> voxel_ids) {
			std::cout << "checking 0-connectdedness of voxels : ";
			for (GRuint i(0); i < voxel_ids.size(); i++) {
				std::cout << " " << voxel_ids[i];
			}
			std::cout << std::endl;

			std::vector<bool> visited(voxel_ids.size(), false);
			std::vector<bool> explored(voxel_ids.size(), false);

			bool result(false);
			bool uncertain(true);
			GRuint last_explored_index(0);
			GRuint iteration_count(0);
			while (uncertain && iteration_count < visited.size()) {
				iteration_count++;

				//visit all neighbors of the last visited
				for (GRuint i(0); i < voxel_ids.size(); i++) {
					if (!visited[i] && are_0adjacent(voxel_ids[last_explored_index],
						voxel_ids[i])) {
						visited[i] = true;
						std::cout << "visited voxel : " << i << std::endl;
					}
				}
				explored[last_explored_index] = true;
				visited[last_explored_index] = true;

				bool found_unexplored(false);

				GRuint next_explored_index(0);
				while (next_explored_index < explored.size()
					&& !found_unexplored) {

					if (visited[next_explored_index] && !explored[next_explored_index]) {
						found_unexplored = true;
						last_explored_index = next_explored_index;
						std::cout << "found unexplored : " << next_explored_index << std::endl;
					}
					next_explored_index++;
				}

				std::cout << "last explored index is now : " << last_explored_index << std::endl;

				//if we reached the end of the array, we check if all voxels have been visited
				if (!found_unexplored || last_explored_index == explored.size() - 1) {
					bool visited_voxels_are_also_explored(true);
					bool all_visited(true);
					for (GRuint i(0); i < visited.size(); i++) {
						if (visited[i]) {
							visited_voxels_are_also_explored &= explored[i];
						}
						all_visited &= visited[i];
					}
					if (visited_voxels_are_also_explored) {
						uncertain = false;
						result = all_visited;
					}
				}
				std::cout << "visited : ";
				for (GRuint i(0); i < visited.size(); i++) {
					std::cout << " " << visited[i];
				}
				std::cout << std::endl;
				std::cout << "explored : ";
				for (GRuint i(0); i < explored.size(); i++) {
					std::cout << " " << explored[i];
				}
				std::cout << std::endl;
			}

			std::cout << " nb voxels : " << voxel_ids.size() << ", iterations : " << iteration_count << std::endl;

			return result;
		}


		/**K_2 mask matchings. the letter corresponds to the axis
		TODO : check if symmetric (i.e. matches_k2x(x,y,z,x2,y2,z2) = matches_k2x(x2,y2,z2,x,y,z)*/

		bool matches_K2X_mask(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {
			return false;
		}

		bool matches_K2Y_mask(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {

			//first create the list of voxels in {X0,...,X7, Y0,...,Y7}AND X (at most 16 voxels)
			std::vector<GRuint> mask_neighborhood_intersection;

			std::cout << "checking clique (" << x << ", " << y << ", " << z<<") - ("<<x2<<", "<<y2<<", "<<z2<<")"<< std::endl;


			for (GRuint i(0); i < 3; i++) {
				for (GRuint j(0); j < 3; j++) {
					if (!(i == 1 && j == 1)) {
						std::cout << "checking interesection at coordinates " << x - 1 + i<<", "<< y<<", "<< z - 1 + j << std::endl;
						if (voxels_[voxel_coordinates_to_id(x - 1 + i, y, z - 1 + j)].value_) {
							mask_neighborhood_intersection.push_back(
								voxel_coordinates_to_id(x - 1 + i, y, z - 1 + j));
							std::cout << "found X neighbor" << std::endl;
						}
						std::cout << "checking interesection at coordinates " << x2 - 1 + i << ", " << y2 << ", " << z2 - 1 + j << std::endl;
						if (voxels_[voxel_coordinates_to_id(x2 - 1 + i, y2, z2 - 1 + j)].value_) {
							mask_neighborhood_intersection.push_back(
								voxel_coordinates_to_id(x2 - 1 + i, y2, z2 - 1 + j));
							std::cout << "found Y neighbor" << std::endl;
						}
					}
				}
			}


			//first check if the set of intersection is empty
			if (!mask_neighborhood_intersection.size()) {
				return true;
			}

			std::cout << "neighborhood is non-empty..." << std::endl;

			//then check if they are 0-connected 
			//(i.e. if even one voxel of the mask has no 0-neighbor
			if (!is_0connected(mask_neighborhood_intersection)) {
				return true;
			}
			std::cout << "neighborhood is 0-connected" << std::endl;

			return false;
			//finally check if for each i in {0,2,4,6}, Xi or Yi is in X
			bool is_in_subset =
				(voxels_[voxel_coordinates_to_id(x + 1, y, z)].value_ //X0
					|| voxels_[voxel_coordinates_to_id(x2 + 1, y2, z2)].value_)//Y0
				&& (voxels_[voxel_coordinates_to_id(x, y, z + 1)].value_ //X2
					|| voxels_[voxel_coordinates_to_id(x2, y2, z2 + 1)].value_)//Y2
				&& (voxels_[voxel_coordinates_to_id(x - 1, y, z)].value_ //X2
					|| voxels_[voxel_coordinates_to_id(x2 - 1, y2, z2)].value_)//Y2
				&& (voxels_[voxel_coordinates_to_id(x, y, z - 1)].value_ //X2
					|| voxels_[voxel_coordinates_to_id(x2, y2, z2 - 1)].value_);//Y2


			if (is_in_subset) {
				std::cout << "there is a voxel in each Xi or Yi" << std::endl;
			} else {
				std::cout << "there is not a voxel in each Xi or Yi" << std::endl;

			}

			return is_in_subset;
		}

		bool matches_K2Z_mask(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {
			return false;
		}

		bool has_critical_2clique(GRuint x, GRuint y, GRuint z, GRuint x2, GRuint y2, GRuint z2) {
			
			if (!are_2adjacent(x, y, z, x2, y2, z2)) {
				return false;
			}

			if (x != x2) {
				return matches_K2X_mask(x, y, z, x2, y2, z2);
			}
			else if(y != y2) {
				return matches_K2Y_mask(x, y, z, x2, y2, z2);
			}
			else if (z != z2) {
				return matches_K2Z_mask(x, y, z, x2, y2, z2);
			}
			else {
				std::cerr << " ERROR - 'are_2neighbors' probably failed" << std::endl;
				exit(EXIT_FAILURE);
			}


			return false;
		}

		static VoxelSkeleton* BertrandStructure() {
			GRuint w(10), h(10), s(10);

			VoxelSkeleton* skeleton = new VoxelSkeleton(w, h, s);

			skeleton->set_voxel(2, 1, 1);
			skeleton->set_voxel(2, 2, 1);
			skeleton->set_voxel(5, 2, 1);
			skeleton->set_voxel(3, 2, 2);
			skeleton->set_voxel(4, 2, 2);
			skeleton->set_voxel(3, 3, 2);
			skeleton->set_voxel(4, 3, 2);
			skeleton->set_voxel(1, 2, 3);
			skeleton->set_voxel(1, 3, 3);
			skeleton->set_voxel(2, 3, 3);
			skeleton->set_voxel(4, 3, 3);
			skeleton->set_voxel(5, 4, 3);


			return skeleton;
		}


		/**RANDOM GENERATION METHODS*/
		void generate_random(GRuint nb_voxels, GRuint seed = 1234) {
			srand(seed);

			//erasing voxel grid
			memset(voxels_, 0, nb_voxels_ * sizeof(SkeletonVoxel));

			GRint current_voxel_id = rand() % nb_voxels_;

			GRuint x, y, z;

			for (GRuint i(0); i < nb_voxels; i++) {

				voxel_id_to_coordinates(current_voxel_id, x, y, z);

				set_voxel(current_voxel_id);


				GRint next_voxel_id = -1;
				GRuint random_voxel_index = rand() % true_voxels_.size();
				GRuint random_voxel_id = true_voxels_[random_voxel_index];
				/*std::cout << "random index : " << random_voxel_index << std::endl;
				std::cout << "true voxels size : " << true_voxels_.size() << std::endl;
				std::cout << "random voxels id : " << random_voxel_id << std::endl;*/

				while (next_voxel_id < 0 || next_voxel_id >= (GRint)nb_voxels_ || voxels_[next_voxel_id].value_) {
					random_voxel_index = rand() % true_voxels_.size();
					random_voxel_id = true_voxels_[random_voxel_index];

					GRuint face_index = rand() % 6;

					switch (face_index) {
					case 0: next_voxel_id = random_voxel_id + 1; break;//right
					case 1: next_voxel_id = random_voxel_id - 1; break;//left
					case 2: next_voxel_id = random_voxel_id + width_; break;//back
					case 3: next_voxel_id = random_voxel_id - width_; break;//front
					case 4: next_voxel_id = random_voxel_id + height_*width_; break;//top
					case 5: next_voxel_id = random_voxel_id - height_*width_; break;//bottom
					}
				}

				current_voxel_id = next_voxel_id;
				
			}

		}


		void generate_random_skeleton_like(GRuint nb_voxels, GRuint seed = 1234) {

			srand(seed);
			//erasing voxel grid
			memset(voxels_, 0, nb_voxels_ * sizeof(SkeletonVoxel));

			GRuint nb_skeleton_voxels(nb_voxels / 10);
			//phase 1 : generate skeleton
			
			GRuint root_voxel_id = voxel_coordinates_to_id(width_/2, height_/2, slice_/2);
			set_voxel(root_voxel_id);

			set_voxel(root_voxel_id + 1);
			set_voxel(root_voxel_id + 2);
			set_voxel(root_voxel_id + 3);
			set_voxel(root_voxel_id + 4);
			set_voxel(root_voxel_id + 5);

			GRuint current_voxel_id = root_voxel_id;

			GRuint branch_length(nb_skeleton_voxels / 5);

			GRuint previous_face_index(4);

			for (GRuint i(0); i < nb_skeleton_voxels; i++) {
				GRint next_voxel_id = -1;

				GRuint tried_face_count(0);

				while (next_voxel_id < 0 || next_voxel_id >= (GRint)nb_voxels_ || voxels_[next_voxel_id].value_) {

					GRuint face_index = rand() % 6;
					if (rand() % 100 < 80) {
						face_index = previous_face_index;
					}

					switch (face_index) {
					case 0: next_voxel_id = current_voxel_id + 1; break;//right
					case 1: next_voxel_id = current_voxel_id - 1; break;//left
					case 2: next_voxel_id = current_voxel_id + width_; break;//back
					case 3: next_voxel_id = current_voxel_id - width_; break;//front
					case 4: next_voxel_id = current_voxel_id + height_*width_; break;//top
					case 5: next_voxel_id = current_voxel_id - height_*width_; break;//bottom
					}

					previous_face_index = face_index;

					if (tried_face_count++ >= 6) {
						current_voxel_id = true_voxels_[rand() % true_voxels_.size()];
					}
				}

				current_voxel_id = next_voxel_id;
				set_voxel(current_voxel_id);

				if ((i % branch_length) == 0) {
					current_voxel_id = true_voxels_[rand() % true_voxels_.size()];
					//std::cout << "branching at iteration " << i << std::endl;
					//std::cout << "voxel id : " << current_voxel_id << std::endl;
				}
			}


			//phase 2: thicken skeleton
			for (GRuint i(true_voxels_.size()); i < nb_voxels; i++) {
				//std::cout << "true voxel count : " << true_voxels_.size() << std::endl;

				GRint next_voxel_id = -1;
				GRuint random_voxel_index = rand() % true_voxels_.size();
				GRuint random_voxel_id = true_voxels_[random_voxel_index];
				/*std::cout << "random index : " << random_voxel_index << std::endl;
				std::cout << "true voxels size : " << true_voxels_.size() << std::endl;
				std::cout << "random voxels id : " << random_voxel_id << std::endl;*/

				GRuint tried_face_count(0);

				while (next_voxel_id < 0 || next_voxel_id >= (GRint)nb_voxels_ || voxels_[next_voxel_id].value_) {
					random_voxel_index = rand() % true_voxels_.size();
					random_voxel_id = true_voxels_[random_voxel_index];

					GRuint face_index = rand() % 6;

					switch (face_index) {
					case 0: next_voxel_id = random_voxel_id + 1; break;//right
					case 1: next_voxel_id = random_voxel_id - 1; break;//left
					case 2: next_voxel_id = random_voxel_id + width_; break;//back
					case 3: next_voxel_id = random_voxel_id - width_; break;//front
					case 4: next_voxel_id = random_voxel_id + height_*width_; break;//top
					case 5: next_voxel_id = random_voxel_id - height_*width_; break;//bottom
					}

					if (tried_face_count++ >= 6) {
						random_voxel_id = true_voxels_[rand() % true_voxels_.size()];
					}
				}

				//std::cout << "set voxel : " << random_voxel_id << std::endl;
				set_voxel(next_voxel_id);

			}

		}

	};

}
