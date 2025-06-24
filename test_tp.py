from TransistorNet import TransistorNet as tn


samp = 2000

# tnet = tn.from_bsd('int2float/int2float.bsd')
tnet = tn.from_bsd('Transfer8_1/Transfer8_1.bsd')

avg_tp, max_tp = tnet.get_tp(samp)

print('avg:', avg_tp)
print('max:', max_tp)
