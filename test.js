const addon = require('./build/Release/index.node');

console.log('U盘列表：', addon.getUsbFlashDisks())