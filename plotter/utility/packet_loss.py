from SeriesCollector import SeriesCollector, transform
from SeriesPlotter import SeriesPlotter
import matplotlib.pyplot as plt
from util import sum as usum

sc = SeriesCollector("analysis/json/twindow_maxsas_adaptive",
                     "twindow_maxsas_adaptive_$mps-$,$,$,$Mobility,$,$,$",
                     speed=[2],
                     twait=[1],
                     th=[0],
                     n=[40],
                     mobility=["Random"],
                     MNs=[10, 50, 100, 150, 200, 250, 300, 350,400,450,500,512],
                     policy = ["roundrobin", "closestfirst"],
                     pdr_t = [0.95,0.99]
                     )

sp = SeriesPlotter("errorbar")

pol = "roundrobin"

sp.add_series(transform(sc.query("MNs", "packetsLostConflict",  twait=1, policy=pol, pdr_t = 0.99, speed = 2 ), lambda x: x * 100), color="magenta", label="Conflicts", marker="o")
sp.add_series(transform(sc.query("MNs", "packetsLostTransmissionErrors", twait=1, policy=pol, pdr_t = 0.99, speed = 2 ), lambda x: x * 100), color="gold", label="Errors", marker="o")
sp.add_series(transform(sc.query("MNs", "packetsLostOutOfRange", twait=1, policy=pol, pdr_t = 0.99, speed = 2 ), lambda x: x * 100), color="black", label="Out of range", marker="o")

fig, ax = plt.subplots()
sp.draw_all(ax)
ax.legend()
ax.grid(True)
ax.set_ylim(0,25)
fig.savefig(f"plotter/plots/adaptive_099/{pol}_packet_loss_2mps.pdf")


