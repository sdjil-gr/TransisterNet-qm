#### 将所有的端点按电势从0开始编号，所有的等电势的端点具有相同的编号。

#### 一个晶体管网络用三元组(x, y, z)表示，其中x, y, z分别表示晶体管的D/S、D/S和栅极的编号。

#### 若所有的等电势点编号的集合表示为
$$V$$

#### 晶体管网络可以表示为
$$S_{Pmos}  = \{(x, y, z)|x, y, z \in V，(x, y, z)为网络中的一个PMOS\}$$
$$S_{Nmos}  = \{(x, y, z)|x, y, z \in V，(x, y, z)为网络中的一个NMOS\}$$
$$S = S_{Pmos} \cup S_{Nmos}$$


#### 一个晶体管网络在给定输入后是一个确定的状态，令$\psi(x)$为编号为x的端点所对应的逻辑值（$\psi(x) = 0,1$），则满足以下的式子:

- 对于PMOS，当$\psi(z) = 1$时，$\psi(x) = \psi(y)$，因此

$$\psi(z) \land (\psi(x) \oplus \psi(y)) = 0 \quad \forall (x, y, z) \in S_{Pmos}$$

- 类似的，对于NMOS，当$\psi(z) = 0$时，$\psi(x) = \psi(y)$，因此

$$\overline{\psi(z)}\land (\psi(x) \oplus \psi(y)) = 0 \quad \forall (x, y, z) \in S_{Nmos}$$

#### 为了排除其中出现的未定义值，比如中间某些等电势点由于周围的晶体管均未连通而出现的情况，令$h(x)$为编号为x的端点是否未定义（$h(x) = 0, 1$），$h(x)$为1表示该端点未定义，则满足以下的式子:

- 根据未定义状态的定义以及其传递性，有：

$$h(y) = (\wedge_{(x, y, z) \in S_{Pmos}} (\overline{\psi(z)} \lor h(x))) \land (\wedge_{(y, x, z) \in S_{Pmos}} (\overline{\psi(z)} \lor h(x))) \land\\
(\wedge_{(x, y, z) \in S_{Nmos}} (\psi(z) \lor h(x)) )\land (\wedge_{(y, x, z) \in S_{Nmos}} (\psi(z) \lor h(x)))
\quad \forall y \in V$$

#### 在对确定的晶体管网络给定输入后，以上均为可确定的方程式，仿真等价于判断整个方程组有没有解，有 $h(out)=0$ 的解，说明该网络是可用的，此时 $\psi(out)$ 为即为输出的逻辑值。