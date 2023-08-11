#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/point-to-point-module.h"

int packetSize=1024;
int delay=100;
int txRange=5;
double L = 5 * txRange;
int start_time = 0;
int packet_sent = 0;
int packet_received = 0;
int bits_sent = 0;
int bits_received = 0;
double throughput = 0.0;
double ratio = 0.0;


using namespace ns3;
void packet_sent_function(Ptr<const ns3::Packet> packet_ptr)
{
   packet_sent++;
  
}
void packet_receive_function(Ptr<const ns3::Packet> packet_ptr, const Address &address)
{
   packet_received++;
   int sz=packet_ptr->GetSize();
   bits_received+=sz*8;
}
void timer(){
    double final_time=Simulator::Now().ToDouble(Time::S);
    throughput = (bits_received)/ (final_time)/1e6 ;
    ratio = (double)packet_received / packet_sent;
    
    Simulator::Schedule(ns3::MilliSeconds(delay), timer);
}

int main(int argc, char *argv[]) {

    // LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);
    // LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

    int num_station = 20;
    int num_flow = 10;
    int packet_rate = 100;
    int speed = 5;
   

    CommandLine cmd(__FILE__);

    cmd.AddValue("num-station", "Set number of sender and reciever stations", num_station);
    cmd.AddValue("num-flow", "Set number of data packets to be sent", num_flow);
    cmd.AddValue("packet-rate", "Set number of packets to be sent per second", packet_rate);
    cmd.AddValue("speed", "Set speed of nodes", speed);
    //cmd.AddValue("verbose", "Enable verbose mode", verbose);

    cmd.Parse(argc, argv);
    Time::SetResolution(Time::NS);

    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(packetSize));

    NodeContainer p2pNodes;
    p2pNodes.Create(2);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);

    NodeContainer wifiStaNodesLeft;
    wifiStaNodesLeft.Create(num_station/2);
    NodeContainer wifiApNodeLeft = p2pNodes.Get(0);

    NodeContainer wifiStaNodesRight;
    wifiStaNodesRight.Create(num_station/2);
    NodeContainer wifiApNodeRight = p2pNodes.Get(1);
    
    //Left Part
    YansWifiChannelHelper channelLeft = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phyLeft;
    phyLeft.SetChannel(channelLeft.Create()); // share the same wireless medium 

    WifiMacHelper macLeft;
    Ssid ssidLeft = Ssid("ns-3-ssidLeft"); // creates an 802.11 service set identifier (SSID) 

    WifiHelper wifiLeft;

    NetDeviceContainer staDevicesLeft;
    macLeft.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssidLeft), "ActiveProbing", BooleanValue(false));
    staDevicesLeft = wifiLeft.Install(phyLeft, macLeft, wifiStaNodesLeft);

    NetDeviceContainer apDevicesLeft;
    macLeft.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssidLeft));
    apDevicesLeft = wifiLeft.Install(phyLeft, macLeft, wifiApNodeLeft);

    //Right Part
    YansWifiChannelHelper channelRight = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phyRight;
    phyRight.SetChannel(channelRight.Create()); // share the same wireless medium 

    WifiMacHelper macRight;
    Ssid ssidRight = Ssid("ns-3-ssidRight"); // creates an 802.11 service set identifier (SSID) 

    WifiHelper wifiRight;

    NetDeviceContainer staDevicesRight;
    macRight.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssidRight), "ActiveProbing", BooleanValue(false));
    staDevicesRight = wifiRight.Install(phyRight, macRight, wifiStaNodesRight);

    NetDeviceContainer apDevicesRight;
    macRight.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssidRight));
    apDevicesRight = wifiRight.Install(phyRight, macRight, wifiApNodeRight);

    MobilityHelper mobility_helper;
   
    mobility_helper.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    Ptr<ListPositionAllocator> positions = CreateObject<ListPositionAllocator>();
    Vector leftApVect(0.0, 0.0, 0.0);
    Vector rightApVect(400, 0.0, 0.0);
    positions->Add(leftApVect);
    positions->Add(rightApVect);
    mobility_helper.SetPositionAllocator(positions);
    mobility_helper.Install(p2pNodes);

    Ptr<ns3::NormalRandomVariable> radius =
    CreateObject<ns3::NormalRandomVariable>();
    radius->SetAttribute("Mean", DoubleValue(0.0));
    radius->SetAttribute("Bound", DoubleValue(5*txRange));
    radius->SetAttribute("Variance", DoubleValue(5*txRange));

    mobility_helper.SetPositionAllocator("ns3::RandomDiscPositionAllocator", "X", DoubleValue(0.0), "Y",DoubleValue(0.0), "Rho", PointerValue(radius));
    mobility_helper.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(ns3::Rectangle(-L, L, -L, L)), "Speed", ns3::StringValue("ns3::ConstantRandomVariable[Constant=" + std::to_string(speed) + "]"));

    for(uint32_t i=0;i<wifiStaNodesLeft.GetN();i++){
        mobility_helper.Install(wifiStaNodesLeft.Get(i));
    }
   
    mobility_helper.SetPositionAllocator("ns3::RandomDiscPositionAllocator", "X", DoubleValue(400),"Y", DoubleValue(0.0), "Rho", PointerValue(radius));
    mobility_helper.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(ns3::Rectangle(-L+400, L+400, -L, L)), "Speed", ns3::StringValue("ns3::ConstantRandomVariable[Constant=" + std::to_string(speed) + "]"));
    for(uint32_t i=0;i<wifiStaNodesRight.GetN();i++){
        mobility_helper.Install(wifiStaNodesRight.Get(i));
    }
    
    
    InternetStackHelper stack;
    stack.Install(wifiApNodeRight);
    stack.Install(wifiStaNodesRight);
    stack.Install(wifiApNodeLeft);
    stack.Install(wifiStaNodesLeft);

    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);

    address.SetBase("10.1.2.0", "255.255.255.0");
    address.Assign(staDevicesLeft);
    address.Assign(apDevicesLeft);

    address.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer staInterfacesRight;
    staInterfacesRight= address.Assign(staDevicesRight);
    address.Assign(apDevicesRight);

    ApplicationContainer sender_apps;
    ApplicationContainer receiver_apps;
    
    int station_each_side=num_station/2;

    for(int i = 0; i < num_flow; i++)
    {
        OnOffHelper sender_helper("ns3::TcpSocketFactory", InetSocketAddress(staInterfacesRight.GetAddress(i % (station_each_side)), 9));

        sender_helper.SetAttribute("PacketSize", UintegerValue(packetSize));
        sender_helper.SetAttribute("DataRate", DataRateValue(ns3::DataRate(packet_rate * packetSize*8)));
        sender_apps.Add(sender_helper.Install(wifiStaNodesLeft.Get(i % (station_each_side))));
    }

    for(int i = 0; i < station_each_side; i++)
    {
        PacketSinkHelper receiver_helper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 9));

        receiver_apps.Add(receiver_helper.Install(wifiStaNodesRight.Get(i)));
    }
   
    for(uint32_t i = 0; i < sender_apps.GetN(); i++)
    {
       Ptr<Application> sender_app = sender_apps.Get(i);
       Callback<void, Ptr<const Packet>> callback = MakeCallback(packet_sent_function);
       sender_app->TraceConnectWithoutContext("Tx", callback);
    
    }
    for(uint32_t i = 0; i < receiver_apps.GetN(); i++)
    {
       Ptr<Application> receiver_app = receiver_apps.Get(i);
       Callback<void, Ptr<const Packet>,const Address& > callback = MakeCallback(packet_receive_function);
       receiver_app->TraceConnectWithoutContext("Rx", callback);
    
    }
    
    sender_apps.Start(Seconds(1));
    receiver_apps.Start(Seconds(0));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Schedule(MilliSeconds(delay), timer);
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();

   
    std::cout << throughput << " " << ratio << std::endl;
    return 0;
}
