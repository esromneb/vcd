'use strict';

const stream = require('stream');
const EventEmitter = require('events').EventEmitter;
let lib = require('bindings')('vcd.node');

module.exports = () => {

  const wires = {};
  const info = {stack: [wires], wires: wires};

  const s = new stream.Writable();

  // const lifee = new EventEmitter();
  const lifemit = s.emit.bind(s);

  const triee = new EventEmitter();
  const triemit = triee.emit.bind(triee);

  const cxt = lib.init(lifemit, triemit, info);

  s._write = function (chunk, encoding, callback) {
    lib.execute(cxt, lifemit, triemit, info, chunk);
    callback();
  };

  s.change = {
    on: (id, fn) => {
      triee.on(id, fn);
      const triggerString = triee.eventNames().join(' ') + '  ';
      lib.setTrigger(cxt, triggerString);
    }
  };

  s.info = info;

  s.debug0 = lib.debug0;

  return s;
};
