int	BDD_class::next_bit_layer_single(int depth,int which_node_this_layer){
		
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	bool	should_not_be_unfold[parameter_input_bit_width] = {0};
	for (zz=0;zz<parameter_input_bit_width;zz++){
		amount_turn[zz] = 0;
	}
	long	parameter_sample_mul = 10;
	for (j=0;j<20;j++){
		parameter_sample_mul *= 10;
		if((parameter_sample_mul * (parameter_output_bit_width*1-total_finish_weight/pow(2.0,20))) > 10000){
			break;
		}
	}
	int	BSD_samples_influence = 1;
	int	which_node_this_layer_sample = 0;
	int	which_node_this_layer_array[BSD_samples_influence];
	for (zz=0;zz<10;zz++){
		if(depth < parameter_multi_output_index){
			break;
		}
		

		for (i=0;i<BSD_samples_influence;i++){
			for (j=0;j<parameter_input_bit_width;j++){
				int zi = 0;
				long randint;
				zi = j%30;
				if(zi == 0){
					randint = rand();
				}
				if(depth == 0){
					mask_input_data[i][j] = bool((randint >> (zi))%2);
				}
				else{
					if(has_been_unfold[j]){
						mask_input_data[i][j] = BDD_mask_this[which_node_this_layer].mask[j];
						//mask_input_data[i][j] = rand()%2;		
					}else{	
						mask_input_data[i][j] = bool((randint >> (zi))%2);		
					}
					
				}
			}	
			which_node_this_layer_array[i] = which_node_this_layer;
		}
		
		for (i=0;i<parameter_input_bit_width+1;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				
			}else{
				for(int j=0;j<BSD_samples_influence;j++){
					bool amount_turn_input_data_ij[parameter_input_bit_width];
					for(int k=0;k<parameter_input_bit_width;k++){
							amount_turn_input_data_ij[k] = mask_input_data[j][k];
					}
					amount_turn_input_data_ij[i] = !amount_turn_input_data_ij[i];
					bool amount_turn_output_data_ij = io_generator_single(amount_turn_input_data_ij,BDD[depth][which_node_this_layer_array[j]].which_bit_output);
					amount_turn_output_data[i][j] = amount_turn_output_data_ij;
				}
				
			}
		}
		bool	finish_influence_sample = 0;
		for (i=0;i<parameter_input_bit_width;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				amount_turn[i] = -1;	
			}else{
				for(j=0;j<BSD_samples_influence;j++){
					if(amount_turn_output_data[i][j] && !amount_turn_output_data[parameter_input_bit_width][j]){
						amount_turn[i] += 1;
					}
					else if(!amount_turn_output_data[i][j] && amount_turn_output_data[parameter_input_bit_width][j]){
						amount_turn[i] += 1;
					}
					//if(amount_turn_output_data[i][j]){
					//	amount_turn[i] += 1;
					//}

					///	amount_turn[i] += train_dataset_bit_influence[i];
				}
					
				if((amount_turn[i] > 10)&&(amount_turn[i]<(zz*BSD_samples_influence/2.0))){
					finish_influence_sample = 1;
				}
				if(((zz*BSD_samples_influence-amount_turn[i]) > 10)&&(amount_turn[i]>(zz*BSD_samples_influence/2.0))){
					finish_influence_sample = 1;
				}
								
			}
		}
		
		if(finish_influence_sample){
			break;
		}
	}
	//if(zz>10000-1){
	//	parameter_early_stop_depth = depth+1;
	//}
	most_influence_next = max_element(amount_turn,amount_turn+parameter_input_bit_width) - amount_turn;	
	for (i=0;i<parameter_input_bit_width;i++){
		if(has_been_unfold[most_influence_next]){
			amount_turn[most_influence_next] = -1;
		}else{
			break;	//has_been_unfold[most_influence_next] = 1;
		}
		most_influence_next = max_element(amount_turn,amount_turn+parameter_input_bit_width) - amount_turn;
			
	}
	
	return	most_influence_next;
};


int	BDD_class::next_bit_layer(int depth){
	cout<<depth<<" "<<BSD_variable_order_number<<endl;
	if(depth < BSD_variable_order_number){
		most_influence[depth] = BSD_variable_order[depth];
		has_been_unfold[most_influence[depth]]=1;
		cout<<most_influence[depth]<<endl;
		return  BSD_variable_order[depth];
	}
	else
		return next_bit_layer_0(depth);
}

