class	BDD_node{
public:
	int	left_node_index		=	0;
	int	right_node_index	=	0;
	int	depth			=	0;
	double	weight			=	pow(2.0,20);
	bool	mask[parameter_input_bit_width] 	= 	{0};
	int	sort_in_layer		=	0;
	bool	left_node_neg		=	0;
	bool	right_node_neg		=	0;
	int	non_equal_number	=	0;
	bool	has_equal_father	=	0;
	bool	which_root_node_all[1001] = {0};
	int	which_root_node		=	0;
	int	which_bit_output	=	0;

	int	this_layer_bit_expansion= 0;
	bool	switch_to_another_BDD	= 0;
	int	switch_to_which_BDD	= 0;
	int	switch_to_which_node	= 0;

	BDD_node(){
	};
};
class	BDD_node_mask{
public:	
	bool	mask[parameter_input_bit_width] = {0};
	BDD_node_mask(){
	}
};
class BDD_class{
public:
	//BSD内部使用的变量（可变）
	int			BSD_samples 		= min(1000,parameter_max_samples);
	int			BSD_samples_train 	= min(40000,parameter_max_samples);
	int			BSD_samples_sort 	= min(400,parameter_max_samples);
	const int	how_often_simplify 	= 1;
	//BSD内部使用的变量（固定）
	const double	total_weight_max		= pow(2.0,20);	//指导权重分布修改
	const int	total_sample_max 	= 1000000000;	//指导采样个数修改
	const int	BSD_samples_influence_max 	= 10*parameter_num_threads;		//BSD确定展开序的置信度
	int	which_demo_function ;

	int	BDD_id;
	bool	this_is_BDD_temp = 0;	
	struct timeval	initial_start_time;
	int	which_BDD = 0;
	int	total_compare_times=0;
	const	int	cal_data_width = min(parameter_input_bit_width/2,32);

	int i,j,zi,zj,ii,jj;

	int	*	most_influence		;
	int	*	BDD_width_each_layer 	;
	bool	**	mask_input_data  	;
	bool	*	mask_output_data 	;
	
	double		total_finish_weight = 0;
	long		total_non_equal_nodes = 0;

	BDD_node ** 	BDD 		;
	BDD_node_mask*	BDD_mask_this	;
	BDD_node_mask*	BDD_mask_next	;
	long		io_read_times;


	bool*	has_been_unfold 	;	
	//bool***	amount_turn_input_data	;
	bool**	amount_turn_output_data	;
	double*	amount_turn 		;
	
	bool	**	simplify_list ;
	bool	cal_infer_result(bool input_data[parameter_input_bit_width],int which_root_node,bool neg);
	bool*	io_generator(bool input_data[parameter_input_bit_width],bool* output_data);
	bool	io_generator_single(bool input_data[parameter_input_bit_width],int which_bit_output);
	int	set_random_input_data(bool** mask_input_data);
	int	mask_random_input_data(int depth,bool mask[parameter_input_bit_width],int amount,int* most_influence,bool** mask_input_data);
	int	next_bit_layer(int depth);
	int	next_bit_layer_0(int depth);
	int	next_bit_layer_1(int depth);
	int	next_bit_layer_old(int depth);
	int	next_bit_layer_single(int depth,int which_node_this_layer);

	int 	compare_simplify_list(int list_line_amount,bool* this_line,bool** simplify_list);
	int 	compare_simplify_list_neg(int list_line_amount,bool* this_line,bool** simplify_list);
	
	long	total_nodes_amount = 0;
	long	total_split_nodes  = 0;
	long	total_nodes_amount_recursive = 0;
	long	total_split_nodes_recursive  = 0;
	int	total_BDD_depth	   = 0;
	double	train_time	   = 0;	
	struct timeval	start_time,finish_time;

	int	start_depth = 0;	
	int	how_many_start_nodes = 32;
	BDD_node*	start_nodes;
	int 	train_BDD(BDD_node** BDD,int* most_influence,int start_depth,int how_many_start_nodes,BDD_node* start_nodes);

