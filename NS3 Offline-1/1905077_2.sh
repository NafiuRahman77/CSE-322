mkdir "offline-2-2-output"

#node part
node_data_file_name="offline-2-2-output/node.dat"
current_node=20
max_node=100
increment=20

while [ "$current_node" -le "$max_node" ]
do
    echo "Working on num-station $current_node"

    temp_flow=$((current_node/2))

    output=$(./ns3 run --quiet "scratch/1905077_2.cc" -- --num-station="$current_node" --num-flow="$temp_flow")

    echo "$current_node $output" >> "$node_data_file_name"

    current_node=$((current_node + increment))
done

node_vs_throughput_png="offline-2-2-output/node-vs-throughput.png"
node_vs_ratio_png="offline-2-2-output/node-vs-ratio.png"

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
flow_data_file_name="offline-2-2-output/flow.dat"
current_flow=10
max_flow=50
increment=10

while [ "$current_flow" -le "$max_flow" ]
do
    echo "Working on num-flow $current_flow"

    output=$(./ns3 run --quiet "scratch/1905077_2.cc" -- --num-flow="$current_flow")

    echo "$current_flow $output" >> "$flow_data_file_name"

    current_flow=$((current_flow + increment))
done

flow_vs_throughput_png="offline-2-2-output/flow-vs-throughput.png"
flow_vs_ratio_png="offline-2-2-output/flow-vs-ratio.png"

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

packet_rate_data_file_name="offline-2-2-output/packet_rate.dat"
current_packet_rate=100
max_packet_rate=500
increment=100

while [ "$current_packet_rate" -le "$max_packet_rate" ]
do
    echo "Working on packet_rate $current_packet_rate"

    output=$(./ns3 run --quiet "scratch/1905077_2.cc" -- --packet-rate="$current_packet_rate")

    echo "$current_packet_rate $output" >> "$packet_rate_data_file_name"

    current_packet_rate=$((current_packet_rate + increment))
done

packet_rate_vs_throughput_png="offline-2-2-output/packet_rate-vs-throughput.png"
packet_rate_vs_ratio_png="offline-2-2-output/packet_rate-vs-ratio.png"

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

#speed 
speed_data_file_name="offline-2-2-output/speed.dat"
current_speed=5
max_speed=25
increment=5

while [ "$current_speed" -le "$max_speed" ]
do
    echo "Working on speed $current_speed "

    output=$(./ns3 run --quiet "scratch/1905077_2.cc" -- --speed="$current_speed")

    echo "$current_speed $output" >> "$speed_data_file_name"

    current_speed=$((current_speed + increment))
done

speed_vs_throughput_png="offline-2-2-output/speed-vs-throughput.png"
speed_vs_ratio_png="offline-2-2-output/speed-vs-ratio.png"

speed_vs_throughput_script=$(cat << EOL
set terminal png size 640, 480
set output "$speed_vs_throughput_png"
set xlabel "Speed"
set ylabel "Throughput (Mbit/s)"
plot "$speed_data_file_name" using 1:2 title "speed vs Throughput" with linespoints
EOL
)
speed_vs_ratio_script=$(cat << EOL
set terminal png size 640, 480
set output "$speed_vs_ratio_png"
set xlabel "Speed"
set ylabel "Ratio"
plot "$speed_data_file_name" using 1:3 title "speed vs Ratio" with linespoints
EOL
)

echo "$speed_vs_throughput_script" | gnuplot
echo "$speed_vs_ratio_script" | gnuplot

echo "Plots made for speeds"
