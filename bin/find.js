#!/usr/bin/env node
'use strict';

const fs = require('fs-extra');
const async = require('async');

const lib = require('../lib/index.js');

const dir = './tmp/';

function dumpKeys(obj, depth) {
  for (var k in obj) {
    if (typeof obj[k] == "object" && obj[k] !== null) {
      dumpKeys(obj[k], depth+1);
    } else {
      let p = ' '.repeat(depth*2);

      let v = obj[k];
      let vv = '';
      for( let i = 0; i < v.length; i++ ) {
        vv += v.charCodeAt(i) + ', ';
      }

      // console.log(p + k + vv);
      console.log(`${p}${k}: ${vv}`);
    // do something... 
    }
  }
}

fs.readdir(dir).then(files => {
  const tt0 = Date.now();
  async.eachLimit(files, 2, (fileName, callback) => {
    console.log(`file: ${fileName}`);
    if (!fileName.match(/.vcd$/)) {
      callback();
      return;
    }


    const t0 = Date.now();
    const inst = lib.parser();
    const loads = lib.and();
    const stores = lib.and();
    const duration = lib.activity(10);

    inst.on('$enddefinitions', () => {
      const _ = inst.info.wires;

      // console.log(JSON.stringify(_));
      console.log(_);
      dumpKeys(_, 0);
      console.log();
      console.log();
      // debugger;

      inst.change.on(_.TOP.clk,
        (time, cmd) => {
          debugger;
          loads.onA(time, cmd);
          // stores.onNotA(time, cmd);
        }
      );

      // inst.change.on(_.top.TestDriver.testHarness.system.tile.vpu.mem_i_load,
      //   (time, cmd) => {
      //     loads.onA(time, cmd);
      //     stores.onNotA(time, cmd);
      //   }
      // );
      // inst.change.on(_.top.TestDriver.testHarness.system.tile.vpu.mem_i_valid,
      //   (time, cmd) => {
      //     loads.onB(time, cmd);
      //     stores.onB(time, cmd);
      //     duration.on(time);
      //   }
      // );
    });

    inst.on('finish', () => {
      console.log(fileName, duration.time(), loads.time(), stores.time(), ((Date.now() - t0) / 1000 + 's'));
      inst.debug0();
      callback();
    });

    inst.on('error', err => {
      console.log(err);
    });

    fs.createReadStream(dir + fileName).pipe(inst);

  }, () => {
    console.log('Total time: ' + (Date.now() - tt0) / 1000 + 's');
  });
});
