import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob

SIM_TIME = 3600
INTERVAL = 5

def extract_info(file_name):
    df = pd.read_csv(file_name)
    df = df[['type', 'name', 'vectime', 'vecvalue']]
    df = df.loc[(df['name'] == 'receivedMessage:vector') & (df['type'] == 'vector')]
    
    total = [(0,0) for _ in range(0, SIM_TIME, INTERVAL)]

    for _, row in df.iterrows():
        l = list(zip([int(x) for x in row.vectime.split(" ")], [int(x) for x in row.vecvalue.split(" ")]))
        for time in range(INTERVAL, SIM_TIME, INTERVAL):
            val = [v for t,v in l if t > time and t <= (time + INTERVAL)]
            if len(val) == 1:
                total[int(time/INTERVAL)] = (total[int(time/INTERVAL)][0] + val[0] ,total[int(time/INTERVAL)][1] + 1)

    total = total[1:-1]
    total = [(p / t) * 100 for p,t in total]
    return total

fig = plt.figure()
over_time = fig.add_subplot(1, 2, 1)
mean = fig.add_subplot(1, 2, 2)
val = 1

for file in glob.glob('./*.csv'):
    res = extract_info(file)
    name = "Sim " + str(val)
    val += 1
    res_mean = np.mean(np.array(res[2:-1]).reshape(-1, 5), axis=1)

    mean.bar([name], [np.mean(res)], width=0.35)
    mean.set_title("Mean coverage")
    mean.set_ylabel("Coverage (%)")

    over_time.plot(range(len(res_mean)), res_mean, label=name, marker="*")
    over_time.set_title("Coverage over time")
    over_time.set_xlabel("Instant time (25s)")
    over_time.set_ylabel("Coverage (%)")
    over_time.legend(loc="lower right")

plt.show()