#!/usr/bin/python3
import sys
import os
import time
from copy import copy

"""
    Expects the following programs to exist:
        ./randl
        ./usel
        sort
        seq 
"""

class SortingTest:
    # sorting types
    RANDOM = 0  # random
    SORTED = 1  # ascending sorted
    REVERSE = 2 # descending sorted

    # generate different types of lists
    def generate_list(total, mode, duplicates, location):
        program = "seq" 
        if duplicates:
            program = "./randl"
        if mode == SortingTest.RANDOM:  
            os.system("{program} {total} | sort -R > {location}".format(program=program, total=total, location=location))
        elif mode == SortingTest.SORTED:
            os.system("{program} {total} > {location}".format(program=program, total=total, location=location))
        elif mode == SortingTest.REVERSE:
            os.system("{program} {total} | sort -nr > {location}".format(program=program, total=total, location=location))
        else:
            raise NameError("{mode} is not a valid option for list generation")
    
    # sort for program
    def test_sort(program, total, mode, duplicates, total_runs, location="/tmp/nums"):
        total_time = 0
        for run in range(total_runs):
            SortingTest.generate_list(total, mode, duplicates, location)
            start_time = time.time()
            os.system("{program} < {location} > /dev/null".format(program=program, location=location))
            elapsed_time = time.time()-start_time
            total_time += elapsed_time
        average_time = total_time/float(total_runs)
        return average_time

    # run tests for a list of programs, and output results
    def run_tests(test_regime, test_programs):
        results = []
        for test in test_regime:
            results_row = []
            results_row.extend(test)
            for program in test_programs:
                average_time = SortingTest.test_sort(program, *test)
                results_row.append(average_time)
            results.append(results_row)
        return results

    # create a html table
    def generate_table(headers, rows):
        table = open("results.html", mode="w")
        # write head
        table.write("<html><body><table border=1>")
        # write headings
        table.write("<tr>") 
        for index, header in enumerate(headers):
            value = header
            if index >= 4:
                value = "Avg time for {program} (s)".format(program=header)
            table.write("<td><b>{header}</b></td>".format(header=value))
        table.write("</tr>")
        # write data
        for row in rows:
            table.write("<tr>")
            for index, column in enumerate(row):
                value = column
                # dirty way to format each column
                if index == 1:
                    if column == SortingTest.RANDOM:
                        value = "Random"
                    elif column == SortingTest.SORTED:
                        value = "Sorted"
                    elif column == SortingTest.REVERSE:
                        value = "Reverse"
                    else:
                        value = "Unknown"
                elif index >= 4:
                    value = "{:.03f}".format(column)
                table.write("<td>{column}</td>".format(column=value))
            table.write("</tr>")
        # end table
        table.write("</table></body></html>")
        table.close()

    # run test from headers, config, programlist
    def start_tests(headers, config, programs):
        results = SortingTest.run_tests(config, programs)
        headers.extend(programs)
        SortingTest.generate_table(headers, results)

def main():
    ST = SortingTest
    headers = ["Input Size", "Initial Order", "Has Duplicates", "Total Runs"]
    config = [
    #   Total   Order       Copies  Runs    
        [5000,  ST.RANDOM,  False,  10],
        [5000,  ST.SORTED,  False,  10],
        [5000,  ST.REVERSE, False,  10],
        [5000,  ST.RANDOM,  True,   10],
        [5000,  ST.SORTED,  True,   10],
        [5000,  ST.REVERSE, True,   10],
        [10000, ST.RANDOM,  False,  10],
        [10000, ST.SORTED,  False,  10],
        [10000, ST.REVERSE, False,  10],
        [10000, ST.RANDOM,  True,   10],
        [10000, ST.SORTED,  True,   10],
        [10000, ST.REVERSE, True,   10],
    ]
    programs = ["./usel", "sort"]
    SortingTest.start_tests(headers, config, programs)
    

if __name__ == "__main__":
    main()

