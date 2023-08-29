

echo "Working on NewReno vs HighSpeed Congestion Window"

./ns3 run --quiet "scratch/1905077.cc" -- --choice=2

output_png6="scratch/cwnd/NewRenoVsHighSpeed_cwnd.png"

cwnd_data_file_name5="scratch/cwnd/NewReno_cwnd_vs_t.dat"
cwnd_data_file_name6="scratch/cwnd/HighSpeed_cwnd_vs_t.dat"

gnuplot_script6="scratch/NewRenoVsHighSpeed_cwnd_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script6"
echo "set output '$output_png6'" >> "$gnuplot_script6"
echo "set xlabel 'Time'" >> "$gnuplot_script6"
echo "set ylabel 'Congestion Window'" >> "$gnuplot_script6"
echo "plot '$cwnd_data_file_name5' using 1:2 with linespoints title 'NewReno', '$cwnd_data_file_name6' using 1:2 with linespoints title 'HighSpeed'" >> "$gnuplot_script6"

# Run the Gnuplot script
gnuplot "$gnuplot_script6"


echo "Working on NewReno vs HighSpeed Throughput vs Packet Loss Rate"
#node part
lr_data_file_name3="offline-2-1-output/NewRenoVsHighSpeed_tpVslr.dat"



# Define the range of x-axis values
x_values=(-6 -5 -4 -3 -2)

# Compile and run your .cc file for each x value
for x in "${x_values[@]}"; do
    
    output=$(./ns3 run --quiet "scratch/1905077.cc" -- --lossrate="$x" --choice=2)
    echo "$output"
    echo "$x $output" >> "$lr_data_file_name3"
done

output_png7="offline-2-1-output/NewRenoVsHighSpeed_tpVslr.png"
gnuplot_script7="offline-2-1-output/NewRenoVsHighSpeed_tpVslr_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script7"
echo "set output '$output_png7'" >> "$gnuplot_script7"
echo "set xlabel 'Packet Loss Rate'" >> "$gnuplot_script7"
echo "set ylabel 'Throughput'" >> "$gnuplot_script7"
echo "plot '$lr_data_file_name3' using 1:2 with linespoints title 'NewReno', '$lr_data_file_name3' using 1:3 with linespoints title 'HighSpeed'" >> "$gnuplot_script7"

# Run the Gnuplot script
gnuplot "$gnuplot_script7"

ji_file_name="offline-2-1-output/NewRenoVsHighSpeed_jainVslr.png"
gnuplot_script7="offline-2-1-output/NewRenoVsHighSpeed_jainVslr_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script7"
echo "set output '$ji_file_name'" >> "$gnuplot_script7"
echo "set xlabel 'Packet Loss Rate'" >> "$gnuplot_script7"
echo "set ylabel 'Jain index'" >> "$gnuplot_script7"
echo "plot '$lr_data_file_name3' using 1:4 with linespoints title 'Jain index'" >> "$gnuplot_script7"

# Run the Gnuplot script
gnuplot "$gnuplot_script7"

echo "Working on NewReno vs HighSpeed Throughput vs Bottleneck Rate"

# Node part
br_data_file_name3="offline-2-1-output/NewRenoVsHighSpeed_tpVsbtl.dat"

x=1
max_btl_rate=300  # Correct variable name
increment=50

while [ "$x" -le "$max_btl_rate" ]
do
    output=$(./ns3 run --quiet "scratch/1905077.cc" -- --btlrate="$x" --choice=2)

    echo "$x $output" >> "$br_data_file_name3"

    x=$((x + increment))
done

output_png8="offline-2-1-output/NewRenoVsHighSpeed_tpVsbtl.png"
gnuplot_script8="offline-2-1-output/NewRenoVsHighSpeed_tpVsbtl_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script8"
echo "set output '$output_png8'" >> "$gnuplot_script8"
echo "set xlabel 'Bottleneck Link Rate'" >> "$gnuplot_script8"
echo "set ylabel 'Throughput'" >> "$gnuplot_script8"
echo "plot '$br_data_file_name3' using 1:2 with linespoints title 'New Reno', '$br_data_file_name3' using 1:3 with linespoints title 'HighSpeed'" >> "$gnuplot_script8"

# Run the Gnuplot script
gnuplot "$gnuplot_script8"


ji_file_name="offline-2-1-output/NewRenoVsHighSpeed_jainVsbtl.png"
gnuplot_script8="offline-2-1-output/NewRenoVsHighSpeed_jainVsbtl_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script8"
echo "set output '$ji_file_name'" >> "$gnuplot_script8"
echo "set xlabel 'Bottleneck Link Rate'" >> "$gnuplot_script8"
echo "set ylabel 'Jain index'" >> "$gnuplot_script8"
echo "plot '$br_data_file_name3' using 1:4 with linespoints title 'Jain index'" >> "$gnuplot_script8"

# Run the Gnuplot script
gnuplot "$gnuplot_script8"





echo "Working on NewReno vs WestwoodPlus Congestion Window"

