## nodejs U盘、移动磁盘检测工具 （windows系统）

### About

快速列出本机U盘和移动磁盘的名称和盘符

### Usage

```
// nodejs

const UsbTool = require('./build/Release/index.node');

console.log('U盘列表：', UsbTool.getUsbFlashDisks()) 

// U盘列表：[ { path: 'H:\\', name: 'Kingston' }, { path: 'I:\\', name: '金士顿' } ]


```

### For Electron

```
先安装electron-rebuild和相关环境

// 根据所用electron版本修改pakage.json中的相关配置

{
    "scripts": {
        "electron-rebuild": "electron-rebuild --win -x64 -v 1.7.10"
    },
    "devDependencies": {
        "electron-rebuild": "^1.6.0"
    }
}

// 编译成electron版本

npm run electron-rebuild

```
参考
https://github.com/electron/electron-rebuild
