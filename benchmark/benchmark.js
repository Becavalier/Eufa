const CDP = require('chrome-remote-interface');
const chromeLauncher = require('chrome-launcher');
const fs = require('fs');
const Table = require('easy-table');
const ora = require('ora');
const chalk = require('chalk');

// Launch chrome in headless mode;
var spinner = ora('[Eufa] calculating for benchmark...');
spinner.start();

chromeLauncher.launch({
    //chromeFlags: ['--headless'],
    port: 9222
}).then(chrome => {
    CDP(client => {
        const { Runtime } = client;
        const wasmPath = './dist/eufa-module.wasm';
        const modulePath = './dist/eufa-module.js'
        fs.stat(wasmPath, (err, stats) => {
            if (stats.isFile()) {
                fs.readFile(wasmPath, (err, data) => {
                    if (err) throw err;
                    let strPresent = data.reduce((pre, value) => {
                        return `${pre} ${value}`
                    });
                    fs.readFile(modulePath, 'utf8', (err, data) => {
                        const code = `new Promise((resolve, reject) => {
                                          var Module = {}, Eufa = {};
                                          Module.wasmBinary = \`${strPresent}\`.trim().split(/[\\s\\r\\n]+/g).map(str => parseInt(str, 10));
                                          function callback () {
                                              let i_testWithOperands = (method, operandCount = 2, times = 1e+7) => {
                                                  let startTime = performance.now();
                                                  let sum = 0;
                                                  for (let j = 0; j < times; j++) {
                                                      let _x = Math.round(Math.random() * 1e+7) + 100;
                                                      let _y = Math.round(Math.random() * 1e+7) + 100;
                                                      if (operandCount === 1) {
                                                          sum += method(_x);
                                                      }
                                                      if (operandCount === 2) {
                                                          sum += method(_x, _y);
                                                      }
                                                  }
                                                  return (performance.now() - startTime).toFixed(3);
                                              }
                                    
                                              let f_testWithOperands = (method, operandCount = 2, times = 1e+7) => {
                                                  let startTime = performance.now();
                                                  let sum = 0;
                                                  for (let j = 0; j < times; j++) {
                                                      let _x = Math.random() * 1e+7 + 100;
                                                      let _y = Math.random() * 1e+7 + 100;
                                    
                                                      if (operandCount === 1) {
                                                          sum += method(_x);
                                                      }
                                                      if (operandCount === 2) {
                                                          sum += method(_x, _y);
                                                      }
                                                  }
                                                  return (performance.now() - startTime).toFixed(3);
                                              }
                                              
                                              let s_testWithOperands = (method, times = 1e+7) => {
                                                  let startTime = performance.now();
                                                  for (let j = 0; j < times; j++) {
                                                      method('abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz');
                                                  }
                                                  return (performance.now() - startTime).toFixed(3);
                                              }
                                    
                                              var data = [
                                                  {
                                                      method: 'i64_add／+',
                                                      times: '1e+7',
                                                      nativeResult: i_testWithOperands((x, y) => { return x + y; }),
                                                      wasmResult: i_testWithOperands(Eufa.Math.i64_add)
                                                  },
                                                  {
                                                      method: 'i64_minus／-',
                                                      times: '1e+7',
                                                      nativeResult: i_testWithOperands((x, y) => { return x - y; }),
                                                      wasmResult: i_testWithOperands(Eufa.Math.i64_minus)
                                                  },
                                                  {
                                                      method: 'i64_multiply／*',
                                                      times: '1e+7',
                                                      nativeResult: i_testWithOperands((x, y) => { return x * y; }),
                                                      wasmResult: i_testWithOperands(Eufa.Math.i64_multiply)
                                                  },
                                                  {
                                                      method: 'i64_divide／/',
                                                      times: '1e+7',
                                                      nativeResult: i_testWithOperands((x, y) => { return x / y; }),
                                                      wasmResult: i_testWithOperands(Eufa.Math.i64_divide)
                                                  },
                                                  {
                                                      method: 'i64_sqrt／Math.sqrt',
                                                      times: '1e+7',
                                                      nativeResult: i_testWithOperands(x => { return Math.sqrt(x); }, 1),
                                                      wasmResult: i_testWithOperands(Eufa.Math.i64_sqrt, 1)
                                                  },
                                                  {
                                                      method: 'f64_sqrt／Math.sqrt',
                                                      times: '1e+7',
                                                      nativeResult: f_testWithOperands(x => { return Math.sqrt(x); }, 1),
                                                      wasmResult: f_testWithOperands(Eufa.Math.f64_sqrt, 1)
                                                  },
                                                  {
                                                      method: 'ptr_capitalize／String.prototype.toUpperCase',
                                                      times: '1e+6',
                                                      nativeResult: s_testWithOperands(x => { return x.toUpperCase(); }, 1e+4),
                                                      wasmResult: s_testWithOperands(Eufa.String.capitalize, 1e+4)
                                                  }
                                              ];
                                    
                                              resolve(JSON.stringify(data));
                                          }
                                          ${data.replace(/\／n/g, '//n')} 
                                      }).catch(err => {
                                          throw err;
                                      });`;
                        Runtime.evaluate({
                            expression: code,
                            awaitPromise: true
                        }).then(response => {
                            let data = JSON.parse(response.result.value);
                            let table = new Table;
                            data.forEach(function(item) {
                                table.cell('Methods', item.method);
                                table.cell('Times', item.times);
                                table.cell('WebAssembly(ms)', item.wasmResult);
                                table.cell('Native(ms)', item.nativeResult);
                                table.cell('Percent(%)', (100 * (item.nativeResult - item.wasmResult) / item.nativeResult).toFixed(5));
                                table.newRow();
                            });
                            chrome.kill();
                            spinner.succeed('[Eufa] calculating for benchmark... done\n');
                            // Print result;
                            console.log(table.toString());
                            // Exit current pricess;
                            process.exit(0);
                        }).catch(err => {
                            chrome.kill();
                            spinner.fail(err);
                            process.exit(1);
                        });
                    });
                });
            }
        });
    }).on('error', (err) => {
        // Cannot connect to the remote endpoint
        console.error(err);
    });
});
