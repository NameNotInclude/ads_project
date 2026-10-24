# 不同搜索树的比较

用 C 语言实现的五种搜索树及其基准测试程序：

- **BST**：普通二叉搜索树
- **SPLAY**：伸展树（Splay Tree）
- **AVL**：AVL 树
- **RBT**：红黑树（Red-Black Tree）
- **BPT**：B+ 树

程序以 `1..N` 为数据集合，测试递增插入与随机插入，以及同序、逆序、随机三种删除顺序的耗时，并将结果输出为 CSV；配套的 Python 脚本可生成 Altair 图表，`document/` 下包含完整的 LaTeX 实验报告。

所有树的插入与删除均采用**迭代**实现，不使用递归。

## 目录结构

```
.
├── main.c                  # 基准测试入口
├── BST.h / BST.c           # 普通二叉搜索树
├── Splay.h / Splay.c       # 伸展树
├── AVL.h / AVL.c           # AVL 树
├── RBT.h / RBT.c           # 红黑树
├── B+.h / B+.c             # B+ 树
├── visualize/
│   ├── draw.py             # 图表生成脚本（polars + altair）
│   ├── *.html / *.pdf      # 已生成的图表
│   └── csv/                # 基准测试原始数据
│       ├── BST.csv
│       ├── SPLAY.csv
│       ├── AVL.csv
│       ├── RBT.csv
│       └── BPT*.csv        # BPT.csv 及 order=4/10/20/100 的数据
└── document/
    ├── report.tex          # 实验报告 LaTeX 源码
    └── report.pdf          # 编译后的实验报告
```

## 实现要点

| 结构 | 实现方式 |
| ---- | -------- |
| **BST** | 迭代插入/删除/查找；删除时用父指针定位，两个孩子时用右子树最小结点（后继）替换；不维护平衡。 |
| **SPLAY** | 迭代实现 Zig-Zig / Zig-Zag 伸展；插入后新结点成为根；删除时先把目标伸展到根，再合并左右子树。 |
| **AVL** | 结点保存 `height` 与平衡因子 `bf`；使用显式路径栈回溯，失衡时做 LL / RR / LR / RL 旋转。 |
| **RBT** | 结点带 `parent` 指针；插入和删除均迭代修复红黑性质，删除使用 `transplant` + `deleteFixup`。 |
| **B+ 树** | 多路平衡树，`order` 控制结点容量；内部结点按序保存分隔键，叶子结点用 `next` / `prev` 连成链表；插入分裂、删除借位/合并均用显式栈迭代完成。 |

B+ 树默认 `order = 10`，写死在 `main.c` 中：

```c
int order = 10;
```

如需测试其他 order，修改该值后重新编译运行即可，输出的 `Tree_Type` 会变成 `BPT_<order>`。

## 编译

要求 C11（使用 POSIX `clock_gettime`），Linux / macOS / MinGW 均可：

```bash
gcc -O2 -std=gnu11 -o main main.c AVL.c RBT.c B+.c BST.c Splay.c
```

- 建议开启 `-O2`，否则计时结果参考意义不大。
- 若提示 `clock_gettime` / `CLOCK_MONOTONIC` 未定义，可加 `-D_POSIX_C_SOURCE=199309L`，老版本 glibc 可能还需要 `-lrt`。
- 当前实现依赖 POSIX 计时接口，**不支持 MSVC（cl.exe）**；Windows 下请使用 MinGW / MSYS2 或 WSL。

## 运行

```bash
./main <N1> [N2 ...] <迭代次数> <数据结构> <输出.csv>
```

最后三个参数固定为迭代次数、数据结构、输出路径；前面的所有参数都是结点数量，可以一次测试多个规模。

| 参数 | 含义 | 取值 |
| ---- | ---- | ---- |
| `N1 N2 ...` | 每次建树的结点数，数据为 `1..N` | 正整数，可多个 |
| 迭代次数 | 对每个 `N` 重复“建树 + 删除”的轮数 | 正整数 |
| 数据结构 | 被测结构 | `BST`、`SPLAY`（或 `SPT`）、`AVL`、`RBT`、`BPT` |
| 输出路径 | CSV 输出位置，目录必须已存在 | `.csv` 路径 |

