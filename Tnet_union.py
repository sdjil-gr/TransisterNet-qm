class TransistorNet_kernel:
    class Transistor:
        def __init__(self, t_type, transistor_id, g_id, d_id, s_id):
            self.type = t_type  # "pmos" / "nmos"
            self.id = transistor_id
            self.g_id = g_id    # gate
            self.d_id = d_id    # drain
            self.s_id = s_id    # source

            # default values
            self.w = 54.0       # width in nm
            self.l = 20       # length in nm
            self.nfin = 2      # number of fins
    
    class UnionFind:
        # a simple implementation of union-find data structure
        def __init__(self):
            self.parent = {}
            self.rank = {}
            self.nodes = {}
        
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
            self.node_info[node_id] = (None, None)
        
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