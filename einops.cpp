#include <bits/stdc++.h>

using namespace std;

typedef struct tensor {
	vector<int> dimensions;
	vector<int> data;
	
	struct tensor operator[](vector<int> idxs){
		assert(idxs.size() <= dimensions.size());
		
		struct tensor result;
		int ret_size = 1;
		for(int i = idxs.size(); i < dimensions.size(); i++){
			ret_size *= dimensions[i];
			result.dimensions.push_back(dimensions[i]);
		}

		int curr_size = ret_size;
		int idx = 0;

		for(int i = idxs.size() - 1; i >= 0; i--){
			assert(dimensions[i] > idxs[i]); // check for out of bounds
			idx += idxs[i] * curr_size;
			curr_size *= dimensions[i];
		}
		for(int i = 0; i < ret_size; i++){
			result.data.push_back(data[idx + i]);
		}
		if(result.data.size() != 0 && result.dimensions.size() == 0){
			result.dimensions.push_back(1);
		}
		return result;
	}

	void print_dim(ostream& os, int dim_idx, int data_idx, int curr_size){
		if(dim_idx != dimensions.size() - 1){
			os << "[\n";
			int block_size = curr_size / dimensions[dim_idx];
			for(int i = 0; i < dimensions[dim_idx]; i++){
				print_dim(os, dim_idx + 1,data_idx + i * (block_size), block_size);
				os << ", \n";
			}
			os << "]\n";
			return;
		}
		os << "[";
		for(int i = 0; i < dimensions[dim_idx]; i++){
			os << data[data_idx + i] << ", ";
		}
		os << "]";
	}

	
	friend ostream& operator<<(ostream& os, struct tensor& t);
} Tensor;

ostream& operator<<(ostream& os, struct tensor& t){
	os << "dimensions: (";
	for(auto dim : t.dimensions){
		os << dim << ", ";
	}
	os << ")\n";

	os << "data: ";
	t.print_dim(os, 0, 0, t.data.size());
	return os;
}
int main(){
	Tensor t;
	t.dimensions = {2, 3};
	t.data = {1, 2, 3, 4, 5, 6};
	Tensor t2 = t[{2}][{1}];
	cout << t2 << endl;
	return 0;
}
