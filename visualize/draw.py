import polars as pl
import altair as alt

df_bst = pl.read_csv('visualize/csv/BST.csv')
df_avl = pl.read_csv('visualize/csv/AVL.csv')
df_rbt = pl.read_csv('visualize/csv/RBT.csv')
df_splay = pl.read_csv('visualize/csv/SPLAY.csv')
df_bpt_4 = pl.read_csv('visualize/csv/BPT_4.csv')
df_bpt_10 = pl.read_csv('visualize/csv/BPT_10.csv')
df_bpt_20 = pl.read_csv('visualize/csv/BPT_20.csv')
df_bpt_100 = pl.read_csv('visualize/csv/BPT_100.csv')

df_bst_asds = df_bst.filter(pl.col('Delete_Order') == 'Same_Order')
df_avl_asds = df_avl.filter(pl.col('Delete_Order') == 'Same_Order')
df_rbt_asds = df_rbt.filter(pl.col('Delete_Order') == 'Same_Order')
df_splay_asds = df_splay.filter(pl.col('Delete_Order') == 'Same_Order')
df_bpt_4_asds = df_bpt_4.filter(pl.col('Delete_Order') == 'Same_Order')
df_bpt_10_asds = df_bpt_10.filter(pl.col('Delete_Order') == 'Same_Order')
df_bpt_20_asds = df_bpt_20.filter(pl.col('Delete_Order') == 'Same_Order')
df_bpt_100_asds = df_bpt_100.filter(pl.col('Delete_Order') == 'Same_Order')


df_bst_asdr = df_bst.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_avl_asdr = df_avl.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_rbt_asdr = df_rbt.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_splay_asdr = df_splay.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_bpt_4_asdr = df_bpt_4.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_bpt_10_asdr = df_bpt_10.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_bpt_20_asdr = df_bpt_20.filter(pl.col('Delete_Order') == 'Reverse_Order')
df_bpt_100_asdr = df_bpt_100.filter(pl.col('Delete_Order') == 'Reverse_Order')

df_bst_ardr = df_bst.filter(pl.col('Delete_Order') == 'Random_Order')
df_avl_ardr = df_avl.filter(pl.col('Delete_Order') == 'Random_Order')
df_rbt_ardr = df_rbt.filter(pl.col('Delete_Order') == 'Random_Order')
df_splay_ardr = df_splay.filter(pl.col('Delete_Order') == 'Random_Order')
df_bpt_4_ardr = df_bpt_4.filter(pl.col('Delete_Order') == 'Random_Order')
df_bpt_10_ardr = df_bpt_10.filter(pl.col('Delete_Order') == 'Random_Order')
df_bpt_20_ardr = df_bpt_20.filter(pl.col('Delete_Order') == 'Random_Order')
df_bpt_100_ardr = df_bpt_100.filter(pl.col('Delete_Order') == 'Random_Order')

df_asds = pl.concat(
    [df_splay_asds,df_rbt_asds,df_bpt_4_asds,df_bst_asds,df_avl_asds,df_bpt_10_asds,df_bpt_20_asds,df_bpt_100_asds],
    how='vertical')

df_asdr = pl.concat(
    [df_splay_asds,df_rbt_asds,df_bpt_4_asds,df_bpt_10_asds,df_bpt_20_asds,df_bpt_100_asds,df_bst_asds,df_avl_asds],
    how='vertical')

df_ardr = pl.concat(
    [df_splay_ardr,df_rbt_ardr,df_bpt_4_ardr,df_bpt_10_ardr,df_bpt_20_ardr,df_bpt_100_ardr,df_bst_ardr,df_avl_ardr],
    how='vertical')
print(df_asds,df_asdr,df_ardr)

asds_d = alt.Chart(df_asds).mark_point().encode(
    x="Number_of_Node:Q",
    y="Delete_Time_Cost_Amortized:Q",
    color="Tree_Type",
    tooltip=["Number_of_Node","Delete_Time_Cost_Amortized","Delete_Order","Tree_Type"]
)

asds_i = alt.Chart(df_asds).mark_point().encode(
    x="Number_of_Node:Q",
    y="Insert_Time_Cost_Amortized:Q",
    color="Tree_Type",
    tooltip=["Number_of_Node","Insert_Time_Cost_Amortized","Tree_Type"]
)

asdr_d = alt.Chart(df_asds).mark_point().encode(
    x="Number_of_Node:Q",
    y="Delete_Time_Cost_Amortized:Q",
    color="Tree_Type",
    tooltip=["Number_of_Node","Delete_Time_Cost_Amortized","Delete_Order","Tree_Type"]
)

ardr_i = alt.Chart(df_asds).mark_point().encode(
    x="Number_of_Node:Q",
    y="Insert_Time_Cost_Amortized:Q",
    color="Tree_Type",
    tooltip=["Number_of_Node","Delete_Time_Cost_Amortized","Delete_Order","Tree_Type"]
)

ardr_d = alt.Chart(df_asds).mark_point().encode(
    x="Number_of_Node:Q",
    y="Delete_Time_Cost_Amortized:Q",
    color="Tree_Type",
    tooltip=["Number_of_Node","Delete_Time_Cost_Amortized","Delete_Order","Tree_Type"]
)


asds_i.save('visualize/Increase_insert.pdf')
asds_d.save('visualize/Same_delete.pdf')
asdr_d.save('visualize/Reverse_delete.pdf')
ardr_i.save('visualize/Random_insert.pdf')
ardr_d.save('visualize/Random_delete.pdf')
