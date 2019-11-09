#!C:\Perl\perl.exe
use 5.01;
use strict;
use warning;

sub main {
     say "Content-type: text/html";
     print "\n";
     say "<html>";
     say "    <head> <title> Perl here </title> </head>";
     say "    <body>";
     say "        <h1> Hello from Perl CGI </h1>";
     say "    </body>";
     say "</html>";
}

main();
