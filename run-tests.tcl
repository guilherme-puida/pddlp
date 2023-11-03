#!/usr/bin/env tclsh

set tests [glob tests/tokenizer/*.test]

foreach test $tests {
  set actual [exec build/pddlp-tokenize $test]
  set expected [file rootname $test].expected

  set expectedexists [file exists $expected]

  if {$expectedexists} {
    set status [catch {exec diff <<$actual $expected - -Z} result]

    if {$status == 0} {
      puts "= $test OK"
    } else {
      puts "= $test FAILED"
      puts $result
    }
  } else {
    set out [open $expected w]
    puts -nonewline $out $actual
    close $out
    puts "= $test CREATED"
    puts $actual
  }
}
