#!/usr/bin/env perl

use strict;
use warnings;

print "Content-Type: text/html\r\n\r\n";

my $method = $ENV{'REQUEST_METHOD'} || '';

print "<html><body>";
print "<h2>Method: $method</h2>";

if ($method eq "GET") {
    my $query = $ENV{'QUERY_STRING'} || '';
    print "<p>Query string: $query</p>";
}

elsif ($method eq "POST") {
    my $length = $ENV{'CONTENT_LENGTH'} || 0;

    my $body = '';
    if ($length > 0) {
        read(STDIN, $body, $length);
    }

    print "<p>POST body: $body</p>";
}

else {
    print "<p>Unknown method</p>";
}

print "</body></html>";
