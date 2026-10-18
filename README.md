# 不同搜索树的比较

用 C 语言实现的 **普通二叉搜索树（BST）**、**伸展树（Splay Tree）**、**AVL 树**、**红黑树**（Red-Black Tree）与 **B+ 树**，并提供基准测试程序，对比几种搜索树在不同插入 / 删除顺序下的性能表现。所有树的插入、删除与释放均采用**迭代**实现，不使用递归。

## 项目结构

```
.
├── BST.h / BST.c        # 普通二叉搜索树的实现（迭代）
├── Splay.h / Splay.c    # 伸展树的实现（迭代）
├── AVL.h / AVL.c        # AVL 树的实现
├── RBT.h / RBT.c        # 红黑树的实现
├── B+.h / B+.c          # B+ 树的实现
├── BST.h / BST.c        # 二叉搜索树的实现
├── Splay.h / Splay.c    # Splay树的实现
├── main.c               # 基准测试入口
├── visualie/
│   └── draw.py          # 绘制实验图像
└── BST.csv / AVL.csv / RBT.csv / BPT.csv / SPLAY.csv #BST、AVL、RBT、BPT、SPLAY原始实验数据
|   └── AVL_combined.html/html    #AVL实验图像，分为线性坐标系以及对数坐标系   
|   └── RBT_combined.html/html      #RBT实验图像，分为线性坐标系以及对数坐标系   
|   └── BPT_combined.html/html     #BPT实验图像，分为线性坐标系以及对数坐标系   
|   └── BST_combined.html/html     #BST实验图像，分为线性坐标系以及对数坐标系  
|   └── SPLAY_combined.html/html     #Splay Tree实验图像，分为线性坐标系以及对数坐标系  
├── document/
│   └── report.tex        # 实验报告latex源码
|   └── report.html        # 实验报告
└── main                  # 编译产物（可执行文件）
```

## 数据结构

### 普通二叉搜索树（`BST.h`）

```c
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
```

- 采用**迭代**方式实现插入、删除、查找与释放。
- 插入：沿查找路径下降到空位置，重复键不重复插入。
- 删除：使用可增长的动态数组保存根到目标结点的路径，代替递归回溯；
  被删结点有两个孩子时，用右子树最小结点（后继）替换。
- 释放：通过“左孩子上旋”遍历整棵树，额外空间为 O(1)，不使用递归。
- 不保证平衡：有序数据插入时最坏退化为链表，单次操作 O(n)；
  随机数据下平均为 O(log n)。

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

- 采用**迭代**方式实现插入与删除。
- 通过 `update()` 维护高度与平衡因子，失衡时进行 LL / RR / LR / RL 旋转。
- 删除时用右子树最小结点替换，再回溯修复平衡。

### 伸展树（`Splay.h`）

```c
typedef struct SplayNode {
    int val;
    struct SplayNode *left;
    struct SplayNode *right;
} SplayNode;
```

- 采用**迭代**方式实现 `splay`、插入、删除、查找与释放。
- 每次访问后通过 Zig-Zig / Zig-Zag 双旋转将目标结点（或最接近的结点）伸展到根。
- 插入：先伸展，再把新结点作为根，原树拆成左右子树挂到新根下。
- 删除：先把目标伸展到根，再合并左右子树；释放同样为迭代实现。
- 单次操作最坏 O(n)，但**均摊**复杂度为 O(log n)，具有良好的局部性。

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
  
### B+树（`B+.h`）

```c
typedef struct Bn
{
    int leaf;         // 是否为leaf结点
    int* data;
    int num;

    struct Bn** children;
    struct Bn* next;  // leaf节点的链表指针
    struct Bn* prev;
}BNode;
typedef struct Bn* PtrToBNode;

typedef struct 
{
    PtrToBNode root;   // 指向树根
    PtrToBNode head;   // 指向链表头
    int order;
}BTree;
```

- 采用**迭代**的方式实现插入与删除，手动维护一个栈。
- 插入：处理结点分裂，以及继续向上传递。
- 删除：处理向兄弟节点借值，以及节点合并。
- B+树的`order`在`main.c`文件当中写定，可以在当中修改。


## 编译

```bash
gcc -O2 -o main main.c AVL.c RBT.c B+.c BST.c Splay.c
```

> 建议开启 `-O2` 以更真实地反映运行性能；如需调试可加 `-g`。

### 不同操作系统的命令

**Linux / macOS**（使用 gcc）：

