from TransistorNet import TransistorNet as tn
import time
from bsd_sim import bsd_sim

def test(tnet, input_len, bsd_path, io_generator, compare = True):
    tnet_accuracy = 1 << (input_len)
    io_generator_accuracy = 1 << (input_len)
    for i in range(1<<input_len):
        input = [int(x) for x in bin(i)[2:].zfill(input_len)]

        tnet_result = tnet.functional_simulate(input)

        if compare:
            io_generator_result = io_generator(input)
            bsd_sim_result = bsd_sim(bsd_path, input)

            if tnet_result != bsd_sim_result:
                print('')
                print(input, '->', tnet_result)
                print(input, '->', io_generator_result)
                print(input, '->', bsd_sim_result)
                tnet_accuracy -= 1
            if io_generator_result != bsd_sim_result:
                print(input, '->', tnet_result)
                print(input, '->', io_generator_result)
                print(input, '->', bsd_sim_result)
                io_generator_accuracy -= 1
                # break
        if i % 10 == 0:
            print(i)

    if compare:
        print(f"tnet_accuracy: {tnet_accuracy/(1<<input_len) * 100:.2f}%, io_generator_accuracy: {io_generator_accuracy /(1<<input_len) * 100:.2f}%")

def test_Transfer8_1():
    tnet = tn.from_bsd('Transfer8_1/Transfer8_1.bsd')
    tnet.to_cdl('Transfer8_1/Transfer8_1.cdl')
    print('load Transfer8_1')
    from Transfer8_1.io import io_generator
    test(tnet, 24, 'Transfer8_1/Transfer8_1.bsd', io_generator)

def test_int2float():
    tnet = tn.from_bsd('int2float/int2float.bsd')
    tnet.to_cdl('int2float/int2float.cdl')
    print('load int2float')
    from int2float.io import io_generator
    test(tnet, 11, 'int2float/int2float.bsd', io_generator)

# 测试随机输入下的性能
def random_test_time(testbench : str, input_len : int):
    tnet = tn.from_bsd(f'{testbench}/{testbench}.bsd')
    tnet.to_cdl(f'{testbench}/{testbench}.cdl')
    print(f'load {testbench}')
    from int2float.io import io_generator
    import random

    inputs = [0] * input_len
    times = 1000
    
    start_time = time.time()
    for i in range(times):
        for i in range(input_len):
            inputs[i] = random.randint(0, 1)
        tnet.functional_simulate(inputs)
    print(f"tnet: %s seconds, {times} times" % (time.time() - start_time))

    start_time = time.time()
    for i in range(times):
        for i in range(input_len):
            inputs[i] = random.randint(0, 1)
        io_generator(inputs)
    print(f"io_generator: %s seconds, {times} times" % (time.time() - start_time))

    start_time = time.time()
    for i in range(times):
        for i in range(input_len):
            inputs[i] = random.randint(0, 1)
        bsd_sim(f'{testbench}/{testbench}.bsd', inputs)
    print(f"bsd_sim: %s seconds, {times} times" % (time.time() - start_time))

if __name__ == '__main__':
    # test_Transfer8_1()
    # test_int2float()
    random_test_time('int2float', 11)
    