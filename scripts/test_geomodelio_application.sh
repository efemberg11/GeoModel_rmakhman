#! /bin/bash
# test_geomodelio_application.sh
#

# RUN GEOMODELIO-BASED APPLICATION N TIMES AND PARSE THE TIME/MEMORY OUTPUT
#
# You should have a GeoModelIO-based application compiled.
# For example, you can use GeoModelExamples/HelloGeoRead.
# After compiling that, you will have a `helloGeoRead` executable in the
# build folder. Just source this script passing the executable name as
# parameter.
# Optional arguments:
#  - max number of concurrent threads as the second parameter [default = 8]
#  - number of repetitions per test as the third parameter [default = 5]
#  - `time` application to use [default = `/usr/bin/time`] --> TODO: parsing is for GNU time at the moment!
#
# For example:
#
#   source ../GeoModelIO/scripts/time_geomodelio_app.sh helloGeoRead 4 10
#
# will test `helloGeoRead` with up to 4 threads, 10 times each
#
# The output of all tests will be saved in an `output.txt` file.
#
# On macOS, we suggest to install GNU time with Homebrew (http:brew.sh):
#
#   brew install gtime
#
# Then, you can use `gtime` as timing application by passing that as
# the fourth parameter:
#
#   source ../GeoModelIO/scripts/time_geomodelio_app.sh helloGeoRead 4 10 gtime
#
#
# -----------------
#
# On Linux, use `/usr/bin/time` instead of the shell built-in `time` function.
# Then capture the `stderr` into a separate file from the `stdout` of your
# application command.
# Since the elapsed real time is the first word of time's output,
# that's easy to grab with `awk`.
#
# `grep` has a `-q` switch so no need to redirect to `/dev/null` to avoid
# cluttering the output with the output of `grep`.
#
# TODO:
# GNU time does not allow to use milliseconds, needed for quick commands
#
#
# Sources:
# - https://unix.stackexchange.com/a/533451/245009 (parse `time` output)
# - https://askubuntu.com/a/29379/830120 {get return value of a command}
# - https://stackoverflow.com/a/16040609/320369
# - https://stackoverflow.com/a/10959179/320369
# - https://stackoverflow.com/a/2013573/320369
# - https://bash.cyberciti.biz/guide/Create_usage_messages
# - https://www.cyberciti.biz/faq/unix-linux-iterate-over-a-variable-range-of-numbers-in-bash/
# - https://stackoverflow.com/a/6002329/320369
# - https://unix.stackexchange.com/a/122848/245009
# - https://stackoverflow.com/a/39458935/320369
# - https://stackoverflow.com/a/22190928/320369 (awk - set line number)
# - https://stackoverflow.com/a/8467449/320369 (printf / echo)
# - https://stackoverflow.com/a/2440685/320369 ('read' line from file)
# - https://stackoverflow.com/a/4181721/320369 (concatenate strings - I)
# - https://stackoverflow.com/a/2250199/320369 (concatenate strings - II)
# - https://stackoverflow.com/a/20915874/320369 (long, complete answer about getting command output & logfiles in general)
# - https://github.com/torvalds/linux/blob/master/tools/memory-model/scripts/cmplitmushist.sh
#

# Set the executable to test
EXEC=$1   # first command line argument

# Verify the type of input and number of value
# Display an error message if the executable to test was not passed and
# exit the shell script with a status of 1 using exit 1 command.
[ $# -eq 0 ] && { echo "Usage: $0 executable"; exit 1; }


# Variables. Assign default values if not specified by the user
NTHREADS=${2:-8} # default value: up to 8 threads
MTIMES=${3:-5}   # default value: 5 repetitions for each test
# APP=${4:-"/usr/bin/time"} # default timing application: /usr/bin/time
APP=${4:-"gtime"} # TODO: output parsing is only for GNU tinme at the moment

function eval_command() {
  "$@";
}

# init the output file
HEADER="ThreadsN TestN Timestamp TimeUser[s] TimeElapsed[s] MaxMem[Kb] Status"
printf "$HEADER\n" > output.txt

echo "Testing the application ${1} with up to $NTHREADS concurrent threads, $MTIMES, using the '$APP' timing application."
# Test up to N concurrent threads
# for n in {0..${NTHREADS}}; do
for (( n=0; n<=$NTHREADS; n++ ))
do
  export GEOMODEL_GEOMODELIO_NTHREADS=$n;
  echo "Using # threads: " $n;
  ## run command M times
  # for m in {1..$MTIMES};
  for (( m=1; m<=$MTIMES; m++ ))
  do
    # launch the actual command, through an 'eval function'
    eval_command $APP $EXEC > log.out 2>time.out
    # get the return value of the command, to check if it succeeded
    return_value=$?

    time_user_str=$(awk 'NR==1{print $1}' time.out)
    time_elapsed_str=$(awk 'NR==1{print $3}' time.out)
    mem_max_str=$(awk 'NR==1{print $6}' time.out)

    time_user=$(awk -F'user' '{print $1}' <<< "$time_user_str")
    time_elapsed=$(awk -F'elapsed' '{print $1}' <<< "$time_elapsed_str")
    mem_max=$(awk -F'maxresident' '{print $1}' <<< "$mem_max_str")

    date_str=$(date)
    date_epoch=$(date +%s)

    OUT_VERBOSE="threads $n - test $m - $date_str ($date_epoch) - user $time_user_str $time_user [s] elapsed $time_elapsed_str $time_elapsed [s] maxmem $mem_max_str $mem_max [Kb]"
    OUT="$n $m $date_epoch $time_user $time_elapsed $mem_max"

    # test the success status of the executable
    if [ $return_value -ne 0 ]; then
      echo "Return code was not zero but $retval --> executable probably failed"
      FLAG="Failed"
    else
      FLAG="Succeeded"
    fi

    # concatenate the test results and the global statement
    OUT_VERBOSE+=" $FLAG"
    OUT="$OUT $FLAG"


    # print on screen and save to file
    printf "$OUT_VERBOSE\n"
    printf "$OUT\n" >> output.txt

  done
done
