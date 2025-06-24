# selecet the type of net to use
# ! now support union only

from Tnet_union import TransistorNet_kernel
# from Tnet_graph import TransistorNet_kernel

from random import randint
import tqdm

class TransistorNet(TransistorNet_kernel):
    @staticmethod
    def from_bsd(file_path):
        """
        Create a TransistorNet object from a BSD file.
        
        Args:
            file_path: the path of the BSD file.
        
        Returns:
            A TransistorNet object.
        """
        try:
            bsd_read = eval('['+','.join(open(file_path, 'r').readlines())+']')
            input_order = list(reversed(bsd_read[-1]))
            bsd_layers = list(reversed(bsd_read[:-1]))

            tnet = TransistorNet()
            tnet.in_size = max(input_order) + 1

            tnet.add_special_node("VDD", 'VDD')
            tnet.add_special_node("VSS", 'VSS')

            for i in range(tnet.in_size):
                tnet.add_special_node(f"In_{i}", 'IN')

            node_list = []
            for level, layer in enumerate(bsd_layers):
                new_list = []
                input_var = f"In_{input_order[level]}"
                
                for idx, (left, right) in enumerate(layer):

                    pmos = tnet.add_transistor("pmos")
                    nmos = tnet.add_transistor("nmos")

                    tnet.connect(input_var, pmos.g_id)
                    tnet.connect(input_var, nmos.g_id)
                    tnet.connect(pmos.d_id, nmos.d_id)

                    if left == -1:
                        tnet.connect("VDD", pmos.s_id)
                    elif left == -2:
                        tnet.connect("VSS", pmos.s_id)
                    else:
                        tnet.connect(node_list[left], pmos.s_id)

                    if right == -1:
                        tnet.connect("VDD", nmos.s_id)
                    elif right == -2:
                        tnet.connect("VSS", nmos.s_id)
                    else:
                        tnet.connect(node_list[right], nmos.s_id)

                    new_list.append(pmos.d_id)
                node_list = new_list
            
            for out_idx, node_id in enumerate(node_list):
                tnet.add_special_node(f"Y_{out_idx}", 'OUT')
                tnet.connect(f"Y_{out_idx}", node_id)
                tnet.out_size += 1
    
        except:
            print(f"Error: {file_path} is not a valid BSD file.")
            raise Exception
        
        tnet.tname = file_path.split('/')[-1].split('.')[0]
        tnet.special_nodes.sort()
        tnet.transistors.sort(key=lambda x: x.type)

        return tnet
    
    def to_cdl(self, file_path, append=False):
        """
        Write the net to a CDL file.
        
        Args:
            file_path: the path of the CDL file.
            append: whether to append to the file or not.
        """
        # write the net to a CDL file
        def pretreated(): 
            with open(file_path, 'a' if append else 'w') as f:
                f.write(".SUBCKT " + self.tname)
                for node_id in self.special_nodes:
                    f.write(f" {node_id}")
                f.write("\n")

                for t in self.transistors:
                    f.write(f"{t.id} {self.node_wire(t.d_id)} {self.node_wire(t.g_id)} {self.node_wire(t.s_id)} " \
                        + f"{'VSS' if t.type == 'nmos' else 'VDD'} {t.type} " \
                        + f"w={t.w}n l={t.l}n nfin={t.nfin} \n")

                f.write(".ENDS\n\n")

        pretreated()
    
    def get_tp(self, samp):
        """
        Get the average and maximum TP of the net.
        
        Args:
            samp: the number of samples to get the TP.
        
        Returns:
            A tuple of two lists, the first list is the average TP of each output, the second list is the maximum TP of each output.
        """
        input_num = self.in_size
        output_num = self.out_size
        sum_eyp_tp = [0] * output_num
        max_eyp_tp:list[int] = [0] * output_num

        def get_sum_C(tnet, tst):
            if tst in already_tst:
                return 0
            already_tst.append(tst)
            out_node = tst.out_id
            sum_C = 0
            for other in tnet.get_connected_nodes(out_node):
                other_tst = tnet.node_info[other][0]
                if other_tst and other_tst.passable:
                    sum_C += get_sum_C(tnet, other_tst)
            return sum_C


        for i in tqdm.tqdm(range(samp)):
            already_tst = []
            inputs = [int(x) for x in bin(randint(1, 2**input_num - 1))[2:].zfill(input_num)]

            result, path, value_dict = self.functional_simulate_kernel(inputs, True)
            lpath = list(map(lambda x: x[:-1][::-1], path))
            everypath_tp = []
            for p in lpath:
                sum_R = 0
                sum_tp = 0
                for i in range(len(p)-1):
                    node = p[i]
                    tst = self.node_info[node][0]
                    next_tst = self.node_info[p[i+1]][0]
                    sum_R += tst.R
                    sum_tp += tst.C * sum_R
                    already_tst.append(tst)
                    for other in self.get_connected_nodes(node):
                        if other == node:
                            continue
                        other_tst = self.node_info[other][0]
                        if other_tst and other_tst != next_tst and other_tst.passable:
                            sum_tp += sum_R * get_sum_C(self, other_tst)
                last_tst = self.node_info[p[-1]][0]
                sum_R += last_tst.R
                sum_tp += last_tst.C * sum_R
                everypath_tp.append(sum_tp)

            sum_eyp_tp = [sum(x) for x in zip(sum_eyp_tp, everypath_tp)]
            max_eyp_tp = [max(x) for x in zip(max_eyp_tp, everypath_tp)]
        
        avg_eyp_tp:list[int] = [x/samp for x in sum_eyp_tp]
        
        return avg_eyp_tp, max_eyp_tp

        
    def functional_simulate(self, inputs):
        """
        Simulate the net with given input values.
        
        Args:
            inputs: a list of input values [value_0, value_1, ...] (values are 0 or 1)
        
        Returns:
            A list of output values.
        """
        if self.type == "union":
            return self.functional_simulate_kernel(inputs)[0] # type: ignore
        elif self.type == "graph":
            return self.functional_simulate_kernel(inputs) # type: ignore
    