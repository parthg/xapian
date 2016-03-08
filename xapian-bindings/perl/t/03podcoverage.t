use strict;
# Make warnings fatal
use warnings;
BEGIN {$SIG{__WARN__} = sub { die "Terminating test due to warning: $_[0]" } };

use Test::More;

eval "use Test::Pod::Coverage 1.04";
plan skip_all => 'Test::Pod::Coverage 1.04 required' if $@;
plan skip_all => 'set TEST_POD_COVERAGE to enable this test' unless $ENV{TEST_POD_COVERAGE};

all_pod_coverage_ok({also_private => [qr/[^\d][0123]$/]});
