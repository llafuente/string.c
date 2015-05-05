/*
* Copyright 2015 Luis Lafuente <llafuente@noboxout.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//
// easy execution using: `node compare.js t1 t2`
// backtips included to open firefox!
//

var async = require("async");
var fs = require("fs");
var child_process = require("child_process");


var benchmarks = ["append", "ascii", "case", "compare", "encoding", "from", "hexbinhex", "internal", "iterators", "justify", "repeat", "search", "shuffle", "sub", "to", "trim", "utf32", "utf8", "utils"];
var output_file = "bench_data/comparison.html";

var output = ["<html><head></head><body><table>"];
var images = [];

var t1 = process.argv[2];
var t2 = process.argv[3];

if (!t1 || !t2) {
  console.error("Usage: node compare.js <bench> <bench2>");

  process.exit();
}

console.error("create comparison:", t1, "vs", t2);

try {
  fs.mkdirSync("bench_data");
} catch(e) {}

async.eachSeries(benchmarks, function(file, callback) {
  console.error("processing:", file);

  output.push('<tr><td>' + file + '<td>');
  output.push('<td>');
  var ministat = 'ministat bench_data/' + t1 + '-' + file + ' bench_data/' + t2 + '-' + file;
  child_process.exec(ministat, function(err, stdout) {

    stdout = stdout.split("\n").filter(function(l) {
      return l.indexOf("+-") != 0 && l.indexOf("|") != 0;
    }).slice(3);

    // something conclusive is found!
    if (stdout[2] ==  'Difference at 95.0% confidence') {
      var diff = parseFloat(stdout[4].trim());
      if (diff > 0) {
        output.push('<span style="color: red">' + stdout[4] +'</span>');
      } else {
        output.push('<span style="color: green">' + stdout[4] +'</span>');
      }
    } else {
      output.push('<span style="color: orange">N/A</span>');
    }

    output.push('</td><td><a href="#' + file + '">Plot</a>');
    output.push('</td></tr>');

    images.push('<a name="' + file + '">');
    images.push('<img src="' + t1 + '-' + t2 + '-' + file + '.png">');
    images.push('<br />');

    var plot = ['set terminal png',
    'set output \'bench_data/' + t1 + '-' + t2 + '-' + file + '.png\'',

    'set datafile separator "\\t"',

    'plot \'bench_data/' + t1 + '-' + file + '\' with points, \\',
    '\'bench_data/' + t2 + '-' + file + '\' with points, \\',
    '\'bench_data/' + t1 + '-' + file + '\' with lines, \\',
    '\'bench_data/' + t2 + '-' + file + '\' with lines'].join("\n");

    fs.writeFileSync("bench_data/plot.p", plot);

    child_process.exec("gnuplot bench_data/plot.p", callback);
  });
}, function(err){
  console.error(arguments);

  output.push("</table>");
  output.push(images.join("\n"));
  output.push('</body>');
  require("fs").writeFileSync(output_file, output.join("\n"));

  console.log("firefox " + output_file);
});
