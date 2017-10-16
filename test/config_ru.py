import sys
import matplotlib as mpl
import matplotlib.pyplot as plot
import matplotlib.ticker as ticker

mpl.rcParams['ps.useafm'] = True
mpl.rcParams['pdf.use14corefonts'] = True
mpl.rcParams['text.usetex'] = True
mpl.rcParams['text.latex.preamble'] = [
  r'\usepackage{siunitx}',
  r'\sisetup{detect-all}',
  r'\usepackage{helvet}',
  r'\usepackage{sansmath}',
  r'\sansmath'
]

fig = plot.figure(figsize=(8.000000, 4.000000))
ax = fig.add_subplot(111)

ax.plot([8.000000, 16.000000, 24.000000, 32.000000, 40.0, ], [15.01, 15.47, 16.06, 16.33, 15.95, ], color="#77ab59", label="32", linewidth=2, marker='o', markersize=8)
ax.plot([8.000000, 16.000000, 24.000000, 32.000000, 40.0, ], [15.24, 15.98, 16.19, 16.42, 16.71, ], color="#36802d", label="64", linewidth=2, marker='x', markersize=8)
ax.plot([8.000000, 16.000000, 24.000000, 32.000000, 40.0, ], [15.36, 16.17, 16.72, 16.84, 16.96, ], color="#234d20", label="128", linewidth=2, marker='v', markersize=8)

ax.set_xlabel("Maximum Delta Chain Length", fontsize=24, weight='bold')
ax.set_ylabel("Throughput (MOps/Sec)", fontsize=24, weight='bold')


for label in ax.get_xticklabels():
    label.set_fontsize(24)

for label in ax.get_yticklabels():
    label.set_fontsize(24)

ax.set_ylim(0.000000, 25.000000)
ax.set_xlim(6, 42)

# This sets the X axis ticks
ax.xaxis.set_ticks([8, 16, 24, 32, 40, ])
ax.yaxis.grid(True)

ax.legend(loc="lower left", prop={'size':22}, ncol=3)

plot.savefig("config-ru.pdf", bbox_inches='tight')