```bash
gcc -O2 -o main main.c AVL.c RBT.c B+.c BST.c Splay.c   # 生成可执行文件 main
```

**Windows（MinGW / MSYS2 / WSL）**：

```bat
:: MinGW / MSYS2：生成 main.exe，运行时可省略 .exe
gcc -O2 -o main.exe main.c AVL.c RBT.c B+.c BST.c Splay.c

:: 如果使用 MSVC（cl.exe），需手动指定源文件并生成 main.exe
cl /O2 /Fe:main.exe main.c AVL.c RBT.c B+.c BST.c Splay.c
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
| 数据结构 | 选择被测树 | `BST`、`SPLAY`、`AVL`、`RBT` 或 `BPT` |
| 输出地址 | 程序会将结果以csv的形式输出，此处填写输出结果的地址 | `.csv`文件路径 |

示例：

```bash
# 对 AVL 树测试 10000 个结点，重复 100 次，输出到./visualize/AVL.csv
./main 10000 100 AVL ./visualize/AVL.csv

# 对红黑树测试 10000 个结点，重复 100 次，输出到当前目录中的RBT.csv
./main 10000 100 RBT ./RBT.csv

# 对 AVL 树测试 1000、5000 个结点，重复 10 次，输出到./visualize/AVL.csv
./main 1000 5000 10 AVL ./visualize/AVL.csv

# 对 BPT 树测试 1000、5000 个结点，重复 10 次，order为10，输出到./visualize/BPT.csv
./main 1000 5000 10 BPT ./visualize/BPT.csv

# 对普通 BST 测试 10000 个结点，重复 100 次，输出到./visualize/BST.csv
./main 10000 100 BST ./visualize/BST.csv

# 对伸展树测试 10000 个结点，重复 100 次，输出到./visualize/SPLAY.csv
./main 10000 100 SPLAY ./visualize/SPLAY.csv
```

> `SPLAY` 也可以简写为 `SPT`。B+ 树的 `order` 在 `main.c` 中写定为 10。

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
BST, delete in same order
number of node:10000,iteration:100 insert time cost:12.500 ms, delete time cost:0.570 ms
BST, delete in reverse order
number of node:10000,iteration:100 insert time cost:12.660 ms, delete time cost:250.339 ms
BST, delete in random order
number of node:10000,iteration:100 insert time cost:12.300 ms, delete time cost:65.299 ms
```
.csv文件
``` 
Tree_Type,Number_of_Node,Delete_Order,Iteration,Insert_Time_Cost_Total,Insert_Time_Cost_Average,Insert_Time_Cost_Amortized,Delete_Time_Cost_Total,Delete_Time_Cost_Average,Delete_Time_Cost_Amortized
BST,1000,Same_Order,10,0.420000,0.042000,0.000042,0.274000,0.027400,0.000027
SPLAY,1000,Same_Order,10,0.051000,0.005100,0.000005,0.070000,0.007000,0.000007
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
gcc -O2 -o main main.c AVL.c RBT.c B+.c BST.c Splay.c

# 2. 运行基准测试（节点数 1000~500000，迭代 10 次）
./main 1000 5000 10000 50000 100000 200000 500000 10 AVL visualize/AVL.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 RBT visualize/RBT.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 BPT visualize/BPT.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 SPLAY visualize/SPLAY.csv
# BST 在有序插入时会退化为链表，建议使用较小规模
./main 1000 5000 10000 50000 10 BST visualize/BST.csv

# 3. 生成图表（线性坐标 + 双对数坐标并排组合图）
python visualize/draw.py visualize/AVL.csv html
python visualize/draw.py visualize/RBT.csv html
python visualize/draw.py visualize/BPT.csv html
python visualize/draw.py visualize/SPLAY.csv html
python visualize/draw.py visualize/BST.csv html

# 4. 用浏览器打开图表
xdg-open visualize/AVL_combined.html
xdg-open visualize/RBT_combined.html
xdg-open visualize/BPT_combined.html
xdg-open visualize/SPLAY_combined.html
xdg-open visualize/BST_combined.html
```

### macOS

