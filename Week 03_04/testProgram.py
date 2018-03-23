#!/usr/bin/python3
import csv
import filecmp
import os
import sys
import time
import matplotlib.pyplot as plt

HEADER_ROW = ['Total Items', 'Time (s)']
# testing folder
TEST_FOLDER = './Testing'
# generate test file
TEST_FILE_LOCATION = '/tmp/nums'
# read test files
TEST_OUT_CMD = '{program} {args} < {input} > {output}'
TEST_LOCATION_GIVEN = '/tmp/out_0'
TEST_LOCATION_EXPECTED = '/tmp/out_1'
# debugging strings
TEST_DEBUG_STRING = '{type}, {amount}: #{test:02d} {time}'
# test types
TEST_TYPES = {
    'Random_No_Duplicates': './gen {amount} R > {output}',
    'Sorted_No_Duplicates': './gen {amount} A > {output}',
    'Reversed_No_Duplicates': './gen {amount} D > {output}',
    'Random_Duplicates': './randl {amount} R > {output}',
    'Sorted_Duplicates': './randl {amount} A > {output}',
    'Reversed_Duplicates': './randl {amount} D > {output}'
}
# test profile
TOTAL_REPEATS = 5
_AMOUNTS = [
    10, 1000, 10000, 100000,
    250000, 500000, 750000, 1000000,
]

AMOUNTS = [
	10, 50, 100, 500,
	1000, 2500, 5000, 
	10000, 25000, 50000, 75000,
	100000,
]
	


# create CSV file
def makeCSV(program):
    programName = program.replace('./', '')
    file = open('{program}_test_results.csv'.format(folder=TEST_FOLDER, program=programName), 'w')
    writer = csv.writer(file, lineterminator='\n')
    return writer

# run all tests
def runTests(program, programArgs):
    csv = makeCSV(program)
    for testType, cmdString in TEST_TYPES.items():
        runTest(csv, program, programArgs, cmdString, testType)

# run one test
def runTest(csv, program, programArgs, cmdString, testType):
    # setup csv file
    csv.writerow(['Type: '+testType])
    headerRow = ['Input Size']
    headerRow.extend(['Test {} (s)'.format(i+1) for i in range(TOTAL_REPEATS)])
    headerRow.extend(['Average (s)', 'Range (s)', 'Min (s)', 'Max (s)', 'Unstable Outcomes'])
    csv.writerow(headerRow)
    # setup values for graph
    xValues = []
    yValues = []
    yErrors = []
    yUnstable = []
    # testing stability
    for amount in AMOUNTS:
        times = []
        unstableOutcomes = 0
        for repeat in range(TOTAL_REPEATS):
            # make test
            os.system(cmdString.format(amount=amount, output=TEST_FILE_LOCATION))
            # run test
            print('>', end='')
            startTime = time.time()
            os.system(TEST_OUT_CMD.format(program=program, args=programArgs, input=TEST_FILE_LOCATION, output=TEST_LOCATION_GIVEN))
            elapsedTime = time.time()-startTime
            print(':', end='')
            # validate test
            os.system(TEST_OUT_CMD.format(program='sort', args='-n -s', input=TEST_FILE_LOCATION, output=TEST_LOCATION_EXPECTED))
            if not filecmp.cmp(TEST_LOCATION_EXPECTED, TEST_LOCATION_GIVEN):
                print(TEST_DEBUG_STRING.format(type=testType, amount=amount, test=repeat, time=elapsedTime)+' **ERROR/UNSTABLE** ')
                unstableOutcomes += 1
            # if validated
            else:
                print(TEST_DEBUG_STRING.format(type=testType, amount=amount, test=repeat, time=elapsedTime))
            times.append(elapsedTime)
        # get information about runs
        averageTime = sum(times)/len(times)
        maxTime = max(times)
        minTime = min(times)
        rangeTime = maxTime-minTime
        # add to table
        times.extend([averageTime, rangeTime, minTime, maxTime, unstableOutcomes])
        times.insert(0, amount)
        csv.writerow(times)
        # add to graph
        xValues.append(amount)
        yValues.append(averageTime)
        yErrors.append(rangeTime)
        yUnstable.append(unstableOutcomes)
    makePlot(program, testType, xValues, yValues, yErrors, yUnstable)

# create a plot
def makePlot(program, testType, xValues, yValues, yErrors, yUnstable):
    # plot timing info
    plt.scatter(xValues, yValues)
    plt.errorbar(xValues, yValues, yerr=yErrors, linestyle='None')
    plt.xlabel('Amount of items')
    plt.ylabel('Elapsed time (s)')
    plt.title('{program}_{type}_times'.format(program=program, type=testType))
    plt.savefig("{folder}/{program}_{type}_times.png".format(folder=TEST_FOLDER, program=program, type=testType))
    plt.clf()
    # plot instability
    plt.scatter(xValues, yUnstable)
    plt.xlabel('Amount of items')
    plt.ylabel('Total unstable/incorrect runs')
    plt.title("{program}_{type}_instability".format(program=program, type=testType))
    plt.savefig("{folder}/{program}_{type}_instability.png".format(folder=TEST_FOLDER, program=program, type=testType))
    plt.clf()



# check args
def checkArgs():
    if len(sys.argv) <= 1:
        print('Usage: {} <program>'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(0)
    program = sys.argv[1]
    if len(sys.argv) >= 3:
        programArgs = ' '.join(sys.argv[2:])
    else:
        programArgs = ''
    return (program, programArgs)

def main():
    program, programArgs = checkArgs()
    print("Testing: {} {}".format(program, programArgs))
    runTests(program, programArgs)


if __name__ == '__main__':
    main()
