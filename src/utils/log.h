#ifndef LOG_H_
#define LOG_H_

#define INF(label) EV << __FILE__ << ":" << __LINE__ << "\n" << "   INF(" << label << "): "
#define DBG(label) EV << __FILE__ << ":" << __LINE__ << "\n" << " - DBG(" << label << "): "
#define WRN(label) EV << __FILE__ << ":" << __LINE__ << "\n" << " ! WRN(" << label << "): "
#define ERR(label) EV << __FILE__ << ":" << __LINE__ << "\n" << "!!!ERR(" << label << "): "

#endif