//
// Generated file, do not edit! Created by opp_msgtool 6.0 from physicallayer/wired/base/WiredNICPkt.msg.
//

#ifndef __WIREDNICPKT_M_H
#define __WIREDNICPKT_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class WiredNICPkt;
/**
 * Class generated from <tt>physicallayer/wired/base/WiredNICPkt.msg:16</tt> by opp_msgtool.
 * <pre>
 * packet WiredNICPkt
 * {
 *     int src;
 *     int dest;
 *     int seqNum;
 *     int outPort;
 * }
 * </pre>
 */
class WiredNICPkt : public ::omnetpp::cPacket
{
  protected:
    int src = 0;
    int dest = 0;
    int seqNum = 0;
    int outPort = 0;

  private:
    void copy(const WiredNICPkt& other);

  protected:
    bool operator==(const WiredNICPkt&) = delete;

  public:
    WiredNICPkt(const char *name=nullptr, short kind=0);
    WiredNICPkt(const WiredNICPkt& other);
    virtual ~WiredNICPkt();
    WiredNICPkt& operator=(const WiredNICPkt& other);
    virtual WiredNICPkt *dup() const override {return new WiredNICPkt(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getSrc() const;
    virtual void setSrc(int src);

    virtual int getDest() const;
    virtual void setDest(int dest);

    virtual int getSeqNum() const;
    virtual void setSeqNum(int seqNum);

    virtual int getOutPort() const;
    virtual void setOutPort(int outPort);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const WiredNICPkt& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, WiredNICPkt& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline WiredNICPkt *fromAnyPtr(any_ptr ptr) { return check_and_cast<WiredNICPkt*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __WIREDNICPKT_M_H

