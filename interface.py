# only as a example, not complete

class TransistorNet_self:
    class Transistor:
        def __init__(self, t_type : str, transistor_id : str, g_id : str, d_id : str, s_id : str):
            self.type = t_type  # "pmos" / "nmos"
            self.id = transistor_id
            self.g_id = g_id    # gate
            self.d_id = d_id    # drain
            self.s_id = s_id    # source

            # default values
            self.w = 54.0       # width in nm
            self.l = 20       # length in nm
            self.nfin = 2      # number of fins

    def __init__(self):
        self.transistors = []          # the list of transistors in the net
        # self.uf = self.UnionFind()     # the union-find data structure for nodes
        self.in_size = 0               # the number of inputs in the net
        self.out_size = 0              # the number of outputs in the net
        self.node_info = {}            # the metadata of nodes {id: (transistor_obj, 'G'/'D'/'S')}
        self.tname = ""                # the name of the net
        self.special_nodes = []        # the list of special nodes in the net
        # self.netwires = []           # the list of netwires in the net

    def add_transistor(self, t_type : str) -> 'Transistor':
        # 新增晶体管，返回该晶体管

        # generate a unique id
        transistor_id = f"T{len(self.transistors)}"
        g_id = f"{transistor_id}_G"
        d_id = f"{transistor_id}_D"
        s_id = f"{transistor_id}_S"

        # TODO:
        pass

        # create a new transistor object and store it
        t = self.Transistor(t_type, transistor_id, g_id, d_id, s_id)
        self.transistors.append(t)
        # update the node_info
        self.node_info[g_id] = (t, 'G')
        self.node_info[d_id] = (t, 'D')
        self.node_info[s_id] = (t, 'S')
        return t
    
    def add_special_node(self, node_id : str, node_type : str):
        # 新增特殊节点（单节点，例如VDD、VSS、IN[]、OUT）

        # TODO:
        pass

        self.node_info[node_id] = (None, node_type)
        self.special_nodes.append(node_id)

    def connect(self, node_id1 : str, node_id2 : str):
        # 连接两个节点

        # TODO:
        pass

    def is_connected(self, node_id1 : str, node_id2 : str) -> bool: # type: ignore
        # 检查两个节点是否连通（并非图意义上的连通，而是检查是否电路意义上的直接连接），返回布尔值

        # TODO:
        pass
    
    def get_connected_nodes(self, node_id : str) -> list[str]: # type: ignore
        # 获取与指定节点相连的节点列表，返回列表

        # TODO:
        pass

class TransistorNet(TransistorNet_self):

# 上述接口统一后，以下可以共用
    @staticmethod
    def from_bsd(file_path : str) -> 'TransistorNet': # type: ignore
        # 从BSD格式文件中导入数据，返回TransistorNet的一个实例
        pass

    def to_cdl(self, file_path : str, append : bool = False):
        # 将TransistorNet导出为CDL格式文件
        pass

    def functional_simulate(self, inputs : list[bool]) -> list[bool]: # type: ignore
        # 行为仿真，输入逻辑值，输出电路输出值
        pass