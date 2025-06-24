class TransistorNet_kernel:
    class Transistor:
        def __init__(self, t_type, transistor_id, g_id, d_id, s_id):
            self.type = t_type  # "pmos" / "nmos"
            self.id = transistor_id
            self.g_id = g_id    # gate
            self.d_id = d_id    # drain
            self.s_id = s_id    # source

            # flags (valid only after functional_simulation)
            self.passble = None  # passble flag
            self.out_id = None   # output node id

            # default values
            self.w = 54.0       # width in nm
            self.l = 20         # length in nm
            self.nfin = 2       # number of fins
            self.R = 1           # resistance in default unit
            self.C = 1           # capacitance in default unit
    
    class UnionFind:
        # a simple implementation of union-find data structure
        def __init__(self):
            self.parent = {}
            self.rank = {}       # the rank of each component
            self.nodes = {}       # the list of nodes in each component
        
        def add(self, x, rank = 1):
            if x not in self.parent:
                self.parent[x] = x
                self.rank[x] = rank
                self.nodes[x] = [x]
        
        def find(self, x):
            if self.parent[x] != x:
                self.parent[x] = self.find(self.parent[x])
            return self.parent[x]
        
        def union(self, x, y):
            x_root = self.find(x)
            y_root = self.find(y)
            if x_root == y_root: return
        
            if self.rank[x_root] < self.rank[y_root]:
                self.parent[x_root] = y_root
                self.nodes[y_root].extend(self.nodes[x_root])
                del self.nodes[x_root]
            else:
                self.parent[y_root] = x_root
                self.nodes[x_root].extend(self.nodes[y_root])
                del self.nodes[y_root]
                if self.rank[x_root] == self.rank[y_root]:
                    self.rank[x_root] += 1

    def __init__(self):
        self.type = 'union'
        self.transistors = []          # the list of transistors in the net
        self.uf = self.UnionFind()     # the union-find data structure for nodes
        self.in_size = 0               # the number of inputs in the net
        self.out_size = 0              # the number of outputs in the net
        self.node_info = {}            # the metadata of nodes {id: (transistor_obj, 'G'/'D'/'S')}
        self.tname = ""                # the name of the net
        self.special_nodes = []        # the list of special nodes in the net
        # self.netwires = []           # the list of netwires in the net

    def add_transistor(self, t_type):
        # generate a unique id
        transistor_id = f"T{len(self.transistors)}"
        g_id = f"{transistor_id}_G"
        d_id = f"{transistor_id}_D"
        s_id = f"{transistor_id}_S"
        
        # add to the uf
        for node_id in [g_id, d_id, s_id]:
            self.uf.add(node_id)
        
        # create a new transistor object and store it
        t = self.Transistor(t_type, transistor_id, g_id, d_id, s_id)
        self.transistors.append(t)
        # update the node_info
        self.node_info[g_id] = (t, 'G')
        self.node_info[d_id] = (t, 'D')
        self.node_info[s_id] = (t, 'S')
        return t
    
    def add_special_node(self, node_id, node_type):
        # add a special node to the net
        self.uf.add(node_id, 4) # rank = 4 for special nodes, so they are more likely to be root nodes
        self.node_info[node_id] = (None, node_type)
        self.special_nodes.append(node_id)

    def connect(self, node_id1, node_id2):
        # connect two nodes
        self.uf.union(node_id1, node_id2)

    def is_connected(self, node_id1, node_id2):
        # check if two nodes are connected
        return self.uf.find(node_id1) == self.uf.find(node_id2)
    
    def get_connected_nodes(self, node_id):
        # get the list of connected nodes
        root = self.uf.find(node_id)
        return self.uf.nodes[root]
    
    def node_wire(self, node_id):
        return self.uf.find(node_id)
    
    def functional_simulate_kernel(self, inputs, get_path = False):
        # simulate the net with given input values
        # inputs: a list of input values [value_0, value_1, ...] (values are 0 or 1)
        if(self.in_size != len(inputs)):
            raise Exception("Invalid input size. expected: " + str(self.in_size) + " got: " + str(len(inputs)))

        from collections import deque

        value_dict = {f"In_{i}": inputs[i] for i in range(len(inputs))}
        father = {}

        q = deque() # (node_id, type) type is True as a diffusion, False as a source if a mos
        for input in value_dict:
            q.append((input, True))

        q.append(("VDD", True))
        q.append(("VSS", True))


        value_dict["VDD"] = 1
        value_dict["VSS"] = 0

        result = [0] * self.out_size
        get_count = 0

        while q:
            node_id, diffusion = q.popleft()
            node_value = value_dict[node_id]
            if diffusion:
                for next_node_id in self.get_connected_nodes(node_id):
                    if next_node_id == node_id: # pass the same node
                        continue
                    
                    next_tst = self.node_info[next_node_id][0]

                    if next_node_id in value_dict:
                        if value_dict[next_node_id] == None:
                            pass
                        elif value_dict[next_node_id] != node_value:
                            raise Exception("Invalid networks.")
                        else:
                            continue
                    value_dict[next_node_id] = node_value
                    if next_tst:
                        father[next_tst] = node_id
                    elif self.node_info[next_node_id][1] == 'OUT':
                        father[next_node_id] = node_id
                    q.append((next_node_id, False))
            else :
                if self.node_info[node_id][1] == 'OUT':
                    idx = int(node_id.split('_')[1])
                    result[idx] = node_value
                    get_count += 1
                    if get_count == self.out_size:
                        break
                    continue
                
                tst = self.node_info[node_id][0] # transistor
                node_type = self.node_info[node_id][1] # G/D/S/OUT/IN
                gn, dn, sn = tst.g_id, tst.d_id, tst.s_id # gate, drain, source

                # check if the mos is passable
                passable = (tst.type == 'nmos' and value_dict[gn] == 1) or (tst.type == 'pmos' and value_dict[gn] == 0)
                tst.passable = passable
                
            # ?   Only searching by Gate or D/S is enough, but can use both of them to check if the mos is passable (this may 
            # ?   be much slower). So defaultly only searching by one.
                if node_type == 'G':
                    # if dn in value_dict and value_dict[dn] != None:
                    #     if sn not in value_dict or value_dict[sn] == None: # drain -> source
                    #         if passable:
                    #             value_dict[sn] = value_dict[dn]
                    #             q.append((sn, True))
                    #         else :
                    #             value_dict[sn] = None
                    #     else: # drain <-> source
                    #         if passable:
                    #             if value_dict[sn] != value_dict[dn]:
                    #                 raise Exception("Invalid networks.")
                    # else:
                    #     if sn in value_dict and value_dict[sn] != None: # source -> drain
                    #         if passable:
                    #             value_dict[dn] = value_dict[sn]
                    #             q.append((dn, True))
                    #         else :
                    #             value_dict[dn] = None
                    #     else: # source - drain
                    #         q.append((node_id, False))
                    pass
                elif node_type == 'D' or node_type == 'S':
                    on = tst.s_id if node_type == 'D' else tst.d_id # other node
                    if gn in value_dict and value_dict[gn] != None:
                        if on not in value_dict or value_dict[on] == None: # this -> other
                            if passable:
                                value_dict[on] = node_value
                                q.append((on, True))
                                tst.out_id = on
                            else :
                                value_dict[on] = None
                        else: # this <-> other
                            if passable:
                                if value_dict[on] != node_value:
                                    raise Exception("Invalid networks.")
                    else:
                        q.append((node_id, False))
                    pass

                else:
                    raise Exception("Invalid networks.")
                
            

        if get_path:
            path = []
            for i in range(self.out_size):
                ipath = []
                node_id = f"Y_{i}"
                while True:
                    node_id = father[node_id]
                    if self.node_info[node_id][0] == None:
                        ipath.append(node_id)
                        break
                    else:
                        tst = self.node_info[node_id][0]
                        ipath.append(node_id)
                        node_id = tst
                path.append(ipath)
            return result, path, value_dict
        else:
            return result, [], {}
    