示例：

```bash
# 测试 1000 ~ 500000 共 7 个规模，每个规模重复 10 次
./main 1000 5000 10000 50000 100000 200000 500000 10 AVL   visualize/csv/AVL.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 RBT   visualize/csv/RBT.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 SPLAY visualize/csv/SPLAY.csv
./main 1000 5000 10000 50000 100000 200000 500000 10 BPT   visualize/csv/BPT_10.csv

# BST 在有序插入时会退化为链表，500000 个结点单次运行可能需要数分钟
./main 1000 5000 10000 50000 100000 200000 500000 1 BST visualize/csv/BST.csv
```

> B+ 树的 `order` 不是命令行参数；要测 order=4/20/100，请修改 `main.c` 中的 `order` 后重新编译。

### 测试流程

对每个结点数 `N` 和每一轮迭代，分别测试以下三种情况：

1. **Same_Order**：按 `1..N` 递增顺序插入，再按 `1..N` 同序删除；
2. **Reverse_Order**：按 `1..N` 递增顺序插入，再按 `N..1` 逆序删除；
3. **Random_Order**：用 Fisher-Yates 洗牌生成随机排列并随机插入，再生成一个新的随机排列随机删除。

每次插入和删除各自独立计时，重复 `迭代次数` 轮后将耗时累加写入 CSV。

计时使用 `clock_gettime(CLOCK_MONOTONIC)`，单位为毫秒，不受系统时间调整影响。随机顺序在程序内生成，随机种子为 `time(NULL)`，不依赖外部脚本。

### CSV 字段

| 字段 | 含义 |
| ---- | ---- |
| `Tree_Type` | 结构名称；B+ 树为 `BPT_<order>` |
| `Number_of_Node` | 结点数 `N` |
| `Delete_Order` | `Same_Order` / `Reverse_Order` / `Random_Order` |
| `Iteration` | 迭代次数 |
| `Insert_Time_Cost_Total` | 所有迭代的插入总耗时（ms） |
| `Insert_Time_Cost_Average` | 平均每轮插入耗时（ms） |
| `Insert_Time_Cost_Amortized` | 平均每次插入耗时（ms/op） |
| `Delete_Time_Cost_Total` | 所有迭代的删除总耗时（ms） |
| `Delete_Time_Cost_Average` | 平均每轮删除耗时（ms） |
| `Delete_Time_Cost_Amortized` | 平均每次删除耗时（ms/op） |

> `Amortized` 列 = `Total / (Iteration × N)`。乘以 `10^6` 可换算为 ns/op。
>
> 插入耗时的建树方式与本行的 `Delete_Order` 对应：`Same_Order` 和 `Reverse_Order` 行为递增插入，`Random_Order` 行为随机插入。

## 可视化

图表脚本会读取 `visualize/csv/` 下的固定文件，因此**必须在项目根目录执行**：

```bash
pip install polars altair
python visualize/draw.py html
```

`html` 可换成 `pdf`；导出 PDF 还需安装 Altair 的 PDF 导出依赖（如 `vl-convert-python`）。Windows 下若 `python` 不可用，可改用 `py -3 visualize/draw.py html`。

脚本会生成 7 张图：

| 文件 | 内容 |
| ---- | ---- |
| `Increase_insert` | 递增插入耗时对比 |
| `Same_delete` | 同序删除耗时对比 |
| `Reverse_delete` | 逆序删除耗时对比 |
| `Random_insert` | 随机插入耗时对比 |
| `Random_delete` | 随机删除耗时对比 |
| `Increase_insert_without_BST` | 去掉 BST 后的递增插入对比 |
| `Reverse_delete_without_BST` | 去掉 BST 后的逆序删除对比 |

每张图都是“结点数—均摊耗时”的折线图，`Tree_Type` 用颜色区分，悬停可查看具体数据。

## 实验数据与观察

