# 不同搜索树的比较

用 C 语言实现的 **AVL 树** 与 **红黑树**（Red-Black Tree），并提供基准测试程序，对比两种平衡二叉搜索树在不同插入 / 删除顺序下的性能表现。

## 项目结构

```
.
├── AVL.h / AVL.c        # AVL 树的实现
├── RBT.h / RBT.c        # 红黑树的实现
├── main.c               # 基准测试入口
├── random_generate/
│   └── test.py          # 生成随机删除顺序的脚本
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

## 运行

```bash
./main <结点数量> <迭代次数> <数据结构>
```

参数说明：

| 参数 | 含义 | 取值 |
| ---- | ---- | ---- |
| 结点数量 | 每次构建树的结点个数（数据为 `1..N`） | 正整数 |
| 迭代次数 | 重复构建 / 销毁的次数 | 正整数 |
| 数据结构 | 选择被测树 | `AVL` 或 `RBT` |

示例：

```bash
# 对 AVL 树测试 10000 个结点，重复 100 次
./main 10000 100 AVL

# 对红黑树测试 10000 个结点，重复 100 次
./main 10000 100 RBT
```

输出示例：

```
AVL, delete in same order
number of node:10000,iteration:100 time cost:77.070 ms
AVL, delete in reverse order
number of node:10000,iteration:100 time cost:71.999 ms
AVL, delete in random order
number of node:10000,iteration:100 time cost:228.599 ms
```

> 计时使用 `clock_gettime(CLOCK_MONOTONIC)` 的**毫秒级**单调时钟，不受系统时间调整影响。

每种结构都会测试三种删除顺序（插入顺序固定为递增的 `1..N`）：

1. **同序删除**：按插入顺序 `1..N` 删除；
2. **逆序删除**：按 `N..1` 逆序删除；
3. **随机删除**：由 C 内置的 Fisher-Yates 洗牌生成的随机排列顺序删除。

> 随机顺序在 `main.c` 中直接生成（`Random_order`），每次迭代重新洗牌，
> 洗牌时间不计入计时；随机种子为当前系统时间，
> 不依赖外部脚本。

## 测试数据

`random_generate/test.py` 仍可用于独立生成一组 `0..N-1` 的随机打乱序列：

```bash
python3 random_generate/test.py 100 > random_generate/input.txt
```

注意：`main.c` 的随机删除已改为 C 内置洗牌，不再调用该脚本。

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

