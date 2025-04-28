# a simple function to simulate a BSD circuit using a given input vector.


def bsd_sim(file_path, inputs):
    bsd_read = eval('['+','.join(open(file_path, 'r').readlines())+']')
    input_order = list(bsd_read[-1])
    bsd_layers = list(bsd_read[:-1])
    # layer_num = len(bsd_layers)

    if len(inputs)!= max(input_order) + 1:
        raise ValueError('Number of inputs does not match the number of input nodes in the BSD file.')
    
    results = []
    
    for idx, x in enumerate(bsd_layers[0]):
        value = idx
        level = 0
        while value != -1 and value != -2:
            lr = bsd_layers[level][value]
            value = lr[inputs[input_order[level]]]
            level += 1
        results.append(value + 2)

    return results