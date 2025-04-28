int	BDD_class::set_random_input_data(bool** mask_input_data){
	int i,j;
	random_device rd;	
	mt19937 gen(rd());
	//#pragma omp parallel for
	for (int zj=0;zj<BSD_samples*parameter_input_bit_width;zj++){
		int i = int(zj/parameter_input_bit_width);
		int j = int(zj%parameter_input_bit_width);
			//srand((int)time(0));
			int zi = 0;
			long randint;
			zi = zj%30;
			if(zi == 0){
				//randint = rand();
				randint = gen();
			}
			mask_input_data[i][j] = bool((randint >> (zi))%2);
	}
	return 0;
};
int	BDD_class::mask_random_input_data(int depth,bool mask[parameter_input_bit_width],int amount,int* most_influence,bool** mask_input_data){
	int i,j;
	//#pragma omp parallel for
	for (i=0;i<amount;i++){
		for (j=0;j<depth+1;j++){
			mask_input_data[i][most_influence[j]] = mask[most_influence[j]];
		}
	}

	return 0;
};


int BDD_class::compare_simplify_list(int list_line_amount,bool* this_line,bool** simplify_list){
	int which_list_number = -1;
	bool it_can_simplify  = 0;
	int i,j;
	bool it_can_simplify_here;
	which_list_number = -1;
	if(1){	
		for (int i=list_line_amount-1;i>=0;i--){
			it_can_simplify_here = 1;
			for(int j=0;j<BSD_samples;j++){
				if(simplify_list[i][j] != this_line[j]){
					it_can_simplify_here = 0;
					break;
				}
			}
			if(it_can_simplify_here){
				it_can_simplify   = 1;
				which_list_number = i;
				break;
			}
		}
	}
	return which_list_number;
};
int BDD_class::compare_simplify_list_neg(int list_line_amount,bool* this_line,bool** simplify_list){
	int which_list_number = -1;
	//return which_list_number;
	bool it_can_simplify  = 0;
	int i,j;
	bool it_can_simplify_here;
	which_list_number = -1;
	if(1){	
		for (i=list_line_amount-1;i>=0;i--){
			it_can_simplify_here = 1;
			for(int j=0;j<BSD_samples;j++){
				if(simplify_list[i][j] == this_line[j]){
					it_can_simplify_here = 0;
					break;
				}
			}
			if(it_can_simplify_here){
				it_can_simplify   = 1;
				which_list_number = i;
				break;
			}
		}
	}
	return which_list_number;
};


