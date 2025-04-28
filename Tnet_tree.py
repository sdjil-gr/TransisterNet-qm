from Transistor import Transistor

class TransistorNet_kernel:
    def __init__(self):

        self.transistors = []          # the list of transistors in the net
        self.in_size = 0               # the number of inputs in the net
        self.out_size = 0              # the number of outputs in the net
        self.node_info = {}            # the metadata of nodes {id: (transistor_obj, 'G'/'D'/'S')}
        self.tname = ""                # the name of the net
        self.special_nodes = []        # the list of special nodes in the net