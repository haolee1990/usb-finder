interface DiskList {
    name: string,
    path: string
}

declare let UsbDiskTool: {
    getUsbFlashDisks: () => DiskList[]
}

export default UsbDiskTool;