	double	circuit_accuracy;
	double*	circuit_accuracy_all_bits;
	int	error_amount = 0;
	int	error_amount_all = 0;
	int	train_error  = 0;
	int	incre_wrong_dataset_size = 0;
		
	bool*	test_input_data;
	bool	test_output_data;
	bool	infer_output_data;
	

	int	BDD_infer(BDD_node** BDD,int* most_influence);
	int 	print_circuit(BDD_node** BDD,int* most_influence);
	int	BDD_FULL_PROCESS();
	
	bool*	left_son_mask		;
	bool*	right_son_mask		;
	bool* 	left_mask_output_data	;
	bool* 	right_mask_output_data	;
	
	bool* 	left_mask_output_data_all	;
	bool* 	right_mask_output_data_all	;

	int		partition_depth = 1000;
	int		partition_into_how_many_parts  =2;
	int*		partition_start_node_numbers;
	int**		partition_index;
	BDD_class*	BDD_partition;


	//new functions
	int	BSD_samples_train_each_layer();
	int	BSD_samples_sort_each_layer();

	int	BSD_variable_order[parameter_input_bit_width];
	int	BSD_variable_order_number;


	int	split_nodes_each_layer[parameter_input_bit_width+1];
	int	accuracy_each_layer[parameter_input_bit_width+1];

	BDD_class(){
		most_influence			= new int [parameter_input_bit_width];
		BDD_width_each_layer 		= new int [parameter_input_bit_width+1];
		mask_input_data  		= new bool*[parameter_max_samples*2];
		mask_output_data 		= new bool [parameter_max_samples*2];
		simplify_list     		= new bool*[parameter_max_BDD_width];
		BDD 				= new BDD_node*[1+parameter_input_bit_width];
		BDD_mask_this			= new BDD_node_mask[parameter_max_BDD_width];
		BDD_mask_next			= new BDD_node_mask[parameter_max_BDD_width];
		has_been_unfold 		= new bool[parameter_input_bit_width+1];
		amount_turn_output_data		= new bool* [parameter_input_bit_width+1];
		amount_turn 			= new double [parameter_input_bit_width];
		test_input_data 		= new bool[parameter_input_bit_width];
		left_son_mask			= new bool [parameter_input_bit_width];
		right_son_mask			= new bool [parameter_input_bit_width];
		left_mask_output_data		= new bool [parameter_max_samples];
		right_mask_output_data		= new bool [parameter_max_samples];
		left_mask_output_data_all	= new bool [long(parameter_max_BDD_width)*long(parameter_max_samples)];
		right_mask_output_data_all	= new bool [long(parameter_max_BDD_width)*long(parameter_max_samples)];


	};
	
	

};
int	BDD_class::BSD_samples_train_each_layer(){
		int	BSD_samples_train;
		if(total_finish_weight/(pow(2.0,20)*how_many_start_nodes) > 0.9999999999){
			BSD_samples_train = 10;
			if(total_finish_weight/(pow(2.0,20)*how_many_start_nodes) > 1){
				BSD_samples_train = 2;
			}
		}else{
			BSD_samples_train = max(4,min(int(total_sample_max * (1-total_finish_weight/(pow(2.0,20)*how_many_start_nodes))),parameter_max_samples));
		}
		return BSD_samples_train;
}
int	BDD_class::BSD_samples_sort_each_layer(){
		return BSD_samples_sort;
}




bool*	BDD_class::io_generator(bool input_data[parameter_input_bit_width],bool* output_bits){
	//bool*	output_bits = new bool [parameter_output_bit_width];
	output_bits = io_generator_function(input_data,which_demo_function,output_bits);
	//io_read_times += 1;
	return	output_bits;
};

bool	BDD_class::io_generator_single(bool input_data[parameter_input_bit_width],int which_bit_output){
	
	bool	output_bit;
	bool*	output_bits = new bool [64*5];
	output_bits 	= io_generator(input_data,output_bits);
	output_bit		= output_bits[which_bit_output] ;
	delete	[] output_bits;
	return	output_bit;
};