```bash
# 1. 编译
gcc -O2 -o main main.c AVL.c RBT.c B+.c BST.c Splay.c

# 2. 运行基准测试（节点数 1000~500000，迭代 10 次）
./main 1000 5000 10000 50000 100000 200000 500000 10 AVL visualize/AVL.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 RBT visualize/RBT.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 BPT visualize/BPT.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 SPLAY visualize/SPLAY.csv
# BST 在有序插入时会退化为链表，建议使用较小规模
./main 1000 5000 10000 50000 10 BST visualize/BST.csv

# 3. 生成图表（线性坐标 + 双对数坐标并排组合图）
python3 visualize/draw.py visualize/AVL.csv html
python3 visualize/draw.py visualize/RBT.csv html
python3 visualize/draw.py visualize/BPT.csv html
python3 visualize/draw.py visualize/SPLAY.csv html
python3 visualize/draw.py visualize/BST.csv html

# 4. 用浏览器打开图表
open visualize/AVL_combined.html
open visualize/RBT_combined.html
open visualize/BPT_combined.html
open visualize/SPLAY_combined.html
open visualize/BST_combined.html
```

### Windows（PowerShell）

```powershell
# 1. 编译（需已安装 MinGW/MSYS2 的 gcc）
gcc -O2 -o main.exe main.c AVL.c RBT.c B+.c BST.c Splay.c

# 2. 运行基准测试（节点数 1000~500000，迭代 10 次）
.\main.exe 1000 5000 10000 50000 100000 200000 500000 10 AVL visualize/AVL.csv
.\main.exe 1000 5000 10000 50000 100000 200000 500000 10 RBT visualize/RBT.csv
.\main.exe 1000 5000 10000 50000 100000 200000 500000 10 BPT visualize/BPT.csv
.\main.exe 1000 5000 10000 50000 100000 200000 500000 10 SPLAY visualize/SPLAY.csv
# BST 在有序插入时会退化为链表，建议使用较小规模
.\main.exe 1000 5000 10000 50000 10 BST visualize/BST.csv

# 3. 生成图表（python 不可用时改用 py -3）
python visualize/draw.py visualize/AVL.csv html
python visualize/draw.py visualize/RBT.csv html
python visualize/draw.py visualize/BPT.csv html
python visualize/draw.py visualize/SPLAY.csv html
python visualize/draw.py visualize/BST.csv html

# 4. 用浏览器打开图表
start visualize\AVL_combined.html
start visualize\RBT_combined.html
start visualize\BPT_combined.html
start visualize\SPLAY_combined.html
start visualize\BST_combined.html
```

> 无论哪种系统，都要在项目根目录下执行这些命令。

## 复杂度

| 操作 | 普通 BST | 伸展树 | AVL 树 | 红黑树 | B+树 |
| ---- | -------- | ------ | ------ | ------ | ----- |
| 查找 | O(n) 最坏 / O(log n) 平均 | 均摊 O(log n) | O(log n) | O(log n) | O(log_m n) |
| 插入 | O(n) 最坏 / O(log n) 平均 | 均摊 O(log n) | O(log n) | O(log n) | O(log_m n) |
| 删除 | O(n) 最坏 / O(log n) 平均 | 均摊 O(log n) | O(log n) | O(log n) | O(log_m n) |
| 平衡严格程度 | 不保证平衡，有序数据会退化为链表 | 不保证单次平衡，但访问后自动调整形状 | 高度平衡（左右子树高差 ≤ 1） | 近似平衡（最长路径 ≤ 2×最短路径） | 多路平衡分支，内部结点按序存储、叶子结点链表相连 |

- 普通 BST 实现最简单，随机数据下表现良好，但最坏情况退化为链表；
- 伸展树不需要额外的平衡信息，均摊 O(log n)，适合存在局部性（频繁访问相近键）的场景；
- AVL 树平衡更严格，查找更快，但插入 / 删除时旋转更频繁；
- 红黑树平衡较宽松，插入 / 删除的旋转次数更少，综合增删性能通常更优；
- B+ 树的分支因子为 `m`，每次查找 / 插入 / 删除都在 `O(log_m n)` 的层级上进行，适合范围查询和磁盘索引场景。

## 实现方式

- **普通 BST**：插入、查找、删除、释放均为**迭代**实现；删除使用可增长的显式路径栈，释放使用 O(1) 额外空间的指针旋转遍历。
- **伸展树**：`splay`、插入、删除、查找均为**迭代**实现，`splay` 使用左右两棵临时树重组，释放同样不使用递归。
- **AVL 树**：插入 / 删除均为**迭代**实现，使用显式路径栈代替递归调用栈。
- **红黑树**：插入 / 删除同样为**迭代**实现，借助 `parent` 指针回溯调整。
- **B+ 树**：采用多路分支结构，使用 `order` 控制每个结点的最大关键字数，并维护叶子结点之间的链表以支持顺序访问和范围查询。
