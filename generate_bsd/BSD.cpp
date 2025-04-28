#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<iostream>
#include<iomanip>
#include<algorithm>
#include<math.h>
#include<sys/time.h>
#include<functional>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<dirent.h>
#include<stdint.h>
#include<omp.h>
#include<unistd.h>
#include<random>
using namespace std;


//电路的parameter	Circuit_parameter
int			circuit_index	 		= 4444;		//电路编号
extern const int	parameter_input_bit_width 	= 12; 		//输入bit有几位
extern const int	parameter_output_bit_width	= 9;	 	//输出bit有几位
int			parameter_test_ios 		= 100000;	//测试要求多少样本

//算法的parameter 	Algorithm_parameter
extern const int 	parameter_num_threads		= 100;		//线程数
extern const int	parameter_multi_output_index	= 0; 		//BSD从第几层开始化简，前面若干层展开序确定
int	parameter_early_stop_depth	= parameter_input_bit_width;		//BSD到第几层终止，输出此时的不准确BSD
extern const int	parameter_max_samples 		= 10000;		//BSD每一个节点最多进行多少次采样
int	parameter_max_BDD_width		= 1000;	//BSD每一层最多多少个节点
int	parameter_early_stop_split_nodes= 10000;	//BSD每一层最多多少个节点

//全局变量
int	GLOBAL_which_demo_function;
int	GLOBAL_BDD_id_number; 
int	GLOBAL_BDD_nodes; 
int	GLOBAL_BDD_split_nodes; 
int	GLOBAL_train_time; 
int	GLOBAL_program_time; 

//待优化变量
int	variable_order [parameter_input_bit_width];
int	variable_order_number;

int	BSD_execute(int variable_order_number,int* variable_order);
bool	truth_table [1024*1024];

#include"cvt.h"
#include"io_generator_function.h"
#include"BSD.h"
#include"next_layer_bit.h"
#include"tool_function.h"
#include"print_circuit.h"





class BSD_features{
public:
	int	BSD_depth;
	int	BSD_area;
	double	accuracy;
	
	int	BSD_area_layers[parameter_input_bit_width];
	int	accuracy_layers[parameter_input_bit_width];
	//int	nodes_for_each_start_node[parameter_max_BDD_width];
};

	BSD_features BSD_features_0;
void	search_order();
int	search_reward(BSD_features BSD_features_0);

int	main(int argc,char* argv[]){

	omp_set_num_threads(parameter_num_threads);
	GLOBAL_BDD_id_number 	= 0;
	GLOBAL_BDD_nodes 	= 0;
	GLOBAL_BDD_split_nodes 	= 0;


	//io generator来自真值表，不来自写好的文件
 	//char*  truth_table_name = new char [100];
	//int    truth_table_input_width;
	//	if(argc >= 2){
	//		truth_table_input_width= atoi(argv[1]);
	//	}
	//	if(argc>=3){
	//		truth_table_name = argv[2];
	//	}
	//
	//        ifstream truth_table_file(truth_table_name);
	//	string line_data;
	//	for(int i=0;i<pow(2,20);i++){
	//			truth_table[i] = 0;
	//	}
	//	for(int i=0;i<pow(2,20);i++){
	//		getline(truth_table_file,line_data);
	//		//cout<<line_data[truth_table_input_width+1]<<endl;
	//		if(line_data[truth_table_input_width+1]=='0')
	//			truth_table[i] = 0;
	//		else
	//			truth_table[i] = 1;
	//	}

	
	search_order();

};

int BSD_execute(int variable_order_number,int* variable_order){
	
	BDD_class BDD_class_main;
	BDD_class_main.which_demo_function 	= GLOBAL_which_demo_function;
	BDD_class_main.BSD_variable_order_number = variable_order_number;
	for(int i=0;i<variable_order_number;i++){
		BDD_class_main.BSD_variable_order[i] = variable_order[i];
		cout<<BDD_class_main.BSD_variable_order[i]<<" ";
	}
	cout<<endl;


	BDD_class_main.start_depth 	= 0;	
	BDD_class_main.BDD_id 	= 0;
	BDD_class_main.how_many_start_nodes = parameter_output_bit_width;
	BDD_class_main.start_nodes = new BDD_node [BDD_class_main.how_many_start_nodes];
	for(int zi=0;zi<BDD_class_main.how_many_start_nodes;zi++){
		BDD_class_main.start_nodes[zi].which_bit_output 	= zi;
		BDD_class_main.start_nodes[zi].which_root_node  	= zi;
		BDD_class_main.start_nodes[zi].which_root_node_all[BDD_class_main.start_nodes[zi].which_root_node] = 1;
	}
	for(int zi=0;zi<parameter_input_bit_width+1;zi++){
		BDD_class_main.has_been_unfold[zi] = 0;
		BDD_class_main.most_influence[zi] = 0;
		BDD_class_main.BDD_width_each_layer[zi] = 0;
	}
	BDD_class_main.BDD_FULL_PROCESS();

	for(int zi=0;zi<parameter_input_bit_width;zi++){
		variable_order [zi] = BDD_class_main.most_influence[zi];
		BSD_features_0.BSD_area_layers [zi] =  BDD_class_main.split_nodes_each_layer[zi];
		BSD_features_0.accuracy_layers [zi] =  BDD_class_main.accuracy_each_layer[zi];
	}

	BSD_features_0.BSD_depth = BDD_class_main.total_BDD_depth+1;
	BSD_features_0.BSD_area  = BDD_class_main.total_split_nodes;
	BSD_features_0.accuracy  = BDD_class_main.circuit_accuracy;

	int reward = search_reward(BSD_features_0);

	return reward;	

};

