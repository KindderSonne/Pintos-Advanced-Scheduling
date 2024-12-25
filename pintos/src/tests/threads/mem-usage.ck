# -*- perl -*-
use strict;
use warnings;
use tests::tests;
use tests::threads::mlfqs;

our ($test);

my (@output) = read_text_file ("$test.output");
common_checks ("run", @output);
@output = get_core_output ("run", @output);

# Check for approximate memory usage
my $approx_mem_used;
foreach (@output) {
    if (/Approximate memory used: (\d+) KB/) {
        $approx_mem_used = $1;
        last;
    }
}

if (!defined $approx_mem_used) {
    fail ("Could not find approximate memory usage.");
}

# Expected memory usage is approximately THREAD_CNT * ALLOC_CNT * ALLOC_SIZE / 1024 KB
my $expected_mem_used = 5 * 256 * 4; # 5120 KB

# Allow for a margin of error (e.g., due to metadata or other allocations)
my $margin_of_error = 1024; # +/- 1024 KB

if (abs($approx_mem_used - $expected_mem_used) > $margin_of_error) {
    fail ("Approximate memory usage ($approx_mem_used KB) differs from expected usage ($expected_mem_used KB) by more than $margin_of_error KB.");
}

pass;