./ns3 run --quiet "scratch/1905077.cc" -- --choice=1

output_png6="scratch/cwnd/NewRenoVsWestwoodPlus_cwnd.png"

cwnd_data_file_name5="scratch/cwnd/NewReno_cwnd_vs_t.dat"
cwnd_data_file_name6="scratch/cwnd/WestwoodPlus_cwnd_vs_t.dat"

gnuplot_script6="scratch/NewRenoVsWestwoodPlus_cwnd_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script6"
echo "set output '$output_png6'" >> "$gnuplot_script6"
echo "set xlabel 'Time'" >> "$gnuplot_script6"
echo "set ylabel 'Congestion Window'" >> "$gnuplot_script6"
echo "plot '$cwnd_data_file_name5' using 1:2 with linespoints title 'NewReno', '$cwnd_data_file_name6' using 1:2 with linespoints title 'WestwoodPlus'" >> "$gnuplot_script6"

# Run the Gnuplot script
gnuplot "$gnuplot_script6"


echo "Working on NewReno vs WestwoodPlus Throughput vs Packet Loss Rate"
#node part
lr_data_file_name3="offline-2-1-output/NewRenoVsWestwoodPlus_tpVslr.dat"

# Define the range of x-axis values
x_values=(-6 -5 -4 -3 -2)

# Compile and run your .cc file for each x value
for x in "${x_values[@]}"; do
    
    output=$(./ns3 run --quiet "scratch/1905077.cc" -- --lossrate="$x" --choice=1)
    echo "$output"
    echo "$x $output" >> "$lr_data_file_name3"
done

output_png7="offline-2-1-output/NewRenoVsWestwoodPlus_tpVslr.png"
gnuplot_script7="offline-2-1-output/NewRenoVsWestwoodPlus_tpVslr_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script7"
echo "set output '$output_png7'" >> "$gnuplot_script7"
echo "set xlabel 'Packet Loss Rate'" >> "$gnuplot_script7"
echo "set ylabel 'Throughput'" >> "$gnuplot_script7"
echo "plot '$lr_data_file_name3' using 1:2 with linespoints title 'NewReno', '$lr_data_file_name3' using 1:3 with linespoints title 'WestwoodPlus'" >> "$gnuplot_script7"

# Run the Gnuplot script
gnuplot "$gnuplot_script7"

ji_file_name="offline-2-1-output/NewRenoVsWestwood_jainVslr.png"
gnuplot_script7="offline-2-1-output/NewRenoVsWestwood_jainVslr_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script7"
echo "set output '$ji_file_name'" >> "$gnuplot_script7"
echo "set xlabel 'Packet Loss Rate'" >> "$gnuplot_script7"
echo "set ylabel 'Jain index'" >> "$gnuplot_script7"
echo "plot '$lr_data_file_name3' using 1:4 with linespoints title 'Jain index'" >> "$gnuplot_script7"

# Run the Gnuplot script
gnuplot "$gnuplot_script7"

echo "Working on NewReno vs WestwoodPlus Throughput vs Bottleneck Rate"

# Node part
br_data_file_name3="offline-2-1-output/NewRenoVsWestwoodPlus_tpVsbtl.dat"

x=1
max_btl_rate=300  # Correct variable name
increment=50

while [ "$x" -le "$max_btl_rate" ]
do
    output=$(./ns3 run --quiet "scratch/1905077.cc" -- --btlrate="$x" --choice=1)

    echo "$x $output" >> "$br_data_file_name3"

    x=$((x + increment))
done

output_png8="offline-2-1-output/NewRenoVsWestwoodPlus_tpVsbtl.png"
gnuplot_script8="offline-2-1-output/NewRenoVsWestwoodPlus_tpVsbtl_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script8"
echo "set output '$output_png8'" >> "$gnuplot_script8"
echo "set xlabel 'Bottleneck Link Rate'" >> "$gnuplot_script8"
echo "set ylabel 'Throughput'" >> "$gnuplot_script8"
echo "plot '$br_data_file_name3' using 1:2 with linespoints title 'New Reno', '$br_data_file_name3' using 1:3 with linespoints title 'WestwoodPlus'" >> "$gnuplot_script8"

# Run the Gnuplot script
gnuplot "$gnuplot_script8"

ji_file_name="offline-2-1-output/NewRenoVsWestwood_jainVsbtl.png"
gnuplot_script8="offline-2-1-output/NewRenoVsWestwood_jainVsbtl_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script8"
echo "set output '$ji_file_name'" >> "$gnuplot_script8"
echo "set xlabel 'Bottleneck Link Rate'" >> "$gnuplot_script8"
echo "set ylabel 'Jain index'" >> "$gnuplot_script8"
echo "plot '$br_data_file_name3' using 1:4 with linespoints title 'Jain index'" >> "$gnuplot_script8"

# Run the Gnuplot script
gnuplot "$gnuplot_script8"









echo "Working on NewReno vs AdaptiveReno Congestion Window"

