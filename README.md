# 不同搜索树的比较

用 C 语言实现的 **AVL 树** 与 **红黑树**（Red-Black Tree），并提供基准测试程序，对比两种平衡二叉搜索树在不同插入 / 删除顺序下的性能表现。

## 项目结构

```
.
├── AVL.h / AVL.c        # AVL 树的实现
├── RBT.h / RBT.c        # 红黑树的实现
├── main.c               # 基准测试入口
├── visualie/
│   └── draw.py          # 绘制实验图像
|   └── AVL.csv / RBT.csv#AVL、RBT原始实验数据
|   └── AVL_combined.html     #AVL实验图像，分为线性坐标系以及对数坐标系   
|   └── RBT_combined.html     #RBT实验图像，分为线性坐标系以及对数坐标系   
└── main                 # 编译产物（可执行文件）
```

## 数据结构

### AVL 树（`AVL.h`）

```c
typedef struct nA {
    struct nA* left;
    struct nA* right;
    int data;
    int bf;       // 平衡因子 = 左子树高 - 右子树高
    int height;   // 结点高度（叶子为 0）
} Anode;
```

- 采用**递归**方式实现插入与删除。
- 通过 `update()` 维护高度与平衡因子，失衡时进行 LL / RR / LR / RL 旋转。
- 删除时用右子树最小结点替换，再回溯修复平衡。

### 红黑树（`RBT.h`）

```c
typedef struct nR {
    struct nR* left;
    struct nR* right;
    struct nR* parent;   // 指向父结点
    int data;
    int color;           // RED(0) / BLACK(1)
} Rnode;
```

- 采用**迭代**方式实现，结点带 `parent` 指针。
- 插入：红黑性质调整，处理叔叔为红（变色上溯）与叔叔为黑（旋转）两类情况。
- 删除：`transplant` + `deleteFixup`，完整处理兄弟结点四种情形。

## 编译

```bash
gcc -O2 -o main main.c AVL.c RBT.c
```

> 建议开启 `-O2` 以更真实地反映运行性能；如需调试可加 `-g`。

### 不同操作系统的命令

**Linux / macOS**（使用 gcc）：

```bash
gcc -O2 -o main main.c AVL.c RBT.c   # 生成可执行文件 main
```

**Windows（MinGW / MSYS2 / WSL）**：

```bat
:: MinGW / MSYS2：生成 main.exe，运行时可省略 .exe
gcc -O2 -o main.exe main.c AVL.c RBT.c

:: 如果使用 MSVC（cl.exe），需手动指定源文件并生成 main.exe
cl /O2 /Fe:main.exe main.c AVL.c RBT.c
```

| 操作系统 | 可执行文件 | 打开生成的 HTML 图表 |
| -------- | ---------- | -------------------- |
| Linux    | `main`     | `xdg-open visualize/AVL_combined.html` |
| macOS    | `main`     | `open visualize/AVL_combined.html` |
| Windows  | `main.exe` | `start visualize\AVL_combined.html` |

## 运行

Linux / macOS：

```bash
./main <结点数量> <迭代次数> <数据结构> <输出地址>
```

Windows（PowerShell 需加 `.\`，cmd 可直接写 `main.exe`）：

```powershell
# PowerShell
.\main.exe <结点数量> <迭代次数> <数据结构> <输出地址>

# cmd
main.exe <结点数量> <迭代次数> <数据结构> <输出地址>
```

参数说明：

| 参数 | 含义 | 取值 |
| ---- | ---- | ---- |
| 结点数量 | 每次构建树的结点个数（数据为 `1..N`），可以写多个 | 正整数 |
| 迭代次数 | 重复构建 / 销毁的次数 | 正整数 |
| 数据结构 | 选择被测树 | `AVL` 或 `RBT` |
| 输出地址 | 程序会将结果以csv的形式输出，此处填写输出结果的地址 | `.csv`文件路径 |

示例：

```bash
# 对 AVL 树测试 10000 个结点，重复 100 次，输出到./visualize/AVL.csv
./main 10000 100 AVL ./visualize/AVL.csv

# 对红黑树测试 10000 个结点，重复 100 次，输出到当前目录中的RBT.csv
./main 10000 100 RBT ./RBT.csv

