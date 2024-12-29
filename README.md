# README: CSE 322 - Computer Networks Sessional

## Table of Contents
1. [Overview](#overview)
2. [Offline 1: Socket Programming](#offline-1-socket-programming)
3. [Offline 2: NS3 Introduction](#offline-2-ns3-introduction)
4. [Offline 3: Congestion Control Algorithm (TCP Adaptive Reno)](#offline-3-congestion-control-algorithm-tcp-adaptive-reno)
5. [Offline 4: Error Detection and Correction](#offline-4-error-detection-and-correction)

---

## Overview
This repository contains sessional projects and assignments completed as part of the **CSE 322: Computer Networks Sessional** course. Each offline session focuses on hands-on applications of theoretical concepts related to computer networks.

---

## Offline 1: Socket Programming
### Files:
- **Client.cpp**
- **Server.cpp**

### Objective:
Implement client-server communication using sockets in C++.
### Files:
Under the `src` folder:
- **Client.java**
- **Server.java**
- **Worker.java**

### Tasks:
1. Establish a connection between a client and a server.
2. Send and receive messages.
3. Handle errors and ensure reliability.

### How to Run:
1. Compile the Java files:
   ```bash
   javac src/*.java
   ```
2. Run the server:
   ```bash
    java -cp src Server
    ```
3. Run the client:
    ```bash
     java -cp src Client
     ```
4. Follow the on-screen instructions to send and receive messages.

---

## Offline 2: NS3 Introduction
### Files:
- **1905077.cc**
- **1905077.sh**
- **Graphs** folder

### Objective:
Learn the basics of NS3 by simulating network topologies and analyzing their behavior.

### Tasks:
1. Simulate a basic point-to-point network with NS3.
2. Analyze throughput, delay, and packet loss.
3. Generate and save graphs for the analysis.

### How to Run:
1. Install NS3 and set up the environment from the official documentation.
2. Run the simulation script:
   ```bash
   ./1905077.sh
   ```
3. View the generated graphs in the `offline-2-1-output` and `offline-2-2-output` folders.

---

## Offline 3: Congestion Control Algorithm (TCP Adaptive Reno)
### Files:
- **tcp-adaptive-reno.cc**
- **tcp-adaptive-reno.h**
- **1905077.cc**
- **1905077.sh**
- **Graphs** folder

### Objective:
Implement a new congestion control algorithm, **TCP Adaptive Reno**, in NS3 and evaluate its performance.

### Tasks:
#### 1. Network Topology Design:
- Create a dumbbell topology using `PointToPointDumbbellHelper`.
- Use two flows with different congestion control algorithms (e.g., TCP NewReno and TCP WestwoodPlus).
- Configure:
  - DataRate: 1 Gbps
  - Delay: 1 ms for sender/receiver links, 100 ms for bottleneck link.
  - Router buffer: Bandwidth delay product.
  - RateErrorModel for bottleneck link devices.

#### 2. Performance Evaluation:
- Measure throughput using `FlowMonitorHelper`.
- Generate plots:
  - Throughput vs Bottleneck Data Rate (1–300 Mbps).
  - Throughput vs Packet Loss Rate (10⁻²–10⁻⁶).
  - Congestion Window vs Time.

#### 3. Implementation of TCP Adaptive Reno:
- Modify `TcpWestwoodPlus` to create `TcpAdaptiveReno`:
  - Functions: `PktsAcked`, `EstimateCongestionLevel`, `EstimateIncWnd`, `CongestionAvoidance`, `GetSsThresh`.
  - Dynamically adjust the congestion window based on RTT and congestion levels.
- Compare TCP Adaptive Reno with TCP HighSpeed and TCP NewReno.

#### 4. Bonus:
- Calculate Jain’s Fairness Index for co-existing flows.
- Generate plots:
  - Fairness Index vs Bottleneck Data Rate.
  - Fairness Index vs Packet Loss Rate.

### How to Run:
1. Install NS3 and set up the environment from the official documentation.
2. Run the simulation script:
   ```bash
   ./1905077.sh
   ```
3. View the generated graphs in the `Graphs` folder.

---

## Offline 4: Error Detection and Correction
### Files:
- **ErrorDetection.cpp**
### Objective:
Implement error detection using CRC and error correction using Hamming distance.

### Inputs:
1. Data string to be transmitted.
2. `m`: Number of bytes per row in the data block.
3. `p`: Probability of bit toggle during transmission.
4. Generator polynomial for CRC checksum.

### Tasks:
1. **Padding**: Ensure the data string length is a multiple of `m` by appending `~` if necessary.
2. **Data Block Creation**:
   - Convert characters to 8-bit ASCII binary.
   - Arrange data into rows of `m` bytes each.
3. **Add Check Bits**:
   - Use Hamming distance to allow correction of up to 1-bit error per row.
4. **Serialization**:
   - Serialize data in a column-major manner.
5. **CRC Checksum**:
   - Compute and append CRC checksum to the serialized data.
6. **Simulate Transmission**:
   - Toggle bits with probability `p` and identify errors.
7. **Error Detection**:
   - Verify received data using CRC.
8. **De-serialization**:
   - Restore data block in a column-major manner and highlight errors.
9. **Error Correction**:
   - Correct single-bit errors using Hamming distance.
10. **Output Data**:
    - Extract original data string from the corrected block.

### How to Run:
1. Compile the program:
    ```bash
    g++ ErrorDetection.cpp -o ErrorDetection
    ```
2. Run the program:
    ```bash
    ./ErrorDetection
    ```
3. Input the data string, `m`, `p`, and generator polynomial as required.
---


For any queries or further details, contact via email: [nafiu.rahman@gmail.com].

