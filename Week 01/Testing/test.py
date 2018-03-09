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
# sorting types
RANDOM = {"type": "Random", "cmd": "{program} {total} | sort -R > {location}"}  # random
SORTED = {"type": "Sorted", "cmd": "{program} {total} > {location}"}
REVERSE= {"type": "Reverse","cmd": "{program} {total} | sort -nr > {location}"}

# cmd string for testing
CMD_STRING = "{program} -n < {location} > {output}"

# default list options
DEFAULT_CONFIG = {
    "total": 5000,
    "mode": RANDOM,
    "runs": 1,
    "location": "/tmp/nums",
}

# generate different types of lists
def generate_list(**config):
    cmd_config = DEFAULT_CONFIG.copy()
    cmd_config.update(config)
    program = "seq"
    if config["duplicates"]:
        program = "./randl"
    os.system(cmd_config["mode"]["cmd"].format(program=program, **cmd_config))

# test batch with programs
def test_programs(programs, **config):
    cmd_config = DEFAULT_CONFIG.copy()
    cmd_config.update(config)
    results = {program:0 for program in programs}
    total_runs = cmd_config["runs"]
    for run in range(total_runs):
        generate_list(**cmd_config) # regenerate list each run
        for program in programs:
            elapsed_time = test_program(program, cmd_config["location"])
            results[program] += elapsed_time
    average_results = {program:elapsed_time/float(total_runs) for program,elapsed_time in results.items()}
    return average_results

def test_program(program, location, output="/dev/null"):
    start_time = time.time()
    os.system(CMD_STRING.format(program=program, location=location, output=output))
    elapsed_time = time.time()-start_time
    return elapsed_time

# returns average times in a list or dicts
def run_tests(programs, config_list):
    results = []
    for config in config_list:
        results.append(test_programs(programs, **config))
    return results

# create config file given headers and array
def generate_config(headers, data):
    config = []
    for row in data:
        config_entry = {name:column for name,column in zip(headers, row)}
        config_entry_full = DEFAULT_CONFIG.copy()
        config_entry_full.update(config_entry)
        config.append(config_entry_full)
    return config


# create a html table
def generate_table(config_list, results, headers, programs):
    # combine results and config data
    table_data = []
    for config_entry, result in zip(config_list, results):
        config_entry.update(result)
        table_data.append(config_entry)
    # write head
    table = open("results.html", mode="w")
    table.write("<html><body><table border=1>")
    # write headings
    table.write("<tr>") 
    header_format = "<td><b>{column}</b></td>"
    for header in headers:
        column = header["header"]
        if header["name"] is None:
            for program in programs:
                column = header["header"].format(program=program)
                table.write(header_format.format(column=column))
        else:
            table.write(header_format.format(column=column))
    table.write("</tr>")
    # write data
    row_format = "<td>{column}</td>"
    for row in table_data:
        table.write("<tr>")
        for header in headers:
            header_name = header["name"]
            if header_name is None:
                for program in programs:
                    value = row[program]
                    column = header["format"](value)
                    table.write(row_format.format(column=column))
            else:
                value = row[header_name]
                column = header["format"](value)
                table.write(row_format.format(column=column))
        table.write("</tr>")
    # end table
    table.write("</table></body></html>")
    table.close()

def main():
    # create the config
    config_keys = ["total", "mode", "duplicates", "runs"]
    config_data = [
    #   Total   Order       Copies  Runs    
        [5000,  RANDOM,     False,  10],
        [5000,  SORTED,     False,  10],
        [5000,  REVERSE,    False,  10],
        [5000,  RANDOM,     True,   10],
        [5000,  SORTED,     True,   10],
        [5000,  REVERSE,    True,   10],
        [10000, RANDOM,     False,  10],
        [10000, SORTED,     False,  10],
        [10000, REVERSE,    False,  10],
        [10000, RANDOM,     True,   10],
        [10000, SORTED,     True,   10],
        [10000, REVERSE,    True,   10],
    ]
    config_list = generate_config(config_keys, config_data)
    # program list
    programs = ["./usel", "sort"]
    results = run_tests(programs, config_list)
    # generate table of stuff
    headers = [
        {
            "name": "total", 
            "header": "Input Size",
            "format": lambda value: value,    # function returns string
        },
        {
            "name": "mode",
            "header": "Initial Order", 
            "format": lambda value: value["type"],
        },
        {
            "name": "duplicates",
            "header": "Has duplicates",
            "format": lambda value: str(value),
        }, 
        {
            "name": "runs",
            "header": "Number of runs",
            "format": lambda value: value,
        },
        {
            "name": None,   # use for program timings
            "header": "Time taken for {program} (s)",
            "format": lambda value: "{:.03f}".format(value),
        }
    ]
    generate_table(config_list, results, headers, programs)
    

if __name__ == "__main__":
    main()