./ns3 run --quiet "scratch/1905077.cc" -- --choice=3

output_png6="scratch/cwnd/NewRenoVsAdaptiveReno_cwnd.png"

cwnd_data_file_name5="scratch/cwnd/NewReno_cwnd_vs_t.dat"
cwnd_data_file_name6="scratch/cwnd/AdaptiveReno_cwnd_vs_t.dat"

gnuplot_script6="scratch/NewRenoVsAdaptiveReno_cwnd_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script6"
echo "set output '$output_png6'" >> "$gnuplot_script6"
echo "set xlabel 'Time'" >> "$gnuplot_script6"
echo "set ylabel 'Congestion Window'" >> "$gnuplot_script6"
echo "plot '$cwnd_data_file_name5' using 1:2 with linespoints title 'NewReno', '$cwnd_data_file_name6' using 1:2 with linespoints title 'AdaptiveReno'" >> "$gnuplot_script6"

# Run the Gnuplot script
gnuplot "$gnuplot_script6"


echo "Working on NewReno vs AdaptiveReno Throughput vs Packet Loss Rate"
#node part
lr_data_file_name3="offline-2-1-output/NewRenoVsAdaptiveReno_tpVslr.dat"

# Define the range of x-axis values
x_values=(-6 -5 -4 -3 -2)

# Compile and run your .cc file for each x value
for x in "${x_values[@]}"; do
    
    output=$(./ns3 run --quiet "scratch/1905077.cc" -- --lossrate="$x" --choice=3)
    echo "$output"
    echo "$x $output" >> "$lr_data_file_name3"
done

output_png7="offline-2-1-output/NewRenoVsAdaptiveReno_tpVslr.png"
gnuplot_script7="offline-2-1-output/NewRenoVsAdaptiveReno_tpVslr_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script7"
echo "set output '$output_png7'" >> "$gnuplot_script7"
echo "set xlabel 'Packet Loss Rate'" >> "$gnuplot_script7"
echo "set ylabel 'Throughput'" >> "$gnuplot_script7"
echo "plot '$lr_data_file_name3' using 1:2 with linespoints title 'NewReno', '$lr_data_file_name3' using 1:3 with linespoints title 'AdaptiveReno'" >> "$gnuplot_script7"

# Run the Gnuplot script
gnuplot "$gnuplot_script7"


ji_file_name="offline-2-1-output/NewRenoVsAreno_jainVslr.png"
gnuplot_script7="offline-2-1-output/NewRenoVsAreno_jainVslr_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script7"
echo "set output '$ji_file_name'" >> "$gnuplot_script7"
echo "set xlabel 'Packet Loss Rate'" >> "$gnuplot_script7"
echo "set ylabel 'Jain index'" >> "$gnuplot_script7"
echo "plot '$lr_data_file_name3' using 1:4 with linespoints title 'Jain index'" >> "$gnuplot_script7"

# Run the Gnuplot script
gnuplot "$gnuplot_script7"

echo "Working on NewReno vs AdaptiveReno Throughput vs Bottleneck Rate"

# Node part
br_data_file_name3="offline-2-1-output/NewRenoVsAdaptiveReno_tpVsbtl.dat"

x=1
max_btl_rate=300  # Correct variable name
increment=50

while [ "$x" -le "$max_btl_rate" ]
do
    output=$(./ns3 run --quiet "scratch/1905077.cc" -- --btlrate="$x" --choice=3)

    echo "$x $output" >> "$br_data_file_name3"

    x=$((x + increment))
done

output_png8="offline-2-1-output/NewRenoVsAdaptiveReno_tpVsbtl.png"
gnuplot_script8="offline-2-1-output/NewRenoVsAdaptiveReno_tpVsbtl_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script8"
echo "set output '$output_png8'" >> "$gnuplot_script8"
echo "set xlabel 'Bottleneck Link Rate'" >> "$gnuplot_script8"
echo "set ylabel 'Throughput'" >> "$gnuplot_script8"
echo "plot '$br_data_file_name3' using 1:2 with linespoints title 'New Reno', '$br_data_file_name3' using 1:3 with linespoints title 'AdaptiveReno'" >> "$gnuplot_script8"

# Run the Gnuplot script
gnuplot "$gnuplot_script8"

ji_file_name="offline-2-1-output/NewRenoVsAreno_jainVsbtl.png"
gnuplot_script8="offline-2-1-output/NewRenoVsAreno_jainVsbtl_plot_script.gp"

# Generate the Gnuplot script
echo "set term png" > "$gnuplot_script8"
echo "set output '$ji_file_name'" >> "$gnuplot_script8"
echo "set xlabel 'Bottleneck Link Rate'" >> "$gnuplot_script8"
echo "set ylabel 'Jain index'" >> "$gnuplot_script8"
echo "plot '$br_data_file_name3' using 1:4 with linespoints title 'Jain index'" >> "$gnuplot_script8"

# Run the Gnuplot script
gnuplot "$gnuplot_script8"

