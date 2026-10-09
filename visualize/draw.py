import polars as pl
import altair as alt
from pathlib import Path
import sys

file_name = Path(sys.argv[1])
output_format = sys.argv[2].lstrip(".")
df = pl.read_csv(file_name)

# 将插入/删除的平均耗时整理成长表，便于在同一张图里对比
long_df = df.unpivot(
    index=["Tree_Type", "Number_of_Node", "Delete_Order", "Iteration"],
    on=["Insert_Time_Cost_Average", "Delete_Time_Cost_Average"],
    variable_name="Operation",
    value_name="Average_Time",
).with_columns(
    pl.col("Operation")
      .str.replace("_Time_Cost_Average", "")
      .alias("Operation")
)

tooltip = [
    alt.Tooltip("Tree_Type:N", title="Tree"),
    alt.Tooltip("Number_of_Node:Q", title="Number of nodes"),
    alt.Tooltip("Delete_Order:N", title="Delete order"),
    alt.Tooltip("Operation:N", title="Operation"),
    alt.Tooltip("Iteration:Q", title="Iterations"),
    alt.Tooltip("Average_Time:Q", title="Average time (ms)", format=".3f"),
]

base = alt.Chart(long_df).encode(
    color=alt.Color("Operation:N", title="Operation"),
    shape=alt.Shape("Delete_Order:N", title="Delete order"),
    x="Number_of_Node:Q",
    y=alt.Y("Average_Time:Q", title="Average time (ms)"),
    tooltip=tooltip
)

line_chart = base.mark_point()
point_chart = base.mark_point(filled=True)
base_chart = line_chart + point_chart

output_path = file_name.with_suffix(f".{output_format}")

linear_chart = base_chart.properties(title="Linear scale")

log_chart = base_chart.encode(
    x=alt.X(
        "Number_of_Node:Q",
        title="Number of nodes",
        scale=alt.Scale(type="log")
    ),
    y=alt.Y(
        "Average_Time:Q",
        title="Average time (ms)",
        scale=alt.Scale(type="linear")
    )
).properties(title="Linear-Log scale")

combined_chart = alt.hconcat(
    linear_chart,
    log_chart,
    title=f"{df['Tree_Type'][0]} insert / delete performance"
)
combined_chart.save(
    output_path.with_name(f"{output_path.stem}_combined{output_path.suffix}")
)
