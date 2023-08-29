#include "tcp-adaptive-reno.h"

#include "rtt-estimator.h"
#include "tcp-socket-base.h"

#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("TcpAdaptiveReno");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(TcpAdaptiveReno);

TypeId
TcpAdaptiveReno::GetTypeId(void)
{
    static TypeId tid =
        TypeId("ns3::TcpAdaptiveReno")
            .SetParent<TcpNewReno>()
            .SetGroupName("Internet")
            .AddConstructor<TcpAdaptiveReno>()
            .AddAttribute(
                "FilterType",
                "Use this to choose no filter or Tustin's approximation filter",
                EnumValue(TcpAdaptiveReno::TUSTIN),
                MakeEnumAccessor(&TcpAdaptiveReno::m_fType),
                MakeEnumChecker(TcpAdaptiveReno::NONE, "None", TcpAdaptiveReno::TUSTIN, "Tustin"))
            .AddTraceSource("EstimatedBW",
                            "The estimated bandwidth",
                            MakeTraceSourceAccessor(&TcpAdaptiveReno::m_currentBW),
                            "ns3::TracedValueCallback::Double");
    return tid;
}

TcpAdaptiveReno::TcpAdaptiveReno(void)
    : TcpWestwoodPlus(),
      m_rtt_min(Time(0)),
      m_rtt_curr(Time(0)),
      m_jpacketloss_rtt(Time(0)),
      m_jcong_rtt(Time(0)),
      m_jminus1cong_rtt(Time(0)),
      m_increment_wind(0),
      m_base_wind(0),
      m_probe_wind(0),
      a(0.85),
      scalingFactor(1000)
{
    NS_LOG_FUNCTION(this);
}

TcpAdaptiveReno::TcpAdaptiveReno(const TcpAdaptiveReno& sock)
    : TcpWestwoodPlus(sock),
      m_rtt_min(Time(0)),
      m_rtt_curr(Time(0)),
      m_jpacketloss_rtt(Time(0)),
      m_jcong_rtt(Time(0)),
      m_jminus1cong_rtt(Time(0)),
      m_increment_wind(0),
      m_base_wind(0),
      m_probe_wind(0),
      a(0.85),
      scalingFactor(1000)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_LOGIC("Invoked the copy constructor");
}

TcpAdaptiveReno::~TcpAdaptiveReno(void)
{
}

/*
The function is called every time an ACK is received (only one time
also for cumulative ACKs) and contains timing information
*/
void
TcpAdaptiveReno::PktsAcked(Ptr<TcpSocketState> tcb, uint32_t packetsAcked, const Time& rtt)
{
    NS_LOG_FUNCTION(this << tcb << packetsAcked << rtt);

    if (rtt.IsZero())
    {
        NS_LOG_WARN("RTT measured is zero!");
        return;
    }

    m_ackedSegments += packetsAcked;

    if (m_rtt_min.IsZero() || rtt <= m_rtt_min)
    {
        m_rtt_min = rtt;
    }

    m_rtt_curr = rtt;

    TcpWestwoodPlus::EstimateBW(rtt, tcb);
}

double
TcpAdaptiveReno::EstimateCongestionLevel()
{
    a = (m_jminus1cong_rtt < m_rtt_min) ? 0 : 0.85;

    m_jcong_rtt =
        ns3::Seconds(a * m_jminus1cong_rtt.GetSeconds() + (1 - a) * m_jpacketloss_rtt.GetSeconds());

    return std::min((double)(m_rtt_curr.GetSeconds() - m_rtt_min.GetSeconds()) /
                        (m_jcong_rtt.GetSeconds() - m_rtt_min.GetSeconds()),
                    1.0);
}

void
TcpAdaptiveReno::EstimateIncWnd(Ptr<TcpSocketState> tcb)
{
    double congestion = EstimateCongestionLevel();

    double bitRate = m_currentBW.Get().GetBitRate();
    double segmentSizeSquared = std::pow(tcb->m_segmentSize, 2);
    double m_maxIncWnd = bitRate * segmentSizeSquared / scalingFactor / 8.0;

    double alpha = 10.0;
    double term1 = 1.0 / alpha;
    double term2 = (1.0 / alpha + 1.0) / std::exp(alpha);
    double term3 = (1.0 / alpha + 0.5) / std::exp(alpha);

    double beta = 2 * m_maxIncWnd * (term1 - term2);
    double gamma = 1 - 2 * m_maxIncWnd * (term1 - term3);

    double incrementWind =
        (m_maxIncWnd / std::exp(alpha * congestion)) + (beta * congestion) + gamma;
    m_increment_wind = static_cast<int32_t>(incrementWind);
}

void
TcpAdaptiveReno::CongestionAvoidance(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);
    if (segmentsAcked <= 0)
    {
        return;
    }

    EstimateIncWnd(tcb);

    m_base_wind +=
        (uint32_t)(std::max((double)(std::pow(tcb->m_segmentSize, 2)) / tcb->m_cWnd.Get(), 1.0));

    m_probe_wind =
        std::max((double)(m_probe_wind + m_increment_wind / (int)tcb->m_cWnd.Get()), 0.0);

    tcb->m_cWnd.Set(m_base_wind + m_probe_wind);
}

uint32_t
TcpAdaptiveReno::GetSsThresh(Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight)
{
    m_jminus1cong_rtt = m_jcong_rtt;
    m_jpacketloss_rtt = m_rtt_curr;

    double congestion = EstimateCongestionLevel();

    double segmentSizeDoubled = 2*tcb->m_segmentSize;

    m_base_wind = std::max((uint32_t) segmentSizeDoubled, (uint32_t)(tcb->m_cWnd / (1.0 + congestion)));
    m_probe_wind = 0;

    return std::max((uint32_t) segmentSizeDoubled, (uint32_t)(tcb->m_cWnd / (1.0 + congestion)));
}

Ptr<TcpCongestionOps>
TcpAdaptiveReno::Fork()
{
    return CreateObject<TcpAdaptiveReno>(*this);
}

} // namespace ns3
