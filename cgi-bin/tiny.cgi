#! /usr/bin/perl

#File:  tinyPL.cgi
#Example of a Perl script that produces static output

use strict;
use warnings;
use CGI qw(:standard);

my $username;

   $username = "Susan";
   print header();
   print start_html("First CGI program");
   print "Hello $username.\n";
   print end_html();
