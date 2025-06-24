import re
import numpy as np

class list_node:
    def __init__(self,node_id):
        self.node_id = node_id
        self.next = None

class link_list:
    def __init__(self):
        self.head = None

    def is_empty(self):
        return self.head is None

    def append(self,node_id):
        #print("inside append")
        node = list_node(node_id)
        if self.is_empty():
            self.head = node
        else:
            current = self.head
            while current.next is not None: # type: ignore
                current = current.next # type: ignore
            current.next = node # type: ignore
    
    def delete(self,node_id):
        if self.is_empty():
            print("empty list")
        else:
            current = self.head
            while current.next is not None: # type: ignore
                if current.next.node_id == node_id: # type: ignore
                    tmp = current.next # type: ignore
                    current.next = tmp.next # type: ignore
                else:
                    current = current.next # type: ignore
        
    
    def print_all(self):
        #print("in print_all")
        curr = self.head
        if self.is_empty():
            print("empty list")
        else:
            while curr is not None:
                print(curr.node_id,"(",curr.node_id//3,")"," ",  end='')
                curr = curr.next
            print("\n")

class TransistorNet_kernel:
    class Transistor:
        def __init__(self, t_type, transistor_id, g_id, d_id, s_id):
            self.type = t_type  # "pmos" / "nmos"
            self.id = transistor_id
            self.g_id = g_id    # gate
            self.d_id = d_id    # drain
            self.s_id = s_id    # source
            self.g_list = link_list()
            self.d_list = link_list()
            self.s_list = link_list()

            # default values
            self.w = 54.0       # width in nm
            self.l = 20       # length in nm
            self.nfin = 2      # number of fins

    class Special_nodes:
        def __init__(self,id):
            self.name = id 
            self.list = link_list()
    
    def __init__(self):
        self.type = 'graph'
        self.transistors = []          # the list of transistors in the net
        self.in_size = 0               # the number of inputs in the net
        self.out_size = 0              # the number of outputs in the net
        self.node_info = {}            # the metadata of nodes {id: (transistor_obj, 'G'/'D'/'S')}
        self.node_list = {}
        self.tname = ""                # the name of the net
        self.special_nodes = []        # the list of special nodes in the net
        self.result = "True"

    def add_transistor(self, t_type):
        # 新增晶体管，返回该晶体管
        tran_id = len(self.transistors)
        transistor_id = f"T{len(self.transistors)}"
        g_id = 3*tran_id
        d_id = 3*tran_id + 1
        s_id = 3*tran_id + 2
        #self.node_id = self.node_id + 3

        tran = self.Transistor(t_type, transistor_id, g_id, d_id, s_id)
        # self.transistor_num = self.transistor_num + 1
        self.transistors.append(tran)
        #self.transistor_num = self.transistor_num + 1
        self.node_list[g_id] = (tran.g_list)
        self.node_list[d_id] = (tran.d_list)
        self.node_list[s_id] = (tran.s_list)

        self.node_info[g_id] = (tran, 'G')
        self.node_info[d_id] = (tran, 'D')
        self.node_info[s_id] = (tran, 'S')
        return tran
    
    def add_special_node(self, node_id, node_type):
        # 新增特殊节点（单节点，例如VDD、VSS、IN[]、OUT），返回该节点, vdd为-2，vss为-1，out为-3
        spec_node = self.Special_nodes(node_id)
        self.special_nodes.append(node_id)
        self.node_list[node_id] = (spec_node.list)
        self.node_info[node_id] = (None, node_type)


    def connect(self, node_id1, node_id2):
        # 连接两个节点,理论上gds的id除3向下取整即可得到transistor id，即在net中数组的下标
        print("connecting ", node_id1, "and ", node_id2)

        node1 = self.node_list[node_id1]
        node2 = self.node_list[node_id2]
        node1.append(node_id2)
        node2.append(node_id1)

        

    def is_connected(self, node_id1, node_id2):
        # 检查两个节点是否连通（并非图意义上的连通，而是检查是否电路意义上的直接连接）
        pass
    
    def get_connected_nodes(self, node_id):
        # 获取与指定节点相连的节点列表
        connect_list = []
        target_list = self.node_list[node_id]
        node = target_list.head
        while node is not None:
            connect_list.append(node.node_id)
            node = node.next
        return connect_list

    def print_trans(self):
        for trans in self.transistors:
            print(trans.g_id,trans.d_id,trans.s_id,trans.row,trans.column)

    def print_trans_connection(self):
        for trans in self.transistors:
            print("row is ", trans.row, "column is ", trans.column)
            trans.d_list.print_all()

    def cut_invalid_node(self,list,input):
        node = list.head
        while node is not None:
            id = self.node_list[node.node_id]
            if isinstance(id,int):
                tran_off = id//3
                tran = self.transistors[tran_off]
                input_id = tran.g_list.head.node_id
                number = int(input_id.split("_")[1])
                if tran.t_type == "pmos":
                    if input[number] == 1:
                        delete_node_id = node.node_id
                        node = node.next
                        list.delete(delete_node_id)
                    else:
                        node=node.next
                elif tran.t_type == "nmos":
                    if input[number] == 0:
                        delete_node_id = node.node_id
                        node = node.next
                        list.delete(delete_node_id)
                    else :
                        node=node.next
                else:
                    print("error occurred in cut invalid node")
            else:
                node = node.next
        return
    
    def cut_all(self,list,input):
        print("in cut all")
        self.cut_invalid_node(list,input)
        node = list.head
        while node is not None:
            id = node.node_id
            print(f'id is {id}')
            if isinstance(id,int):
                tran_off = id//3
                tran = self.transistors[tran_off]
                next_list = tran.s_list
                print("transistor node")
                self.cut_all(next_list,input)
            elif id == "VDD":
                self.result = "True"
                print("reach VDD")
            elif id == "VSS":
                self.result = "False"
                print("reach VSS")
            node = node.next
        return
            
    
    @staticmethod
    def from_bsd(file_path):
        # 从BSD格式文件中导入数据，返回TransistorNet的一个实例
        print("in to bsd")
        row_num = 0
        Mynet = TransistorNet_kernel()
        Mynet.add_special_node("VDD", 'VDD')
        Mynet.add_special_node("VSS", 'VSS')

        with open(file_path, 'r') as file:
            lines = file.readlines()
            last_line = lines[-1]
            input_order = list(map(int, re.findall(r'\d+', last_line)))
            input_len = len(input_order) 
            for i in range(input_len):
                input_var = f"In_{input_order[i]}"
                Mynet.add_special_node(input_var,'IN')
                print(Mynet.special_nodes[-1])
            Mynet.add_special_node("Y", 'OUT')
            my_list = []
            my_list.append("Y")
            for line in lines[:-2]:
                upper_list = []
                count = 0
                elements = eval(line)
                for (left, right) in elements:
                    print(left,right)
                    input_var = f"In_{input_order[row_num]}"
                    pmos = Mynet.add_transistor("pmos")
                    pmos_off = len(Mynet.transistors) - 1
                    nmos = Mynet.add_transistor("nmos")
                    nmos_off = len(Mynet.transistors) - 1
                    
                    if left==-1:
                        Mynet.connect("VDD", pmos.s_id)
                    elif left == -2:
                        Mynet.connect("VSS", pmos.s_id)
                        print("link to VSS")
                    else:
                        upper_list.append(Mynet.transistors[pmos_off].s_id)

                    if right==-1:
                        Mynet.connect("VDD", nmos.s_id)
                    elif right == -2:
                        Mynet.connect("VSS", nmos.s_id)
                    else:
                        upper_list.append(Mynet.transistors[nmos_off].s_id)

                    Mynet.connect(input_var, pmos.g_id)
                    Mynet.connect(input_var, nmos.g_id)
                    Mynet.connect(pmos.d_id, nmos.d_id)

                    Mynet.connect(my_list[count],pmos.d_id)
                    Mynet.connect(my_list[count],nmos.d_id)
                    count = count + 1
                my_list = upper_list

        print("finish")
        return Mynet
    
    def node_wire(self, node_id):
        return 'None'
    
    def functional_simulate_kernel(self, inputs):
        start = "Y"
        backup = self
        op_list = backup.node_info[start]
        backup.cut_invalid_node(op_list, inputs)
        id = op_list.head.node_id
        tran_off = id//3
        tran = backup.transistors[tran_off]
        op_list = tran.s_list
        print(op_list.head.node_id)
        backup.cut_all(op_list, inputs)
        return backup.result