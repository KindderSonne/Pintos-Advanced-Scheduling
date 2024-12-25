# -*- perl -*-
use strict;
use warnings;
use tests::tests;
use tests::threads::mlfqs;

our ($test);

my (@output) = read_text_file ("$test.output");
common_checks ("run", @output);
@output = get_core_output ("run", @output);

# Check for load average values (simplified check)
my $load_avg_count = 0;
foreach (@output) {
    if (/After \d+ seconds, load average=(\d+\.\d+)\./) {
        $load_avg_count++;
    }
}

if ($load_avg_count < 5) {
    fail ("Not enough load average values were printed.");
}

pass;