int BDD_class::train_BDD(BDD_node** BDD,int* most_influence,int start_depth, int how_many_start_nodes, BDD_node* start_nodes){

	int i,j;
	int zi;
	bool	left_son_neg;
	bool	right_son_neg;

	int	left_mask_output_data_sum;
	int	right_mask_output_data_sum;
	int	left_mask_output_data_sum_half;
	int	right_mask_output_data_sum_half;
		
	int	which_list_number; 	
	int	which_list_number_neg;

	bool	all_zero_left  ;
	bool	all_one_left   ;
	bool	all_one_right  ;
	bool	all_zero_right ;

	int	first_one_left 		;
	int	first_zero_left 	;
	int	first_one_right 	;
	int	first_zero_right 	;
	int	which_cluster;
	bool	it_cannot_simplify 	;
	bool	it_can_simplify 	;
	bool	it_can_simplify_neg 	;
	double  all_train_time;
	int	root_nodes_leafs[how_many_start_nodes];
	int	leaf_nodes_roots[how_many_start_nodes];

	for(i=0;i<start_depth+1;i++)	
		BDD[i] 		= new BDD_node[how_many_start_nodes];
	for (i=0;i<how_many_start_nodes;i++){
		BDD[start_depth][i].which_bit_output = start_nodes[i].which_bit_output;
		BDD[start_depth][i].which_root_node  = start_nodes[i].which_root_node;
		for(zi=0;zi<how_many_start_nodes;zi++){
			BDD[start_depth][i].which_root_node_all[zi] = 0;
		}		
		BDD[start_depth][i].which_root_node_all[i] = 1;
		for (zi=0;zi<parameter_input_bit_width;zi++){
			BDD[start_depth][i].mask[zi] 	= start_nodes[i].mask[zi];
			BDD_mask_this[i].mask[zi] 	= start_nodes[i].mask[zi];
		}
	}
	total_finish_weight = 0;

	BDD_width_each_layer[start_depth] = how_many_start_nodes;
	total_BDD_depth	   = 0;
	train_time	   = 0;
	
	
	for(i=start_depth;i</*parameter_early_stop_depth*/parameter_input_bit_width+1;i++){
		cout<<"BSD input bit sequence :	";
		for(int zi=0;zi<i;zi++){
			cout<<most_influence[zi]<<" ";
		}
		cout<<endl;
	
		if(i>start_depth){	
			for(j=0;j<BDD_width_each_layer[i];j++){
				for (zi=0;zi<parameter_input_bit_width;zi++){
					BDD[i][j].mask[zi] 		= BDD_mask_next[j].mask[zi];
					BDD_mask_this[j].mask[zi] 	= BDD_mask_next[j].mask[zi];
				}	
			}
		}
		total_nodes_amount_recursive 	= total_nodes_amount;
		total_split_nodes_recursive 	= total_split_nodes;
		bool this_layer_need_partition = (i==partition_depth)&&(BDD_width_each_layer[i]>100);
		if(this_layer_need_partition){
			cout<<"go for partition"<<endl;
			partition_into_how_many_parts	= max(2,int(BDD_width_each_layer[i]/100));
			BDD_partition 			= new BDD_class [partition_into_how_many_parts];
			partition_start_node_numbers 	= new int  [partition_into_how_many_parts];
			partition_index 		= new int* [partition_into_how_many_parts];
			
			for(zi=0;zi<partition_into_how_many_parts;zi++){
				partition_start_node_numbers[zi] = 0;
				for(int zj = 0; zj < BDD_width_each_layer[i];zj++){
					//if(BDD[i][zj].mask[most_influence[i-1]] == zi){
					if(zj%partition_into_how_many_parts==zi){
						partition_start_node_numbers[zi] += 1;;
					}
				}
			}
			cout<<"finish partition node numbers	"<<partition_start_node_numbers[0]<<"	"<<partition_start_node_numbers[1]<<endl;	
				for(int zi=0;zi<partition_into_how_many_parts;zi++){	
					partition_index[zi] = new int [partition_start_node_numbers[zi]];
					int counter = 0;
					for(int zj=0;zj<BDD_width_each_layer[i];zj++){
						//if(BDD[i][zk].mask[most_influence[i-1]] == zj){
						if(zj%partition_into_how_many_parts==zi){
							partition_index[zi][counter] = zj;
							counter += 1;
						}
					}
				}
			cout<<"finish partition index"<<endl;	
			for(zi=0;zi<partition_into_how_many_parts;zi++){
				BDD_partition[zi].start_depth = i;	
				BDD_partition[zi].how_many_start_nodes = partition_start_node_numbers[zi];
				BDD_partition[zi].which_demo_function 	= 3;
				BDD_partition[zi].start_nodes = new BDD_node [BDD_partition[zi].how_many_start_nodes];
				for(int zj=0;zj<parameter_input_bit_width+1;zj++){
					BDD_partition[zi].has_been_unfold[zj] 	= has_been_unfold[zj];
					BDD_partition[zi].most_influence[zj] 	= most_influence[zj];
				}
				for(int zj=0;zj<BDD_partition[zi].how_many_start_nodes;zj++){
					if(partition_index[zi][zj] < BDD_width_each_layer[i]){
						BDD_partition[zi].start_nodes[zj].which_bit_output 	= BDD[i][partition_index[zi][zj]].which_bit_output;
						BDD_partition[zi].start_nodes[zj].which_root_node  	= BDD[i][partition_index[zi][zj]].which_root_node;
						for (int zk=0;zk<parameter_input_bit_width;zk++){
							BDD_partition[zi].start_nodes[zj].mask[zk]	= BDD_mask_this[partition_index[zi][zj]].mask[zk];
							BDD_partition[zi].BDD_mask_this[zj].mask[zk] 	= BDD_mask_this[partition_index[zi][zj]].mask[zk];
						}
					}
				}
				for(int zj=0;zj<parameter_input_bit_width+1;zj++){
					BDD_partition[zi].BDD_width_each_layer[zj] = BDD_width_each_layer[zj];
				}
				//if(!this_is_BDD_temp){	
					BDD_partition[zi].partition_depth = 100000;
					
			}
				//	for (zj=0;zj<parameter_max_samples;zj++){
				//		delete [] mask_input_data[j];
				//	}
							
			
			for(j=0;j<BDD_width_each_layer[i];j++){
				BDD[i][j].switch_to_another_BDD = 1;
				for(zi=0;zi<partition_into_how_many_parts;zi++){
					for(int zj=0;zj<BDD_partition[zi].how_many_start_nodes;zj++){
						if(partition_index[zi][zj] == j){
							BDD[i][j].switch_to_which_BDD 	= zi;
							BDD[i][j].switch_to_which_node 	= zj;
						}
					}
				}
			}		
			total_BDD_depth = i;
			cout<<"BDD partition on layer "<<i<<endl;
			
					for (int zj=0;zj<parameter_input_bit_width+1;zj++){
						delete [] amount_turn_output_data[zj] ;
					}
					for (int zj=0;zj<parameter_max_BDD_width;zj++){
						delete [] simplify_list[zj];
					}
					for (int zj=0;zj<parameter_max_samples;zj++){
						delete [] mask_input_data[zj] ;
					}
					for(int zj=0;zj<partition_into_how_many_parts;zj++){
						delete [] partition_index[zj];
						
					}
					delete [] amount_turn_output_data	;
					delete [] amount_turn 			;
					delete [] left_mask_output_data		;
					delete [] right_mask_output_data	;
					delete [] left_mask_output_data_all	;
					delete [] right_mask_output_data_all	;
					delete []left_son_mask			;
					delete []right_son_mask			;
					delete [] BDD_mask_this			;
					delete [] BDD_mask_next			;
					delete [] simplify_list			;
					delete [] mask_input_data;

			for(zi=0;zi<partition_into_how_many_parts;zi++){
					BDD_partition[zi].BDD_FULL_PROCESS();
					total_nodes_amount_recursive 	+= BDD_partition[zi].total_nodes_amount_recursive;	
					total_split_nodes_recursive 	+= BDD_partition[zi].total_split_nodes_recursive;	
				//}
			}
			break;
		}	
		

		BSD_samples_train = BSD_samples_train_each_layer();
		BSD_samples_sort  = BSD_samples_sort_each_layer();
		
	
		if((BDD_width_each_layer[i]==0)){
			total_BDD_depth = i;
			cout<<"total BDD depth:	"<<total_BDD_depth<<endl;
			break;
		}
		
		total_nodes_amount += BDD_width_each_layer[i];

		BDD[i+1] 	= new BDD_node[BDD_width_each_layer[i]*2];
		
		for(zi=0;zi<BDD_width_each_layer[i]*2;zi++){
			BDD[i+1][zi].has_equal_father = 0;
			BDD[i+1][zi].non_equal_number = 0;
		}
		BSD_samples = BSD_samples_sort;
		most_influence[i] = next_bit_layer(i);
 
		BSD_samples = BSD_samples_train;
		cout<<BSD_samples<<endl;
		cout<<"The BSD is on layer: "<<i<<"		";
		cout<<"The input bit is: x"<<most_influence[i]<<endl;
		cout<<"BSD width at this layer: "<<BDD_width_each_layer[i]<<endl;
		set_random_input_data(mask_input_data);
		
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
		for(int j=0;j<BDD_width_each_layer[i];j++){
		
			BDD[i][j].this_layer_bit_expansion = most_influence[i];	
			
			if(BDD[i][j].has_equal_father){
				;
			}else{
				BDD[i][j].non_equal_number = total_non_equal_nodes;
				total_non_equal_nodes += 1;
			}

			for(zi=0;zi<i;zi++){
				left_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
				right_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
			}
			left_son_mask[most_influence[i]] 	= 0;
			right_son_mask[most_influence[i]] 	= 1;
			
			bool all_zero_left  = 1;
			bool all_one_left   = 1;
			bool all_one_right  = 1;
			bool all_zero_right = 1;
		
	
			for(int zi=0;zi<BSD_samples;zi++){
				left_mask_output_data[zi]  =  left_mask_output_data_all[zi+BSD_samples*j];
				right_mask_output_data[zi] = right_mask_output_data_all[zi+BSD_samples*j];
			}
			for(int zi=0;zi<BSD_samples;zi++){
				if(left_mask_output_data[zi]){
					all_zero_left 	= 0;
				}else{
					all_one_left 	= 0;
				}
				if((!all_zero_left)&&(!all_one_left)){
					break;
				}
			}
			for(int zi=0;zi<BSD_samples;zi++){
				if(right_mask_output_data[zi]){
					all_zero_right 		= 0;
				}else{
					all_one_right 		= 0;
				}
				if((!all_zero_right)&&(!all_one_right)){
					break;
				}
			}
			if((i == (parameter_early_stop_depth-1))&&(which_BDD==0) || ((i == (parameter_early_stop_depth-1))&&(which_BDD==1)) || (total_split_nodes > parameter_early_stop_split_nodes)){
				if(left_mask_output_data_sum > (BSD_samples/2)){
					all_one_left 	= 1;
					all_zero_left	= 0;
				}else{
					all_one_left 	= 0;
					all_zero_left	= 1;
				}
				if(right_mask_output_data_sum > (BSD_samples/2)){
					all_one_right 	= 1;
					all_zero_right	= 0;
				}else{
					all_one_right 	= 0;
					all_zero_right	= 1;
				}
			}
			if((BDD_width_each_layer[i] > parameter_max_BDD_width/2) ){
				if(left_mask_output_data_sum > (BSD_samples/2)){
					all_one_left 	= 1;
					all_zero_left	= 0;
				}else{
					all_one_left 	= 0;
					all_zero_left	= 1;
				}
				if(right_mask_output_data_sum > (BSD_samples/2)){
					all_one_right 	= 1;
					all_zero_right	= 0;
				}else{
					all_one_right 	= 0;
					all_zero_right	= 1;
				}
			}

			if(i>parameter_multi_output_index-1){
				if(all_zero_left){
					BDD[i][j].left_node_index = -2;
					total_finish_weight += BDD[i][j].weight/2;
				}else if(all_one_left){
					BDD[i][j].left_node_index = -1; 
					total_finish_weight += BDD[i][j].weight/2;
				}else{
					if(((i%how_often_simplify) == 0)){
						which_list_number 	= compare_simplify_list    (BDD_width_each_layer[i+1],left_mask_output_data,simplify_list);
						which_list_number_neg 	= compare_simplify_list_neg(BDD_width_each_layer[i+1],left_mask_output_data,simplify_list);
						it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
						it_can_simplify 	= (which_list_number 		>=0);
						it_can_simplify_neg 	= (which_list_number_neg	>=0);
						if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
							it_cannot_simplify = 1;
						}
						if(it_cannot_simplify){
							BDD_width_each_layer[i+1] += 1;
							BDD[i+1][BDD_width_each_layer[i+1]-1].depth  = i+1; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2;
							for(zi=0;zi<how_many_start_nodes;zi++){
								BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
							} 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
							for (zi=0;zi<i+1;zi++){
								BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
							}
							BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
							BDD[i][j].left_node_neg   = 	0;
							for (zi=0;zi<BSD_samples;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] = left_mask_output_data[zi];
							}
						}else if(it_can_simplify){
							BDD[i][j].left_node_index = 	which_list_number;
							BDD[i][j].left_node_neg   = 	0;
							BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
							for(zi=0;zi<how_many_start_nodes;zi++){
								BDD[i+1][which_list_number].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							} 
						}else if(it_can_simplify_neg){
							BDD[i][j].left_node_index = 	which_list_number_neg;
							BDD[i][j].left_node_neg   = 	1;
							BDD[i+1][which_list_number_neg].weight += BDD[i][j].weight/2;
							for(zi=0;zi<how_many_start_nodes;zi++){
								BDD[i+1][which_list_number_neg].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							}
						}
					}else{
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
						for(zi=0;zi<how_many_start_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].left_node_neg   = 	0;
						
					}
				}
			}else{			
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 	
						for(zi=0;zi<how_many_start_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){                                                              	
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].left_node_neg   = 	0;
						for (zi=0;zi<BSD_samples;zi++){
							simplify_list[BDD_width_each_layer[i+1]-1][zi] = left_mask_output_data[zi];
						}

			}
			if(i>parameter_multi_output_index-1){
				if(all_zero_right){
					BDD[i][j].right_node_index = -2; 
					total_finish_weight += BDD[i][j].weight/2;
				}else if(all_one_right){
					BDD[i][j].right_node_index = -1; 
					total_finish_weight += BDD[i][j].weight/2;
				}else{
					if(((i%how_often_simplify == 0))){
						which_list_number 	= compare_simplify_list(BDD_width_each_layer[i+1],right_mask_output_data,simplify_list);
						which_list_number_neg 	= compare_simplify_list_neg(BDD_width_each_layer[i+1],right_mask_output_data,simplify_list);
						it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
						it_can_simplify 	= (which_list_number 		>=0);
						it_can_simplify_neg 	= (which_list_number_neg	>=0);
						if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
							it_cannot_simplify = 1;
						}
						if(it_cannot_simplify){
							BDD_width_each_layer[i+1] += 1;
							BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
							for(zi=0;zi<how_many_start_nodes;zi++){
								BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
							} 
							for (zi=0;zi<i+1;zi++){
								BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
							}
							BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
							BDD[i][j].right_node_neg    = 	0;
							for (zi=0;zi<BSD_samples;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] = right_mask_output_data[zi];
							}
						}else if(it_can_simplify){
							BDD[i][j].right_node_index = 	which_list_number;
							BDD[i][j].right_node_neg   = 0;
							BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
							for(zi=0;zi<how_many_start_nodes;zi++){
								BDD[i+1][which_list_number].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							} 
						}else if(it_can_simplify_neg){
							BDD[i][j].right_node_index = 	which_list_number_neg;
							BDD[i][j].right_node_neg   = 1;
							BDD[i+1][which_list_number_neg].weight += BDD[i][j].weight/2;
							for(zi=0;zi<how_many_start_nodes;zi++){
								BDD[i+1][which_list_number_neg].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							} 
						}
					}else{
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
						for(zi=0;zi<how_many_start_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].right_node_neg    = 	0;
							for (zi=0;zi<BSD_samples;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] = right_mask_output_data[zi];
							}
					}
				}
			}else{		
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
						for(zi=0;zi<how_many_start_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].right_node_neg    = 	0;
							for (zi=0;zi<BSD_samples;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] = right_mask_output_data[zi];
							}

			}
			if(BDD[i][j].left_node_index != BDD[i][j].right_node_index){
				total_split_nodes += 1;	
			}else{
				if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 0) && (BDD[i][j].left_node_index >= 0)){
					if(BDD[i+1][BDD[i][j].left_node_index].has_equal_father == 0){
						BDD[i+1][BDD[i][j].left_node_index].has_equal_father = 1;
						BDD[i+1][BDD[i][j].left_node_index].non_equal_number = BDD[i][j].non_equal_number;
					}
				}
			}
		}
		for(zi=0;zi<how_many_start_nodes;zi++){
			root_nodes_leafs[zi] = 0;
			for(int zj=0;zj<BDD_width_each_layer[i+1];zj++){
				root_nodes_leafs[zi] += int(BDD[i+1][zj].which_root_node_all[zi]);
			}
			cout<<root_nodes_leafs[zi]<<" ";
		}
		cout<<endl;

		split_nodes_each_layer[i]	= total_split_nodes;	
		accuracy_each_layer[i]		= ((total_finish_weight)/pow(2.0,20))/double(parameter_output_bit_width);
	
		cout<<"BSD "<<BDD_id<<" nodes = "<<total_nodes_amount<<endl;
		cout<<"BSD "<<BDD_id<<" split nodes = "<<total_split_nodes;
		cout<<"		Finish trained weight =  "<<setprecision(12)<<(total_finish_weight)/pow(2.0,20)<<endl;
		gettimeofday(&finish_time,NULL);
		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
		cout<<"BSD  "<<BDD_id<<" train time = "<<train_time<<"s"<<endl;
		all_train_time = double(finish_time.tv_usec-initial_start_time.tv_usec+1000000*(finish_time.tv_sec-initial_start_time.tv_sec))/1000000;
		cout<<"BSD  "<<BDD_id<<" program time = "<<all_train_time<<"s"<<endl;
		cout<<"######################################################################"<<endl;
	}


	return 0;
};