int	BDD_class::next_bit_layer_1(int depth){
	random_device rd;	
	mt19937 gen(rd());
		//int most_influence_all[64] ={0,32,1,33,2,34,3,35,4,36,5,37,6,38,7,39,8,40,9,41,10,42,11,43,12,44,13,45,14,46,15,47,16,48,17,49,18,50,19,51,20,52,21,53,22,54,23,55,24,56,25,57,26,58,27,59,28,60,29,61,30,62,31,63};
		//int most_influence_next = 	most_influence_all[depth];
		//int most_influence_next = 	;
		//cin>>most_influence_next;
		//return most_influence_next;
		int	most_influence_next=999999;
	int	most_influence_single=0;
	if(depth < 2000){
		most_influence_next = 	next_bit_layer_0(depth);
	}
	else if(depth==parameter_input_bit_width-1){
		most_influence_next = 	next_bit_layer_0(depth);
		return most_influence_next;
	}
		//return most_influence_next;
	if(most_influence_next != 999999){
		return most_influence_next;
	}
	else{
		cout<<"BSD_samples"<<BSD_samples<<endl;
		bool	this_bit_most_influence[parameter_input_bit_width] = {0};
		for(int i=0;i<100;i++){
			int which_node_this_layer;
			if(BDD_width_each_layer[depth] < 100){
				which_node_this_layer = i;
				if(i==BDD_width_each_layer[depth]){
					break;
				}
			}
			else{
				which_node_this_layer = rand()%BDD_width_each_layer[depth];
			}
			most_influence_single = 	next_bit_layer_single(depth,which_node_this_layer);
			this_bit_most_influence[most_influence_single] = 1;
		}
		int 	i = depth;
		int 	j,zi,zj;
		int*	how_many_nodes_if_expansion = new int [parameter_input_bit_width];
		bool*	left_son_mask = new bool [parameter_input_bit_width];
		bool*	right_son_mask = new bool [parameter_input_bit_width];
		int change_nodes[parameter_max_BDD_width];	
		for(j=0;j<parameter_input_bit_width;j++)
			how_many_nodes_if_expansion[j] = -1;
		for(int this_bit_expansion=0;this_bit_expansion<parameter_input_bit_width;this_bit_expansion++){
			bool	left_son_neg;
			bool	right_son_neg;

			int	left_mask_output_data_sum;
			int	right_mask_output_data_sum;
			int	left_mask_output_data_sum_half;
			int	right_mask_output_data_sum_half;
			
			int	which_list_number_l; 	
			int	which_list_number_neg_l;
			int	which_list_number_r; 	
			int	which_list_number_neg_r;

			change_nodes[this_bit_expansion]=0;
			set_random_input_data(mask_input_data);
			if(this_bit_most_influence[this_bit_expansion]){
				how_many_nodes_if_expansion[this_bit_expansion] = 0;
				most_influence[i] = this_bit_expansion;
				#pragma omp parallel for 
				for(long zj=0;zj<BDD_width_each_layer[i]*BSD_samples;zj++){
					int j  = int(zj/BSD_samples);
					int zi = int(zj%BSD_samples);
					bool mask_input_data_left [parameter_input_bit_width]; 
					bool mask_input_data_right[parameter_input_bit_width]; 
					for (int jj=0;jj<parameter_input_bit_width;jj++){
						mask_input_data_left [jj] = mask_input_data[zi][jj];
						mask_input_data_right[jj] = mask_input_data[zi][jj];
					}
					for (int jj=0;jj<i;jj++){
						mask_input_data_left [most_influence[jj]] = BDD_mask_this[j].mask[most_influence[jj]];
						mask_input_data_right[most_influence[jj]] = BDD_mask_this[j].mask[most_influence[jj]];
					}
						mask_input_data_left [most_influence[i]] = 0;
						mask_input_data_right[most_influence[i]] = 1;
					bool left_mask_output_data_b  = io_generator_single(mask_input_data_left ,BDD[i][j].which_bit_output);
					bool right_mask_output_data_b = io_generator_single(mask_input_data_right,BDD[i][j].which_bit_output);
					right_mask_output_data_all[zj] = right_mask_output_data_b;
					left_mask_output_data_all [zj] =  left_mask_output_data_b;
			 	}
				for(j=0;j<BDD_width_each_layer[i];j++){
					left_son_neg 	= 0;
					right_son_neg 	= 0;
					for(zi=0;zi<i;zi++){
						left_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
						right_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
					}
					left_son_mask[most_influence[i]] 	= 0; 
					right_son_mask[most_influence[i]] 	= 1; 
					
					bool	all_zero_left  = 1;
					bool	all_one_left   = 1;
					bool	all_one_right  = 1;
					bool	all_zero_right = 1;
				
					int	first_one_left 		= -1;
					int	first_zero_left 	= -1;
					int	first_one_right 	= -1;
					int	first_zero_right 	= -1;
					for(int zi=0;zi<BSD_samples;zi++){
						left_mask_output_data[zi]  =  left_mask_output_data_all[zi+BSD_samples*j];
						right_mask_output_data[zi] = right_mask_output_data_all[zi+BSD_samples*j];
					}
					for(zi=0;zi<BSD_samples;zi++){
						if(left_mask_output_data[zi]){
							all_zero_left 	= 0;
							first_one_left	= zi;
						}else{
							all_one_left 	= 0;
							first_zero_left = zi;
						}
						if((!all_zero_left)&&(!all_one_left)){
							break;
						}
					}
					for(zi=0;zi<BSD_samples;zi++){
						if(right_mask_output_data[zi]){
							all_zero_right 		= 0;
							first_one_right		= zi;
						}else{
							all_one_right 		= 0;
							first_zero_right	= zi;
						}
						if((!all_zero_right)&&(!all_one_right)){
							break;
						}
					}
						if(all_zero_left){
							change_nodes[this_bit_expansion]+=1;
						}else if(all_one_left){
							change_nodes[this_bit_expansion]+=1;
						}else{
							which_list_number_l 	= compare_simplify_list    (how_many_nodes_if_expansion[this_bit_expansion],left_mask_output_data,simplify_list);
							which_list_number_neg_l 	= compare_simplify_list_neg(how_many_nodes_if_expansion[this_bit_expansion],left_mask_output_data,simplify_list);
							bool	it_cannot_simplify 	= (which_list_number_l < 0)&&(which_list_number_neg_l<0);
							bool	it_can_simplify 	= (which_list_number_l 		>=0);
							bool	it_can_simplify_neg 	= (which_list_number_neg_l	>=0);
							
							if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
								it_cannot_simplify = 1;
							}
							if(it_cannot_simplify){
								how_many_nodes_if_expansion[this_bit_expansion] += 1;
								for (zi=0;zi<BSD_samples;zi++){
									simplify_list[how_many_nodes_if_expansion[this_bit_expansion]-1][zi] = left_mask_output_data[zi];
								}
							}
						}
						if(all_zero_right){
							change_nodes[this_bit_expansion]+=1;
						}else if(all_one_right){
							change_nodes[this_bit_expansion]+=1;
						}else{
							which_list_number_r 	= compare_simplify_list(how_many_nodes_if_expansion[this_bit_expansion],right_mask_output_data,simplify_list);
							which_list_number_neg_r 	= compare_simplify_list_neg(how_many_nodes_if_expansion[this_bit_expansion],right_mask_output_data,simplify_list);
							bool	it_cannot_simplify 	= (which_list_number_r < 0)&&(which_list_number_neg_r<0);
							bool	it_can_simplify 	= (which_list_number_r 		>=0);
							bool	it_can_simplify_neg 	= (which_list_number_neg_r	>=0);
							
							if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
								it_cannot_simplify = 1;
							}
							if(it_cannot_simplify){
								how_many_nodes_if_expansion[this_bit_expansion] += 1;
								for (zi=0;zi<BSD_samples;zi++){
									simplify_list[how_many_nodes_if_expansion[this_bit_expansion]-1][zi] = right_mask_output_data[zi];
								}
								change_nodes[this_bit_expansion]+=1;

							}else{
								if((which_list_number_l<0)&&(which_list_number_neg_l<0)&&(which_list_number_r==how_many_nodes_if_expansion[this_bit_expansion]-1));
								else
									change_nodes[this_bit_expansion]+=1;
							}
						}
				}
			}
		}
		
		int best_expansion_node = 0;
		int best_expansion_node_width = 10000000;
		for(zi=0;zi<parameter_input_bit_width;zi++){
			if(has_been_unfold[zi]){
				how_many_nodes_if_expansion[zi] = -2;
			}
			if((how_many_nodes_if_expansion[zi]>=0)&&(change_nodes[zi]>0)){
				if(how_many_nodes_if_expansion[zi]<best_expansion_node_width){
					best_expansion_node = zi;
					best_expansion_node_width = how_many_nodes_if_expansion[zi];
				}
			}
		}
		cout<<"模拟展开节点个数	"<<how_many_nodes_if_expansion[best_expansion_node]<<endl;
		cout<<"模拟展开修改	"<<change_nodes               [best_expansion_node]<<endl;
		most_influence_next = best_expansion_node; 
		has_been_unfold[most_influence_next] = 1;
		delete []   how_many_nodes_if_expansion ;
		delete []	left_son_mask ;
		delete []	right_son_mask ;
	}
	return	most_influence_next;
};

