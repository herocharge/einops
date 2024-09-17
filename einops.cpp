#include <bits/stdc++.h>

using namespace std;

typedef struct index {
	int left = -1;
	int right = -1; // non-inclusive
	int step = 1;

	bool isAll(){
		return left == -1 && right == -1;
	}

	bool isPref(){
		return left == -1;
	}

	bool isSuff(){
		return right == -1;
	}

	int len(int max_len){
		if(isAll()) return max_len;
		if(isPref()) return right;
		if(isSuff()) return max_len - left + 1;

		return right - left;
	}
} Index;

typedef struct tensor {
	vector<int> dimensions;
	vector<int> data;

	vector<int> toNdIdx(int idx){
		vector<int> idxs;
		/// why reverse?
		for(int i = dimensions.size() - 1; i>=0; i--){
			idxs.push_back(idx%dimensions[i]);
			idx /= dimensions[i];
		}

		reverse(idxs.begin(), idxs.end());
		return idxs;
	}

	/// Need to rewrite this, so that we can do stuff like a[0,:,1]
	/// where we take all of an intermediate dim
	//
	// Lets loop over each element and see if it fits the indexing

	struct tensor operator[](vector<Index> idxs){
		assert(idxs.size() <= dimensions.size());
		struct tensor result;
		// what would be the output dim?
		for(int i = 0; i < dimensions.size(); i++){
			if(i < idxs.size()){
				result.dimensions.push_back(idxs[i].len(dimensions[i]));
			}
			else
				result.dimensions.push_back(dimensions[i]);
		}

		for(int i = 0; i < data.size(); i++){
			/// Check for each data[i] if it belongs to the set of idxs
			/// 1 2 3 4 5 6 7 8 , 2 x 4
			/// _______ _______
			/// _ _ _ _ _ _ _ _
			//
			// 1 2 3 4 5 6
			// _____ _____
			// _ _ _ _ _ _
			
			bool ok = true;
			int curr_size = data.size();
			int left = 0, right = data.size() - 1;
			vector<int> p_idxs = toNdIdx(i);
		//	for(auto x : p_idxs)cout<<x<<" ";cout<<endl;
			for(int j = 0; j < idxs.size(); j++){
				if(idxs[j].isAll())continue;
				if(idxs[j].isPref()){
					ok &= (idxs[j].right > p_idxs[j]);
					continue;
				}
				if(idxs[j].isSuff()){
					ok &= (idxs[j].left <= p_idxs[j]);
					continue;
				}
				ok &= (idxs[j].left <= p_idxs[j] && p_idxs[j] < idxs[j].right);
			}
			if(ok)result.data.push_back(data[i]);
	//		if(ok) cout<<i<<endl;
		}
	//	cout<<"data_sie:"<<result.data.size()<<endl;
		
		return result;
	}

	/// Gets rid of dimensions with only one value
	struct tensor shrink(){
		struct tensor new_tensor;
		for(auto dim : dimensions){
			if(dim != 1)
				new_tensor.dimensions.push_back(dim);
		}
		if(new_tensor.dimensions.size() == 0)
			new_tensor.dimensions.push_back(1);
		new_tensor.data = data;
		return new_tensor;
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
	cout << t << endl;
	Index i1 = {-1 , -1, 1};
	Index i2 = {2 , 3, 1};
	Tensor t2 = t[{i1, i2}];
	cout << t2 << endl;


	Tensor t3;
	t3.dimensions = {10, 10};
	for(int i = 0; i < 100; i++)t3.data.push_back(i);

	cout << t3 << endl;
	Index i3 = {2, 7, 1};
	Tensor t4 = t3[{i3, i3}];
	
	cout<< t4 <<endl;
	return 0;
}