`visualize/csv/` 中已包含 `N = 1000, 5000, 10000, 50000, 100000, 200000, 500000` 的测试结果（`BPT.csv` 为早期数据；`draw.py` 使用 `BPT_4/10/20/100.csv`）。

以 `N = 5×10^5` 的数据为例（BST 为 1 次迭代，其余为 10 次；单位 ns/op）：

| 结构 | 同序插入 | 同序删除 | 逆序删除 | 随机插入 | 随机删除 |
| ---- | -------: | -------: | -------: | -------: | -------: |
| BST | 351309 | 9 | 445459 | 308 | 218 |
| SPLAY | 23 | 30 | 28 | 563 | 661 |
| AVL | 53 | 47 | 45 | 439 | 432 |
| RBT | 112 | 41 | 52 | 452 | 608 |
| BPT（order=10） | 36 | 24 | 46 | 149 | 213 |

主要结论：

- **BST**：随机数据下表现正常，但递增插入会退化为链表，单次插入/逆序删除达到数百微秒，比平衡树慢几个数量级。
- **SPLAY**：有序删除的均摊耗时几乎不随 `N` 增长，体现了很好的局部性；但随机访问会把结点反复伸展到根，随机删除是三种平衡结构中最慢的。
- **AVL / RBT**：有序操作在几十到一百多纳秒量级；随机操作需要更多旋转、变色和随机访存，约慢一个数量级。随机删除时 AVL 反而略快于 RBT。
- **B+ 树**：结点扇出大、树高低，随机操作明显快于二叉树；增大 `order` 可进一步降低随机访问的层数，但结点内线性扫描和分裂/合并的搬移代价会上升，有序负载下 order 过大反而变慢。

> 上述数值与机器、编译选项和系统负载有关，重点应关注不同结构之间的变化趋势，而不是绝对时间。

## 复杂度

设结点数为 `n`，树高为 `h`，B+ 树的 order 为 `m`。

| 结构 | 查找 | 插入 | 删除 | 辅助空间 | 总空间 |
| ---- | ---- | ---- | ---- | -------- | ------ |
| BST | 平均 `O(log n)`，最坏 `O(n)` | 同查找 | 同查找 | `O(1)` | `O(n)` |
| SPLAY | 均摊 `O(log n)`，单次最坏 `O(n)` | 均摊 `O(log n)` | 均摊 `O(log n)` | `O(1)` | `O(n)` |
| AVL | `O(log n)` | `O(log n)` | `O(log n)` | `O(log n)` | `O(n)` |
| RBT | `O(log n)` | `O(log n)` | `O(log n)` | `O(1)` | `O(n)` |
| B+ 树 | `O(log_m n)` 次结点访问 | `O(log_m n)` 次结点访问 | `O(log_m n)` 次结点访问 | `O(log_m n)` | `O(n)` |

- B+ 树若结点内使用线性扫描，比较/搬移次数为 `O(m log_m n)`；`m` 视为常数时即为 `O(log n)`。
- AVL 和 B+ 树在实现中使用显式路径栈，辅助空间为 `O(h)`；BST、SPLAY、RBT 通过父指针或原地旋转实现，辅助空间为 `O(1)`。
- 构建 `n` 个结点：AVL / RBT 为 `O(n log n)`，SPLAY 为均摊 `O(n log n)`，BST 平均 `O(n log n)`、最坏 `O(n^2)`，B+ 树为 `O(n log_m n)`。

## 实验报告

`document/report.tex` 是完整的 LaTeX 报告，包含各结构的算法描述、插入/删除实验结果、不同 order 的 B+ 树对比以及复杂度分析；`document/report.pdf` 为编译产物。

```bash
cd document
latexmk -xelatex report.tex
```

## 注意事项

- 命令行参数没有做合法性检查，请确保结点数、迭代次数为正整数，且输出目录已存在。
- 运行 BST 的大规模有序测试会非常慢，建议减少结点数或迭代次数。
- `main` 为编译产物，修改源码后请重新编译，避免使用旧二进制。