int	BDD_class::next_bit_layer_0(int depth){
		
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	bool	should_not_be_unfold[parameter_input_bit_width] = {0};
	random_device rd;	
	mt19937 gen(rd());

	for (zz=0;zz<parameter_input_bit_width;zz++){
		amount_turn[zz] = 0;
	}
	long	parameter_sample_mul = 10;
			for (j=0;j<20;j++){
				parameter_sample_mul *= 10;
				if((parameter_sample_mul * (1-total_finish_weight/pow(2.0,20))) > 10000){
					break;
				}
			}
	int	BSD_samples_influence = BSD_samples_influence_max;
	for (zz=0;zz<100;zz++){
		if(depth < parameter_multi_output_index){
			break;
		}
		
		int	which_node_this_layer = 0;
		int	which_node_this_layer_sample = 0;
		int	which_node_this_layer_array[BSD_samples_influence];
		//#pragma omp parallel for
		for (int i=0;i<BSD_samples_influence;i++){
			int which_node_this_layer = gen()%BDD_width_each_layer[depth];
			for (j=0;j<parameter_input_bit_width;j++){
				int zi = 0;
				long randint;
				zi = j%30;
				if(zi == 0){
					randint = gen();
				}
				if(depth == 0){
					mask_input_data[i][j] = bool((randint >> (zi))%2);
				}
				else{
					if(has_been_unfold[j]){
						mask_input_data[i][j] = BDD_mask_this[which_node_this_layer].mask[j];
					}else{	
						mask_input_data[i][j] = bool((randint >> (zi))%2);		
					}
					
				}
			}	
			which_node_this_layer_array[i] = which_node_this_layer;
		}
		
		for (i=0;i<parameter_input_bit_width+1;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				
			}else{
				#pragma omp parallel for
				for(int j=0;j<BSD_samples_influence;j++){
					bool amount_turn_input_data_ij[parameter_input_bit_width];
					for(int k=0;k<parameter_input_bit_width;k++){
							amount_turn_input_data_ij[k] = mask_input_data[j][k];
					}
					amount_turn_input_data_ij[i] = !amount_turn_input_data_ij[i];
					bool amount_turn_output_data_ij = io_generator_single(amount_turn_input_data_ij,BDD[depth][which_node_this_layer_array[j]].which_bit_output);
					amount_turn_output_data[i][j] = amount_turn_output_data_ij;
				}
			}
		}
		bool	finish_influence_sample = 0;
		for (i=0;i<parameter_input_bit_width;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				amount_turn[i] = -1;	
			}else{
				for(j=0;j<BSD_samples_influence;j++){
					if(amount_turn_output_data[i][j] && !amount_turn_output_data[parameter_input_bit_width][j]){
						amount_turn[i] += 1;
					}
					else if(!amount_turn_output_data[i][j] && amount_turn_output_data[parameter_input_bit_width][j]){
						amount_turn[i] += 1;
					}
				}
				//USE_THIS:RANDOM
				//if(i<64)
				//	amount_turn[i] = gen()%100;
				//else 
				//	amount_turn[i] = 0;
					
				if((amount_turn[i] > 40) && ((zz*BSD_samples_influence-amount_turn[i])>40)){
					finish_influence_sample = 1;
				}
				
			}
		}
		
		if(finish_influence_sample){
			cout<<"这一层排序的采样个数为："<<zz*BSD_samples_influence<<endl;
			break;
		}
	}
	double amount_turn_all = 0;
	double amount_turn_valid_bits = 0;
	int	amount_turn_static[parameter_input_bit_width];
	for (i=0;i<parameter_input_bit_width;i++){
		if(amount_turn[i]>((zz*BSD_samples_influence)/2.0)){
			amount_turn_static[i] = amount_turn[i];
		}else{
			amount_turn_static[i] = amount_turn[i];
		}
		if(amount_turn_static[i]>=1){
			amount_turn_all = amount_turn_all + max(1,amount_turn_static[i]);
			amount_turn_valid_bits = amount_turn_valid_bits + 1;
		}
	}
	double amount_turn_average = (amount_turn_all+1)/amount_turn_valid_bits;
	most_influence_next = max_element(amount_turn_static,amount_turn_static+parameter_input_bit_width) - amount_turn_static;	
	for (i=0;i<parameter_input_bit_width;i++){
		if(has_been_unfold[most_influence_next]){
			amount_turn[most_influence_next] = -1;
		}else{
			break;	//has_been_unfold[most_influence_next] = 1;
		}
		most_influence_next = max_element(amount_turn_static,amount_turn_static+parameter_input_bit_width) - amount_turn_static;
	}
	has_been_unfold[most_influence_next] = 1;
	double amount_turn_max_divide_average = amount_turn_static[most_influence_next]/amount_turn_average;
	double amount_turn_max_ratio = amount_turn[most_influence_next]/(zz+1);
	//USE_THIS:BEST
	//if((depth<parameter_input_bit_width-2) ){
	//	if((amount_turn_max_divide_average < 4) || (amount_turn_max_ratio < 0.3) || ((amount_turn_max_ratio>0.8)&&((amount_turn_max_ratio<0.999)))){
	//		has_been_unfold[most_influence_next] = 0;
	//		most_influence_next = 999999;
	//	}
	//}	
	//delete[]amount_turn;
	return	most_influence_next;
};
