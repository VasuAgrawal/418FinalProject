import csv

def process(fname):
    timings = dict()
    with open(fname, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            if row[0] == 'Threads':
                continue
            nThreads = int(row[0])
            contention = float(row[1])
            ms = float(row[2])
            if nThreads not in timings:
                timings[nThreads] = dict()
            timings[nThreads][contention] = ms

    newname = fname[:-4] + '-proc.csv'
    with open(newname, 'w', newline='') as newcsvfile:
        writer = csv.writer(newcsvfile, delimiter=',')
        row = ['Thread Count'] + ["%d%% Contention"%((x+(0 if x == 0 else .00001))*100) for x in sorted(timings[1])]
        writer.writerow(row)
        for tnum in sorted(timings):
            row = [timings[tnum][cont] for cont in sorted(timings[tnum])]
            writer.writerow([tnum] + row)

process('coarse.csv')
process('fine.csv')
process('fine-rw.csv')
process('lockfree.csv')


process('coarse-read.csv')
process('fine-read.csv')
process('fine-rw-read.csv')
process('lockfree-read.csv')