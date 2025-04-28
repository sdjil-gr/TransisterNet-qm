export PATH=/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/bin:/bin:/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/bin:/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/bin/gcc
export LIBRARY_PATH=/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/bin:/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/lib 
export LD_LIBRARY_PATH=/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/bin:/lustre/S/chengshuyao/haoshiming/anaconda3/envs/BSD/lib
rm rtl/*
g++ BSD.cpp -O3 -std=c++11 -fopenmp -pg 
./a.out 