int	BDD_class::BDD_infer(BDD_node** BDD,int* most_influence){
	long i,j;
	gettimeofday(&start_time,NULL);
	random_device rd;	
	mt19937 gen(rd());

	cout<<"test dataset size = "<<parameter_test_ios<<endl;
	error_amount_all = 0;
	bool	error_all [how_many_start_nodes*parameter_test_ios];
	//#pragma omp parallel for 
	for(long zj=0;zj<how_many_start_nodes*parameter_test_ios;zj++){
		int test_bit = int(zj/parameter_test_ios);
		bool test_input_data [parameter_input_bit_width];
		bool	test_output_data;
		bool	infer_output_data;
		for (int j=0;j<parameter_input_bit_width;j++){
			int zi = 0;
			long randint;
			zi = j%30;
			if(zi == 0){
				//randint = rand();
				randint = gen();
			}
			test_input_data[j] = bool((int(randint >> (zi)))%2);
		}
		for(int j=0;j<start_depth;j++){
			test_input_data[most_influence[j]] = BDD[start_depth][test_bit].mask[most_influence[j]];
		}
		test_output_data  	= io_generator_single(test_input_data,BDD[start_depth][test_bit].which_bit_output);
		infer_output_data 	= cal_infer_result(test_input_data,test_bit,0);
		error_all[zj] =	(test_output_data != infer_output_data);
	}
	circuit_accuracy_all_bits = new double [how_many_start_nodes];
	for(int test_bit=0;test_bit<how_many_start_nodes;test_bit++){
		error_amount = 0;
		for (int i=0;i<parameter_test_ios;i++){
			if(error_all[test_bit*parameter_test_ios+i]){
				error_amount = error_amount + 1;
			}
		}
		train_error  = 0;
		circuit_accuracy_all_bits[test_bit] = 1-((error_amount)/double(parameter_test_ios));
		cout<<"Testing output bit No. "<<setw(4)<<test_bit<<"	Error amount = "<<error_amount<<"	Accuracy = "<<1-(error_amount/double(parameter_test_ios))<<endl;
	}
	circuit_accuracy = 0;
	for(i=0;i<how_many_start_nodes;i++){
		circuit_accuracy +=	circuit_accuracy_all_bits[i];
	}
	delete [] circuit_accuracy_all_bits;
	circuit_accuracy /= how_many_start_nodes;
	cout<<"How many start nodes = "<<how_many_start_nodes<<endl;
	cout<<"Average Accuracy among all output bits = "<<circuit_accuracy<<endl;
	for (i=0;i<min(parameter_input_bit_width,100);i++){
		cout<<most_influence[i]<<" ";
	}
	cout<<endl;
	gettimeofday(&finish_time,NULL);
	double test_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
	cout<<"Total test time = "<<test_time<<"s"<<endl;
	return 0;
};

