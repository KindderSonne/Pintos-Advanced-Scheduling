# -*- perl -*-
use strict;
use warnings;
use tests::tests;
use tests::threads::mlfqs;



our ($test);

my (@output) = read_text_file ("$test.output");
common_checks ("run", @output);
@output = get_core_output ("run", @output);

# Check for memory allocation failures.
my $failures = 0;
foreach my $line (@output) {
  if ($line =~ /Failed to allocate memory/) {
    $failures++;
  }
}

if ($failures == 0) {
  pass;
} else {
  fail ("Memory allocation failures detected: $failures");
}
