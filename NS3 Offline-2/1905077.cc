#include "ns3/applications-module.h"
#include "ns3/callback.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/stats-module.h"

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace ns3;

/**
 * Tutorial - a simple Application sending packets.
 */
class TutorialApp : public Application
{
  public:
    TutorialApp();
    ~TutorialApp() override;

    /**
     * Register this type.
     * \return The TypeId.
     */
    static TypeId GetTypeId();

    /**
     * Setup the socket.
     * \param socket The socket.
     * \param address The destination address.
     * \param packetSize The packet size to transmit.
     * \param nPackets The number of packets to transmit.
     * \param dataRate the data rate to use.
     */
    void Setup(Ptr<Socket> socket,
               Address address,
               uint32_t packetSize,
               uint32_t nPackets,
               DataRate dataRate);

  private:
    void StartApplication() override;
    void StopApplication() override;

    /// Schedule a new transmission.
    void ScheduleTx();
    /// Send a packet.
    void SendPacket();

    Ptr<Socket> m_socket;   //!< The transmission socket.
    Address m_peer;         //!< The destination address.
    uint32_t m_packetSize;  //!< The packet size.
    uint32_t m_nPackets;    //!< The number of packets to send.
    DataRate m_dataRate;    //!< The data rate to use.
    EventId m_sendEvent;    //!< Send event.
    bool m_running;         //!< True if the application is running.
    uint32_t m_packetsSent; //!< The number of packets sent.
};

TutorialApp::TutorialApp()
    : m_socket(nullptr),
      m_peer(),
      m_packetSize(0),
      m_nPackets(0),
      m_dataRate(0),
      m_sendEvent(),
      m_running(false),
      m_packetsSent(0)
{
}

TutorialApp::~TutorialApp()
{
    m_socket = nullptr;
}

/* static */
TypeId
TutorialApp::GetTypeId()
{
    static TypeId tid = TypeId("TutorialApp")
                            .SetParent<Application>()
                            .SetGroupName("Tutorial")
                            .AddConstructor<TutorialApp>();
    return tid;
}

void
TutorialApp::Setup(Ptr<Socket> socket,
                   Address address,
                   uint32_t packetSize,
                   uint32_t nPackets,
                   DataRate dataRate)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}

void
TutorialApp::StartApplication()
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
TutorialApp::StopApplication()
{
    m_running = false;

    if (m_sendEvent.IsRunning())
    {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket)
    {
        m_socket->Close();
    }
}

void
TutorialApp::SendPacket()
{
    Ptr<Packet> packet = Create<Packet>(m_packetSize);
    m_socket->Send(packet);

    if (++m_packetsSent < m_nPackets)
    {
        ScheduleTx();
    }
}

void
TutorialApp::ScheduleTx()
{
    if (m_running)
    {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &TutorialApp::SendPacket, this);
    }
}

static void
CwndChange1(Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
    *stream->GetStream() << Simulator::Now().GetSeconds() << " " << newCwnd << std::endl;
}

static void
CwndChange2(Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)

{
    *stream->GetStream() << Simulator::Now().GetSeconds() << " " << newCwnd << std::endl;
}

