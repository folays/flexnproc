#!/usr/bin/perl -w

use strict;
use warnings FATAL => qw(uninitialized);

use Getopt::Long qw(:config no_auto_abbrev require_order);

my %long_opts = (verbose => 1, min_mb => 10);
GetOptions("verbose=i" => \$long_opts{verbose},
	   "min-mb=i" => \$long_opts{min_mb},
    ) or die "bad options";

sub run()
{
    printf "-> %s\n", join(" ", @_);
    return system(@_);
}

do { !&run(qw(rmmod flexnproc.ko)) or die } if -e "/sys/module/flexnproc";

!&run(qw(insmod flexnproc.ko),
      "verbose=$long_opts{verbose}",
    ) or die;
