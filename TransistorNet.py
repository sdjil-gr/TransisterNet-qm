from Tnet_union import TransistorNet_kernel
# from Tnet_graph import TransistorNet_kernel

class TransistorNet(TransistorNet_kernel):
    @staticmethod
    def from_bsd(file_path):
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
        # write the net to a CDL file
        def pretreated(): 
            with open(file_path, 'a' if append else 'w') as f:
                f.write(".SUBCKT " + self.tname)
                for node_id in self.special_nodes:
                    f.write(f" {node_id}")
                f.write("\n")

                for t in self.transistors:
                    f.write(f"{t.id} {self.uf.find(t.d_id)} {self.uf.find(t.g_id)} {self.uf.find(t.s_id)} " \
                        + f"{'VSS' if t.type == 'nmos' else 'VDD'} {t.type} " \
                        + f"w={t.w}n l={t.l}n nfin={t.nfin} \n")

                f.write(".ENDS\n\n")

        pretreated()

    def functional_simulate(self, inputs):
        # simulate the net with given input values
        # inputs: a list of input values [value_0, value_1, ...] (values are 0 or 1)
        if(self.in_size != len(inputs)):
            raise Exception("Invalid input size. expected: " + str(self.in_size) + " got: " + str(len(inputs)))

        from collections import deque

        value_dict = {f"In_{i}": inputs[i] for i in range(len(inputs))}

        q = deque() # (node_id, type) type is True as a diffusion, False as a source if a mos
        for input in value_dict:
            q.append((input, True))

        q.append(("VDD", True))
        q.append(("VSS", True))


        value_dict["VDD"] = 1
        value_dict["VSS"] = 0

        result = [0] * self.out_size

        while q:
            node_id, diffusion = q.popleft()
            node_value = value_dict[node_id]
            if diffusion:
                for next_node_id in self.get_connected_nodes(node_id):
                    if next_node_id == node_id: # pass the same node
                        continue
                    if next_node_id in value_dict:
                        if value_dict[next_node_id] == None:
                            value_dict[next_node_id] = node_value
                        elif value_dict[next_node_id] != node_value:
                            raise Exception("Invalid networks.")
                        continue
                    value_dict[next_node_id] = node_value
                    q.append((next_node_id, False))
            else :
                if self.node_info[node_id][1] == 'OUT':
                    idx = int(node_id.split('_')[1])
                    result[idx] = node_value
                    continue
                
                tst = self.node_info[node_id][0] # transistor
                node_type = self.node_info[node_id][1] # G/D/S/OUT/IN
                gn, dn, sn = tst.g_id, tst.d_id, tst.s_id # gate, drain, source
                on = tst.s_id if node_type == 'D' else tst.d_id # other node

                # check if the mos is passable
                passable = (tst.type == 'nmos' and value_dict[gn] == 1) or (tst.type == 'pmos' and value_dict[gn] == 0)
                
                if node_type == 'G':
                    if dn in value_dict and value_dict[dn] != None:
                        if sn not in value_dict or value_dict[sn] == None: # drain -> source
                            if passable:
                                value_dict[sn] = value_dict[dn]
                                q.append((sn, True))
                            else :
                                value_dict[sn] = None
                        else: # drain <-> source
                            if passable:
                                if value_dict[sn] != value_dict[dn]:
                                    raise Exception("Invalid networks.")
                    else:
                        if sn in value_dict and value_dict[sn] != None: # source -> drain
                            if passable:
                                value_dict[dn] = value_dict[sn]
                                q.append((dn, True))
                            else :
                                value_dict[dn] = None
                        else: # source - drain
                            q.append((node_id, False))

                elif node_type == 'D' or node_type == 'S':
                    # if gn in value_dict and value_dict[gn] != None:
                    #     if on not in value_dict or value_dict[on] == None: # this -> other
                    #         if passable:
                    #             value_dict[on] = node_value
                    #             q.append((on, True))
                    #         else :
                    #             value_dict[on] = None
                    #     else: # this <-> other
                    #         if passable:
                    #             if value_dict[on] != node_value:
                    #                 raise Exception("Invalid networks.")
                    # else:
                    #     q.append((node_id, False))
                    pass

                else:
                    raise Exception("Invalid networks.")
    
        return result
        
    