int
main(int argc, char* argv[])
{
    uint32_t payloadSize = 1024;
    int packet_loss_exp = -6;
    int bttlnkRate = 50;
    int choice = 1;
    CommandLine cmd(__FILE__);
    cmd.AddValue("lossrate", "Packet loss rate", packet_loss_exp);
    cmd.AddValue("btlrate", "Bottle link rate", bttlnkRate);
    cmd.AddValue("choice", "Choose model", choice);

    cmd.Parse(argc, argv);
    //ns3::Time::SetResolution(ns3::Time::NS);
    // std::cout<<"bop"<<choice<<std::endl;
    //  std::cout<<" plrate"<<packet_loss_exp<<std::endl;
    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(payloadSize));

    PointToPointHelper p2p_leaf_helper;

    // std::string file = "offline-2-1-output/cong1.dat";

    p2p_leaf_helper.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
    p2p_leaf_helper.SetChannelAttribute("Delay", StringValue("1ms"));

    ;                           // 1 Gbps in bps
    double bttlnkDelay = 100000; // 100 ms in seconds
    // check later
    p2p_leaf_helper.SetQueue(
        "ns3::DropTailQueue",
        "MaxSize",
        StringValue(std::to_string((100000* bttlnkRate) / payloadSize/8) + "p"));

    PointToPointHelper p2p_bottleneck_helper;

    p2p_bottleneck_helper.SetDeviceAttribute("DataRate",
                                             StringValue(std::to_string(bttlnkRate) + "Mbps"));
    p2p_bottleneck_helper.SetChannelAttribute("Delay", StringValue("100ms"));

    PointToPointDumbbellHelper dumbell(2,
                                       p2p_leaf_helper,
                                       2,
                                       p2p_leaf_helper,
                                       p2p_bottleneck_helper);

    // Add a RateErrorModel to the bottleneck link devices
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue((1.0 / std::pow(10, -packet_loss_exp))));
    // dumbell.m_routerDevices.Get(0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    dumbell.m_routerDevices.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
    

    Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TcpNewReno::GetTypeId()));
    InternetStackHelper stack1;
    stack1.Install(dumbell.GetLeft(0));
    stack1.Install(dumbell.GetRight(0));
    stack1.Install(dumbell.GetLeft());
    stack1.Install(dumbell.GetRight());
    if (choice == 1)
    {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType",
                           TypeIdValue(TcpWestwoodPlus::GetTypeId()));
    }
    else if (choice == 2)
    {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType",
                           TypeIdValue(TcpHighSpeed::GetTypeId()));
    }
    else if (choice == 3)
    {
        Config::SetDefault("ns3::TcpL4Protocol::SocketType",
                           TypeIdValue(TcpAdaptiveReno::GetTypeId()));
    }
    // InternetStackHelper stack2;
    stack1.Install(dumbell.GetLeft(1));
    stack1.Install(dumbell.GetRight(1));

    dumbell.AssignIpv4Addresses(Ipv4AddressHelper("10.1.1.0", "255.255.255.0"),  // left nodes
                                Ipv4AddressHelper("10.2.1.0", "255.255.255.0"),  // right nodes
                                Ipv4AddressHelper("10.3.1.0", "255.255.255.0")); // routers

    Ipv4GlobalRoutingHelper::PopulateRoutingTables(); // populate routing table
    

    uint16_t sp = 8080;
    ApplicationContainer senderApps;
    ApplicationContainer receiverApps;

    Ptr<Socket> ns3TcpSocket1 =
        Socket::CreateSocket(dumbell.GetLeft(0), TcpSocketFactory::GetTypeId());
    Ptr<TutorialApp> app1 = CreateObject<TutorialApp>();
    Address sinkaddr1(InetSocketAddress(dumbell.GetRightIpv4Address(0), sp));
    app1->Setup(ns3TcpSocket1, sinkaddr1, payloadSize, 3000000, DataRate("1Gbps"));
    dumbell.GetLeft(0)->AddApplication(app1);
    senderApps.Add(app1);

    AsciiTraceHelper asciiTraceHelper1;
    Ptr<OutputStreamWrapper> stream1 =
        asciiTraceHelper1.CreateFileStream("scratch/cwnd/NewReno_cwnd_vs_t.dat");
    ns3TcpSocket1->TraceConnectWithoutContext("CongestionWindow",
                                              MakeBoundCallback(&CwndChange1, stream1));
    // ns3TcpSocket1->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndChange1));

    Ptr<Socket> ns3TcpSocket2 =
        Socket::CreateSocket(dumbell.GetLeft(1), TcpSocketFactory::GetTypeId());
    Ptr<TutorialApp> app2 = CreateObject<TutorialApp>();
    Address sinkaddr2(InetSocketAddress(dumbell.GetRightIpv4Address(1), sp));
    app2->Setup(ns3TcpSocket2, sinkaddr2, payloadSize, 3000000, DataRate("1Gbps"));
    dumbell.GetLeft(1)->AddApplication(app2);
    senderApps.Add(app2);
    std::ostringstream os2;

    AsciiTraceHelper asciiTraceHelper2;
    Ptr<OutputStreamWrapper> stream2;

    if (choice == 1)
    {
        stream2 = asciiTraceHelper2.CreateFileStream("scratch/cwnd/WestwoodPlus_cwnd_vs_t.dat");
    }
    else if (choice == 2)
    {
        stream2 = asciiTraceHelper2.CreateFileStream("scratch/cwnd/HighSpeed_cwnd_vs_t.dat");
    }
    else if (choice == 3)
    {
        stream2 = asciiTraceHelper2.CreateFileStream("scratch/cwnd/AdaptiveReno_cwnd_vs_t.dat");
    }
    ns3TcpSocket2->TraceConnectWithoutContext("CongestionWindow",
                                              MakeBoundCallback(&CwndChange1, stream2));

    senderApps.Start(Seconds(1.0));
    senderApps.Stop(Seconds(60.0));

    PacketSinkHelper packetSinkHelper1("ns3::TcpSocketFactory",
                                       InetSocketAddress(Ipv4Address::GetAny(), sp));
    receiverApps.Add(packetSinkHelper1.Install(dumbell.GetRight(0)));

    PacketSinkHelper packetSinkHelper2("ns3::TcpSocketFactory",
                                       InetSocketAddress(Ipv4Address::GetAny(), sp));
    receiverApps.Add(packetSinkHelper2.Install(dumbell.GetRight(1)));
    receiverApps.Start(Seconds(0.0));
    receiverApps.Stop(Seconds(60.0));

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    Simulator::Stop(Seconds(62));
    Simulator::Run();

    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
    double th1 = 0.0;
    double th2 = 0.0;
    double jain_up = 0;
    double jain_down = 0;
    int iter = 0;
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end();
         ++i)
    {
        double temp = i->second.rxBytes * 8.0 / 59.0 / 1000;
        if (iter % 2 == 0)
        {
            th1 += temp;
        }
        else
        {
            th2 += temp;
        }
        jain_up+=temp;
        jain_down+=std::pow(temp,2);
        iter++;
    }
    double jain_index=std::pow(jain_up,2)/(iter*jain_down);
    std::cout << th1 << " " << th2 << " "<<jain_index<<std::endl;

    Simulator::Destroy();
    return 0;
}
