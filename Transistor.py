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
