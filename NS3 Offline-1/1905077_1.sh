mkdir "offline-2-1-output"

#node part
node_data_file_name="offline-2-1-output/node.dat"
current_node=20
max_node=100
increment=20

while [ "$current_node" -le "$max_node" ]
do
    echo "Working on num-station $current_node"

    temp_flow=$((current_node/2))

    output=$(./ns3 run --quiet "scratch/1905077_1.cc" -- --num-station="$current_node" --num-flow="$temp_flow")

    echo "$current_node $output" >> "$node_data_file_name"

    current_node=$((current_node + increment))
done

node_vs_throughput_png="offline-2-1-output/node-vs-throughput.png"
node_vs_ratio_png="offline-2-1-output/node-vs-ratio.png"

node_vs_throughput_script=$(cat << EOL
set terminal png size 640, 480
set output "$node_vs_throughput_png"
set xlabel "Node"
set ylabel "Throughput (Mbit/s)"
plot "$node_data_file_name" using 1:2 title "Node vs Throughput" with linespoints
EOL
)
node_vs_ratio_script=$(cat << EOL
set terminal png size 640, 480
set output "$node_vs_ratio_png"
set xlabel "Node"
set ylabel "Ratio"
plot "$node_data_file_name" using 1:3 title "Node vs Ratio" with linespoints
EOL
)

echo "$node_vs_throughput_script" | gnuplot
echo "$node_vs_ratio_script" | gnuplot

echo "Plots made for nodes"

#flow part
flow_data_file_name="offline-2-1-output/flow.dat"
current_flow=10
max_flow=50
increment=10

while [ "$current_flow" -le "$max_flow" ]
do
    echo "Working on num-flow $current_flow"

    output=$(./ns3 run --quiet "scratch/1905077_1.cc" -- --num-flow="$current_flow")

    echo "$current_flow $output" >> "$flow_data_file_name"

    current_flow=$((current_flow + increment))
done

flow_vs_throughput_png="offline-2-1-output/flow-vs-throughput.png"
flow_vs_ratio_png="offline-2-1-output/flow-vs-ratio.png"

flow_vs_throughput_script=$(cat << EOL
set terminal png size 640, 480
set output "$flow_vs_throughput_png"
set xlabel "Flow"
set ylabel "Throughput (Mbit/s)"
plot "$flow_data_file_name" using 1:2 title "flow vs Throughput" with linespoints
EOL
)
flow_vs_ratio_script=$(cat << EOL
set terminal png size 640, 480
set output "$flow_vs_ratio_png"
set xlabel "Flow"
set ylabel "Ratio"
plot "$flow_data_file_name" using 1:3 title "flow vs Ratio" with linespoints
EOL
)

echo "$flow_vs_throughput_script" | gnuplot
echo "$flow_vs_ratio_script" | gnuplot

echo "Plots made for flows"


#packet_rate

packet_rate_data_file_name="offline-2-1-output/packet_rate.dat"
current_packet_rate=100
max_packet_rate=500
increment=100

while [ "$current_packet_rate" -le "$max_packet_rate" ]
do
    echo "Working on packet_rate $current_packet_rate"

    output=$(./ns3 run --quiet "scratch/1905077_1.cc" -- --packet-rate="$current_packet_rate")

    echo "$current_packet_rate $output" >> "$packet_rate_data_file_name"

    current_packet_rate=$((current_packet_rate + increment))
done

packet_rate_vs_throughput_png="offline-2-1-output/packet_rate-vs-throughput.png"
packet_rate_vs_ratio_png="offline-2-1-output/packet_rate-vs-ratio.png"

packet_rate_vs_throughput_script=$(cat << EOL
set terminal png size 640, 480
set output "$packet_rate_vs_throughput_png"
set xlabel "Packet Rate"
set ylabel "Throughput (Mbit/s)"
plot "$packet_rate_data_file_name" using 1:2 title "packet_rate vs Throughput" with linespoints
EOL
)
packet_rate_vs_ratio_script=$(cat << EOL
set terminal png size 640, 480
set output "$packet_rate_vs_ratio_png"
set xlabel "Packet Rate"
set ylabel "Ratio"
plot "$packet_rate_data_file_name" using 1:3 title "packet_rate vs Ratio" with linespoints
EOL
)

echo "$packet_rate_vs_throughput_script" | gnuplot
echo "$packet_rate_vs_ratio_script" | gnuplot

echo "Plots made for packet_rates"

#coverage_area_factor 
coverage_area_factor_data_file_name="offline-2-1-output/coverage_area_factor.dat"
current_coverage_area_factor=1
max_coverage_area_factor=5
increment=1

while [ "$current_coverage_area_factor" -le "$max_coverage_area_factor" ]
do
    echo "Working on coverage_area_factor $current_coverage_area_factor "

    output=$(./ns3 run --quiet "scratch/1905077_1.cc" -- --coverage-area-factor="$current_coverage_area_factor")

    echo "$current_coverage_area_factor $output" >> "$coverage_area_factor_data_file_name"

    current_coverage_area_factor=$((current_coverage_area_factor + increment))
done

coverage_area_factor_vs_throughput_png="offline-2-1-output/coverage_area_factor-vs-throughput.png"
coverage_area_factor_vs_ratio_png="offline-2-1-output/coverage_area_factor-vs-ratio.png"

coverage_area_factor_vs_throughput_script=$(cat << EOL
set terminal png size 640, 480
set output "$coverage_area_factor_vs_throughput_png"
set xlabel "Coverage Area"
set ylabel "Throughput (Mbit/s)"
plot "$coverage_area_factor_data_file_name" using 1:2 title "coverage-area-factor vs Throughput" with linespoints
EOL
)
coverage_area_factor_vs_ratio_script=$(cat << EOL
set terminal png size 640, 480
set output "$coverage_area_factor_vs_ratio_png"
set xlabel "Coverage Area"
set ylabel "Ratio"
plot "$coverage_area_factor_data_file_name" using 1:3 title "coverage-area-factor vs Ratio" with linespoints
EOL
)

echo "$coverage_area_factor_vs_throughput_script" | gnuplot
echo "$coverage_area_factor_vs_ratio_script" | gnuplot

echo "Plots made for coverage_area_factors"