# 对 AVL 树测试 1000、5000 个结点，重复 10 次，输出到./visualize/AVL.csv
./main 1000 5000 10 AVL ./visualize/AVL.csv
```

> 计时使用 `clock_gettime(CLOCK_MONOTONIC)` 的**毫秒级**单调时钟，不受系统时间调整影响。

每种结构都会测试三种删除顺序（插入顺序固定为递增的 `1..N`）：

1. **同序删除**：按插入顺序 `1..N` 删除；
2. **逆序删除**：按 `N..1` 逆序删除；
3. **随机删除**：由 C 内置的 Fisher-Yates 洗牌生成的随机排列顺序删除。

> 每轮迭代中**插入耗时与删除耗时分别独立计时**，因此输出中会同时给出插入和删除的总耗时与平均耗时。

> 随机顺序在 `main.c` 中直接生成（`Random_order`），随机种子为当前系统时间，
> 不依赖外部脚本。

输出示例：

Terminal
``` 
AVL, delete in same order
number of node:10000,iteration:100 insert time cost:38.500 ms, delete time cost:38.570 ms
AVL, delete in reverse order
number of node:10000,iteration:100 insert time cost:37.660 ms, delete time cost:34.339 ms
AVL, delete in random order
number of node:10000,iteration:100 insert time cost:36.300 ms, delete time cost:192.299 ms
```
.csv文件
``` 
Tree_Type,Number_of_Node,Delete_Order,Iteration,Insert_Time_Cost_Total,Insert_Time_Cost_Average,Delete_Time_Cost_Total,Delete_Time_Cost_Average
AVL,1000,Same_Order,10,0.420000,0.042000,0.274000,0.027400
```

# 可视化
本实验的绘图所用的python库有**polars**以及**altair**，请确保你的python环境已安装这两种库。

``` bash
python visualize/draw.py <原始csv文件路径> <需保存的格式>
```

> Windows 下如果 `python` 不可用，可改用 `py -3`，如 `py -3 visualize/draw.py visualize/AVL.csv html`。

参数说明：

| 参数 | 含义 | 取值 |
| ---- | ---- | ---- |
| 原始csv文件路径 | 原始数据 | `.csv`文件路径 |
| 需保存的格式 | 保存的格式 | `html png`等 |

示例
```bash
# 根据 CSV 生成线性坐标图、双对数坐标图并排组合图，保存为 html：

python visualize/draw.py visualize/AVL.csv html
```

该命令会生成 `visualize/AVL_combined.html`，左图为线性坐标、右图为对数坐标，
并在同一张图中对比**插入**与**删除**的平均耗时（不同删除顺序用点形状区分）。
图表包含标题，并可悬停数据点查看详细数据。

## 快速运行

下面按操作系统分别给出可直接复制粘贴的完整命令（含编译、测试、绘图、打开图表）。

### Linux

```bash
# 1. 编译
gcc -O2 -o main main.c AVL.c RBT.c

# 2. 运行基准测试（节点数 1000~500000，迭代 10 次）
./main 1000 5000 10000 50000 100000 200000 500000 10 AVL visualize/AVL.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 RBT visualize/RBT.csv

# 3. 生成图表（线性坐标 + 双对数坐标并排组合图）
python visualize/draw.py visualize/AVL.csv html
python visualize/draw.py visualize/RBT.csv html

# 4. 用浏览器打开图表
xdg-open visualize/AVL_combined.html
xdg-open visualize/RBT_combined.html
```

### macOS

```bash
# 1. 编译
gcc -O2 -o main main.c AVL.c RBT.c

# 2. 运行基准测试（节点数 1000~500000，迭代 10 次）
./main 1000 5000 10000 50000 100000 200000 500000 10 AVL visualize/AVL.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 RBT visualize/RBT.csv

# 3. 生成图表（线性坐标 + 双对数坐标并排组合图）
python3 visualize/draw.py visualize/AVL.csv html
python3 visualize/draw.py visualize/RBT.csv html

# 4. 用浏览器打开图表
open visualize/AVL_combined.html
open visualize/RBT_combined.html
```

### Windows（PowerShell）

```powershell
# 1. 编译（需已安装 MinGW/MSYS2 的 gcc）
gcc -O2 -o main.exe main.c AVL.c RBT.c

# 2. 运行基准测试（节点数 1000~500000，迭代 10 次）
.\main.exe 1000 5000 10000 50000 100000 200000 500000 10 AVL visualize/AVL.csv
.\main.exe 1000 5000 10000 50000 100000 200000 500000 10 RBT visualize/RBT.csv

# 3. 生成图表（python 不可用时改用 py -3）
python visualize/draw.py visualize/AVL.csv html
python visualize/draw.py visualize/RBT.csv html

# 4. 用浏览器打开图表
start visualize\AVL_combined.html
start visualize\RBT_combined.html
```

> 无论哪种系统，都要在项目根目录下执行这些命令。

## 复杂度

| 操作 | AVL 树 | 红黑树 |
| ---- | ------ | ------ |
| 查找 | O(log n) | O(log n) |
| 插入 | O(log n) | O(log n) |
| 删除 | O(log n) | O(log n) |
| 平衡严格程度 | 高度平衡（左右子树高差 ≤ 1） | 近似平衡（最长路径 ≤ 2×最短路径） |

- AVL 树平衡更严格，查找更快，但插入 / 删除时旋转更频繁；
- 红黑树平衡较宽松，插入 / 删除的旋转次数更少，综合增删性能通常更优。

## 实现方式

- **AVL 树**：插入 / 删除均为**迭代**实现，使用显式路径栈代替递归调用栈。
- **红黑树**：插入 / 删除同样为**迭代**实现，借助 `parent` 指针回溯调整。
  
## 待实现

- **B树**,
- **splay树**
- **普通BST**