bool	BDD_class::cal_infer_result(bool input_data[parameter_input_bit_width],int which_root_node,bool neg){
	bool	infer_result = 0;
	int i;
	int position = which_root_node;
	for (i=this->start_depth;i<parameter_input_bit_width+10000;i++){
		if(this->BDD[i][position].switch_to_another_BDD == 0){
			if(!input_data[this->most_influence[i]]){
				if(this->BDD[i][position].left_node_index == -1){
					if(neg){
						infer_result = 0;
					}else{
						infer_result = 1;
					}
					break;
				}else if(this->BDD[i][position].left_node_index == -2){
					if(neg){
						infer_result = 1;
					}else{
						infer_result = 0;
					}
					break;
				}else{
					if(this->BDD[i][position].left_node_neg){
						neg = !neg;
					}
					position = this->BDD[i][position].left_node_index;
				}
			}else{
				if(this->BDD[i][position].right_node_index == -1){
					if(neg){
						infer_result = 0;
					}else{
						infer_result = 1;
					}
					break;
				}else if(this->BDD[i][position].right_node_index == -2){
					if(neg){
						infer_result = 1;
					}else{
						infer_result = 0;
					}
					break;
				}else{
					if(this->BDD[i][position].right_node_neg){
						neg = !neg;
					}
					position = this->BDD[i][position].right_node_index;
				}
			}
		}
		else{
			infer_result 	= BDD_partition[(this->BDD[i][position].switch_to_which_BDD)].cal_infer_result(input_data,this->BDD[i][position].switch_to_which_node,neg);
			break;
		}
	}
	return	infer_result;
};

