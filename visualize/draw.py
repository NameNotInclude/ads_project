import polars as pl
import altair as alt

import sys

def load_csv(name):
    return pl.read_csv(f'visualize/csv/{name}.csv')


df_bst = load_csv('BST')
df_avl = load_csv('AVL')
df_rbt = load_csv('RBT')
df_splay = load_csv('SPLAY')
df_bpt_4 = load_csv('BPT_4')
df_bpt_10 = load_csv('BPT_10')
df_bpt_20 = load_csv('BPT_20')
df_bpt_100 = load_csv('BPT_100')

all_dfs = [
    df_splay, df_rbt,
    df_bpt_4, df_bpt_10, df_bpt_20, df_bpt_100,
    df_bst, df_avl,
]


def filter_order(order):
    return pl.concat(
        [df.filter(pl.col('Delete_Order') == order) for df in all_dfs],
        how='vertical',
    )


df_asds = filter_order('Same_Order')
df_asdr = filter_order('Reverse_Order')
df_ardr = filter_order('Random_Order')


def line_chart(df, y_field, title, tooltip):
    """散点连成折线：相同 Tree_Type 的点按节点数顺序连接。"""
    return (
        alt.Chart(df.sort(['Tree_Type', 'Number_of_Node']))
        .mark_line(point=True)
        .encode(
            x=alt.X('Number_of_Node:Q', title='Number of Nodes'),
            y=alt.Y(f'{y_field}:Q'),
            color=alt.Color('Tree_Type:N', title='Tree Type'),
            tooltip=tooltip,
        )
        .properties(title=title)
    )


insert_tooltip = [
    'Number_of_Node', 'Insert_Time_Cost_Amortized',
    'Delete_Order', 'Tree_Type',
]
delete_tooltip = [
    'Number_of_Node', 'Delete_Time_Cost_Amortized',
    'Delete_Order', 'Tree_Type',
]

asds_i = line_chart(
    df_asds, 'Insert_Time_Cost_Amortized', 'Increase Insert',
    insert_tooltip,
)
asds_d = line_chart(
    df_asds, 'Delete_Time_Cost_Amortized', 'Same Order Delete',
    delete_tooltip,
)
asdr_d = line_chart(
    df_asdr, 'Delete_Time_Cost_Amortized', 'Reverse Order Delete',
    delete_tooltip,
)
ardr_i = line_chart(
    df_ardr, 'Insert_Time_Cost_Amortized', 'Random Insert',
    insert_tooltip,
)
ardr_d = line_chart(
    df_ardr, 'Delete_Time_Cost_Amortized', 'Random Order Delete',
    delete_tooltip,
)

# 不包含 BST 的单独图表
df_asds_no_bst = df_asds.filter(pl.col('Tree_Type') != 'BST')
df_asdr_no_bst = df_asdr.filter(pl.col('Tree_Type') != 'BST')

asds_i_no_bst = line_chart(
    df_asds_no_bst, 'Insert_Time_Cost_Amortized',
    'Increase Insert (without BST)', insert_tooltip,
)
asdr_d_no_bst = line_chart(
    df_asdr_no_bst, 'Delete_Time_Cost_Amortized',
    'Reverse Order Delete (without BST)', delete_tooltip,
)

asds_i.save('visualize/Increase_insert'+sys.argv[1])
asds_d.save('visualize/Same_delete'+sys.argv[1])
asdr_d.save('visualize/Reverse_delete'+sys.argv[1])
ardr_i.save('visualize/Random_insert'+sys.argv[1])
ardr_d.save('visualize/Random_delete'+sys.argv[1])
asds_i_no_bst.save('visualize/Increase_insert_without_BST'+sys.argv[1])
asdr_d_no_bst.save('visualize/Reverse_delete_without_BST'+sys.argv[1])