int search_reward(BSD_features BSD_features_0){
	int	reward = (1000000*(1-BSD_features_0.accuracy));
	if(BSD_features_0.accuracy <1)
		reward += 1000*(parameter_input_bit_width-BSD_features_0.BSD_depth);
	reward += BSD_features_0.BSD_area;
	return reward;	

};

void search_order(){
        ofstream result_0("result_0_20bit",ios::app);
        ofstream result_10("result_10_20bit",ios::app);
        ofstream result_100("result_100_20bit",ios::app);
        ofstream result_1000("result_1000_20bit",ios::app);

	int	best_area=9999999; 
	int	best_area_depth = 0;
	int	best_BDD_split_nodes[parameter_input_bit_width];
	int	best_variable_order [parameter_input_bit_width];


	int area;
	int	mutation_depth;
	random_device rd;	
	mt19937 gen(rd());

	double	feature_variable[parameter_input_bit_width];

	int best_area_0;
	int best_area_10;
	int best_area_100;
	int best_area_1000;

	for (int i=0;i<10;i++){


		if(i==1){
			result_0 << best_area << endl;
			best_area_0 = best_area; 
		}
		else if(i==11){
			result_10 << best_area << endl;
			best_area_10 = best_area; 
			if(best_area_10==best_area_0){
				i=90;
			}
		}
		else if(i==101){
			result_100 << best_area << endl;
			best_area_100 = best_area; 
			if(best_area_100==best_area_10){
				i=990;
			}
		}
		else if(i==999){
			result_1000 << best_area << endl;
		}

		parameter_max_BDD_width=4000;
			mutation_depth =  int(best_area_depth);

		int	min_feature = 9999999;

			cout<<"Mutation Depth:"<<mutation_depth<<endl;
		for(int vi=0;vi<mutation_depth;vi++){
			variable_order[vi] = best_variable_order[vi];
			feature_variable[variable_order[vi]] = 9999999;
			cout<<variable_order[vi]<<" ";
		}
		cout<<endl;
		int learning_rate;
		if(i<10000)
		 	learning_rate= int((10000-i)/1000);
		else
		 	learning_rate= 1;
	
		if(mutation_depth>learning_rate){
			if(i%2==0){
			      for(int vi=0;vi<learning_rate;vi++){
			      	int num_a = gen()%int(mutation_depth);
			      	int num_b = gen()%int(mutation_depth);
			      	int x = variable_order[num_a];
			      	variable_order[num_a] = variable_order[num_b];
			      	variable_order[num_b] =x;
			      }
			}
			else{
			      for(int vi=0;vi<learning_rate;vi++){
			      	int num_a = gen()%int(mutation_depth);
			      	int num_b = (num_a!=mutation_depth-1) ? (num_a+1) : (num_a-1);
			      	int x = variable_order[num_a];
			      	variable_order[num_a] = variable_order[num_b];
			      	variable_order[num_b] =x;
			      }
			}
		}
		else{
			for(int vi=0;vi<mutation_depth;vi++){
				int num_a = gen()%int(mutation_depth);
				int num_b = gen()%int(mutation_depth);
				int x = variable_order[num_a];
				variable_order[num_a] = variable_order[num_b];
				variable_order[num_b] =x;
			}
		}
		//for(int j=mutation_depth;j<parameter_input_bit_width;j++){
		//	for (int vi=0;vi<parameter_input_bit_width;vi++){
		//		if(feature_variable[vi]<min_feature){
		//			min_feature = feature_variable[vi];
		//			variable_order[j] = vi;
		//		}
		//	}
		//	min_feature = 9999999;
		//	feature_variable[variable_order[j]] = 9999999;

		//}
		if (i==0){
			area = 	BSD_execute(0,variable_order);
		}
		else{
			area = 	BSD_execute(mutation_depth,variable_order);
		}



		if(area < best_area){
			best_area = area;
			best_area_depth = BSD_features_0.BSD_depth;
			for(int vi=0;vi<BSD_features_0.BSD_depth;vi++){
				best_variable_order[vi] = variable_order[vi];
				best_BDD_split_nodes[vi] = BSD_features_0.BSD_area_layers[vi];
			}
			parameter_early_stop_split_nodes = best_area+10;
		}
		
		cout<<endl;
		cout<<"Design Area:	"<<area<<endl;
		cout<<"Best Design Area:	"<<best_area<<endl;
		cout<<endl;
	}
		parameter_early_stop_split_nodes = 999999;
		area = 	BSD_execute(parameter_input_bit_width,best_variable_order);

};