int	BDD_class::BDD_FULL_PROCESS(){

	gettimeofday(&initial_start_time,NULL);
		gettimeofday(&start_time,NULL);
				BDD_id	= GLOBAL_BDD_id_number;
				GLOBAL_BDD_id_number += 1;
				cout<<"switch_BDD:	"<<BDD_id<<endl;
				cout<<"######################################################################"<<endl;

	for (i=0;i<parameter_input_bit_width+1;i++){
		amount_turn_output_data[i] = new bool  [BSD_samples_influence_max];
	}

	gettimeofday(&finish_time,NULL);
	train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;


	srand((unsigned)time(0));
	cout<<"ready to train"<<endl;
	
	for (j=0;j<parameter_max_samples;j++){
		mask_input_data[j] 	= new bool[parameter_input_bit_width*2];
	}
	for (int zi=0;zi<parameter_max_BDD_width;zi++){
			simplify_list[zi] = new bool[parameter_max_samples];
	}
	train_BDD(BDD,most_influence,start_depth,how_many_start_nodes,start_nodes);
	BDD_infer(BDD,most_influence);
	print_circuit(BDD,most_influence);
		delete [] test_input_data;
		
		//GLOBAL_BDD_nodes += total_nodes_amount; 
		//GLOBAL_BDD_split_nodes += total_split_nodes; 
		//cout<<"Total nodes = "<<total_nodes_amount<<endl;
		//cout<<"Total split nodes = "<<total_split_nodes<<endl;

	return 0;
}

